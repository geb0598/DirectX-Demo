#pragma once

#include <filesystem>
#include <map>
#include <string>
#include <type_traits>
#include <vector>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include <wrl.h>

#include "dxd/vertex.h"
#include "dxd/log.h"

namespace DXD 
{
	// -------------------------------------------------------------------------------- //
	//				           Interface and Helper functions       				    //
	// -------------------------------------------------------------------------------- //
	class IShader
	{
	public:
		virtual ~IShader() = default;

		virtual void UpdateConstantBuffer(
			ID3D11DeviceContext* DeivceContext, const std::string& BufferName, const void* BufferData) const = 0;
		virtual void Bind(ID3D11DeviceContext* DeviceContext, const std::vector<std::string>& BufferNames) const = 0;
	};

	// -------------------------------------------------------------------------------- //
	class UShaderReflector
	{
	public:
		~UShaderReflector() = default;

		UShaderReflector(ID3D11Device* Device, ID3DBlob* ShaderBlob);

		UShaderReflector(const UShaderReflector&) = delete;
		UShaderReflector& operator=(const UShaderReflector&) = delete;

		UShaderReflector(UShaderReflector&&) noexcept = default;
		UShaderReflector& operator=(UShaderReflector&&) noexcept = default;		

		ID3D11Buffer* GetConstantBuffer(const std::string& BufferName) const;
		ID3D11Buffer** GetAddressOfConstantBuffer(const std::string& BufferName);
		UINT GetConstantBufferBindPoint(const std::string& BufferName) const;

	private:
		struct FConstantBufferInfo
		{
			UINT Size;
			UINT BindPoint;
		};

		void ReflectShader(ID3D11Device* Device, ID3DBlob* ShaderBlob);

		std::map<std::string, Microsoft::WRL::ComPtr<ID3D11Buffer>> ConstantBufferMap;
		std::map<std::string, FConstantBufferInfo> ConstantBufferInfoMap;
	};

	template<typename TVertex, typename TEnable = std::enable_if_t<IsVertex<TVertex>::value>>
	class UVertexShader : public IShader
	{
	public:
		~UVertexShader() = default;

		UVertexShader(ID3D11Device* Device, const std::filesystem::path& FilePath);

		UVertexShader(const UVertexShader&) = delete;
		UVertexShader& operator=(const UVertexShader&) = delete;

		UVertexShader(UVertexShader&&) noexcept = default;
		UVertexShader& operator=(UVertexShader&&) noexcept = default;

		virtual void UpdateConstantBuffer(
			ID3D11DeviceContext* DeivceContext, const std::string& BufferName, const void* BufferData) const override;
		virtual void Bind(ID3D11DeviceContext* DeviceContext, const std::vector<std::string>& BufferNames) const override;

	private:
		void CreateShader(ID3D11Device* Device, ID3DBlob* ShaderBlob);

		Microsoft::WRL::ComPtr<ID3D11VertexShader> VertexShader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayout;

		std::unique_ptr<UShaderReflector> ShaderReflector;
	};

	class UPixelShader : public IShader
	{
	public:
		~UPixelShader() = default;

		UPixelShader(ID3D11Device* Device, const std::filesystem::path& FilePath);

		UPixelShader(const UPixelShader&) = delete;
		UPixelShader& operator=(const UPixelShader&) = delete;

		UPixelShader(UPixelShader&&) noexcept = default;
		UPixelShader& operator=(UPixelShader&&) noexcept = default;

		virtual void UpdateConstantBuffer(
			ID3D11DeviceContext* DeivceContext, const std::string& BufferName, const void* BufferData) const override;
		virtual void Bind(ID3D11DeviceContext* DeviceContext, const std::vector<std::string>& BufferNames) const override;

	private:
		void CreateShader(ID3D11Device* Device, ID3DBlob* ShaderBlob);

		Microsoft::WRL::ComPtr<ID3D11PixelShader> PixelShader;

		std::unique_ptr<UShaderReflector> ShaderReflector;
	};

	// -------------------------------------------------------------------------------- //
	template<typename TVertex, typename TEnable>
	UVertexShader<TVertex, TEnable>::UVertexShader(ID3D11Device* Device, const std::filesystem::path& FilePath)
	{
		if (!std::filesystem::exists(FilePath))
		{
			throw std::invalid_argument("File not exist: " + FilePath.string());
		}

		Microsoft::WRL::ComPtr<ID3DBlob> ShaderBlob;
		Microsoft::WRL::ComPtr<ID3DBlob> ShaderErrorBlob;

		HRESULT HResult = D3DCompileFromFile(FilePath.wstring().c_str(), nullptr, nullptr,
			"mainVS", "vs_5_0", 0, 0, ShaderBlob.GetAddressOf(), ShaderErrorBlob.GetAddressOf());

		if (FAILED(HResult))
		{
			if (ShaderErrorBlob)
			{
				LOG_FATAL(static_cast<const char*>(ShaderErrorBlob->GetBufferPointer()))
				throw std::runtime_error("Vertex Shader Compilation Failed: " + std::string(static_cast<const char*>(ShaderErrorBlob->GetBufferPointer())));
			}
			else
			{
				throw std::runtime_error("Vertex Shader Compilation Failed (No error messages available). HRESULT: " + std::to_string(HResult));
			}
		}

		CreateShader(Device, ShaderBlob.Get());
		ShaderReflector = std::make_unique<UShaderReflector>(Device, ShaderBlob.Get());

		std::vector<D3D11_INPUT_ELEMENT_DESC> InputLayoutDesc = TVertex::GetInputLayoutDesc();
		Device->CreateInputLayout(InputLayoutDesc.data(), InputLayoutDesc.size(),
			ShaderBlob->GetBufferPointer(), ShaderBlob->GetBufferSize(), InputLayout.GetAddressOf());
	}

	template<typename TVertex, typename TEnable>
	void UVertexShader<TVertex, TEnable>::UpdateConstantBuffer(
		ID3D11DeviceContext* DeviceContext, const std::string& BufferName, const void* BufferData) const
	{
		// NOTE: If cbuffer named 'BufferName' is not used or not exist, then ignore
		try
		{
			DeviceContext->UpdateSubresource(ShaderReflector->GetConstantBuffer(BufferName), 0, nullptr, BufferData, 0, 0);
		}
		catch (const std::invalid_argument& Exception)
		{
			LOG_WARN(Exception.what());
		}
	}

	template<typename TVertex, typename TEnable>
	void UVertexShader<TVertex, TEnable>::Bind(ID3D11DeviceContext* DeviceContext, const std::vector<std::string>& BufferNames) const
	{
		DeviceContext->IASetInputLayout(InputLayout.Get());

		DeviceContext->VSSetShader(VertexShader.Get(), nullptr, 0);

		// NOTE: If cbuffer named 'BufferName' is not used or not exist, then ignore
		for (const auto& BufferName : BufferNames)
		{
			try
			{
				DeviceContext->VSSetConstantBuffers(ShaderReflector->GetConstantBufferBindPoint(BufferName), 1,
					ShaderReflector->GetAddressOfConstantBuffer(BufferName));
			}
			catch (const std::invalid_argument& Exception)
			{
				LOG_WARN(Exception.what());
			}
		}
	}

	template<typename TVertex, typename TEnable>
	void UVertexShader<TVertex, TEnable>::CreateShader(ID3D11Device* Device, ID3DBlob* ShaderBlob)
	{
		Device->CreateVertexShader(ShaderBlob->GetBufferPointer(), ShaderBlob->GetBufferSize(),
			nullptr, VertexShader.GetAddressOf());
	}

} // namespace DXD
