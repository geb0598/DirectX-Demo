#include <filesystem>
#include <map>
#include <string>
#include <vector>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include <wrl.h>

#include "dxd/shader.h"
#include "dxd/log.h"

namespace DXD
{

	UShaderReflector::UShaderReflector(ID3D11Device* Device, ID3DBlob* ShaderBlob)
	{
		ReflectShader(Device, ShaderBlob);
	}

	ID3D11Buffer* UShaderReflector::GetConstantBuffer(const std::string& BufferName) const
	{
		auto Iterator = ConstantBufferMap.find(BufferName);
		if (Iterator == ConstantBufferMap.end())
		{
			throw std::invalid_argument("Failed to find constant buffer: " + BufferName);
		}
		return Iterator->second.Get();
	}

	ID3D11Buffer** UShaderReflector::GetAddressOfConstantBuffer(const std::string& BufferName) 
	{
		auto Iterator = ConstantBufferMap.find(BufferName);
		if (Iterator == ConstantBufferMap.end())
		{
			throw std::invalid_argument("Failed to find constant buffer: " + BufferName);
		}
		return Iterator->second.GetAddressOf();
	}

	UINT UShaderReflector::GetConstantBufferBindPoint(const std::string& BufferName) const
	{
		auto Iterator = ConstantBufferInfoMap.find(BufferName);
		if (Iterator == ConstantBufferInfoMap.end())
		{
			throw std::invalid_argument("Failed to find constant buffer info: " + BufferName);
		}
		return Iterator->second.BindPoint;
	}

	void UShaderReflector::ReflectShader(ID3D11Device* Device, ID3DBlob* ShaderBlob)
	{
		Microsoft::WRL::ComPtr<ID3D11ShaderReflection> ShaderReflector;
		D3DReflect(ShaderBlob->GetBufferPointer(), ShaderBlob->GetBufferSize(), 
			IID_ID3D11ShaderReflection, reinterpret_cast<void**>(ShaderReflector.GetAddressOf()));
		
		D3D11_SHADER_DESC ShaderDesc;
		ShaderReflector->GetDesc(&ShaderDesc);

		for (UINT i = 0; i < ShaderDesc.ConstantBuffers; ++i)
		{
			ID3D11ShaderReflectionConstantBuffer* ConstantBufferReflector 
				= ShaderReflector->GetConstantBufferByIndex(i);

			D3D11_SHADER_BUFFER_DESC ConstantBufferDesc;
			ConstantBufferReflector->GetDesc(&ConstantBufferDesc);

			D3D11_SHADER_INPUT_BIND_DESC ConstantBufferInputBindDesc;
			ShaderReflector->GetResourceBindingDescByName(ConstantBufferDesc.Name, &ConstantBufferInputBindDesc);

			ConstantBufferInfoMap[ConstantBufferDesc.Name] = { ConstantBufferDesc.Size, ConstantBufferInputBindDesc.BindPoint };

			D3D11_BUFFER_DESC BufferDesc = {};
			BufferDesc.ByteWidth		   = ConstantBufferDesc.Size;
			BufferDesc.Usage			   = D3D11_USAGE_DEFAULT;
			BufferDesc.BindFlags		   = D3D11_BIND_CONSTANT_BUFFER;
			BufferDesc.CPUAccessFlags	   = 0;
			BufferDesc.MiscFlags		   = 0;
			BufferDesc.StructureByteStride = 0;

			Microsoft::WRL::ComPtr<ID3D11Buffer> Buffer;
			Device->CreateBuffer(&BufferDesc, nullptr, Buffer.GetAddressOf());

			ConstantBufferMap[ConstantBufferDesc.Name] = Buffer;
		}
	}

	UPixelShader::UPixelShader(ID3D11Device* Device, const std::filesystem::path& FilePath)
	{
		if (!std::filesystem::exists(FilePath))
		{
			throw std::invalid_argument("File not exist: " + FilePath.string());
		}

		Microsoft::WRL::ComPtr<ID3DBlob> ShaderBlob;
		Microsoft::WRL::ComPtr<ID3DBlob> ShaderErrorBlob;

		HRESULT HResult = D3DCompileFromFile(FilePath.wstring().c_str(), nullptr, nullptr,
			"mainPS", "ps_5_0", 0, 0, ShaderBlob.GetAddressOf(), ShaderErrorBlob.GetAddressOf());

		if (FAILED(HResult))
		{
			if (ShaderErrorBlob)
			{
				LOG_FATAL(static_cast<const char*>(ShaderErrorBlob->GetBufferPointer()));
				throw std::runtime_error("Pixel Shader Compilation Failed: " + std::string(static_cast<const char*>(ShaderErrorBlob->GetBufferPointer())));
			}
			else
			{
				throw std::runtime_error("Pixel Shader Compilation Failed (No error messages available). HRESULT: " + std::to_string(HResult));
			}
		}

		CreateShader(Device, ShaderBlob.Get());
		ShaderReflector = std::make_unique<UShaderReflector>(Device, ShaderBlob.Get());
	}

	void UPixelShader::UpdateConstantBuffer(
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

	void UPixelShader::Bind(ID3D11DeviceContext* DeviceContext, const std::vector<std::string>& BufferNames) const
	{
		DeviceContext->PSSetShader(PixelShader.Get(), nullptr, 0);

		// NOTE: If cbuffer named 'BufferName' is not used or not exist, then ignore
		for (const auto& BufferName : BufferNames)
		{
			try
			{
				DeviceContext->PSSetConstantBuffers(ShaderReflector->GetConstantBufferBindPoint(BufferName), 1,
					ShaderReflector->GetAddressOfConstantBuffer(BufferName));
			}
			catch (const std::invalid_argument& Exception)
			{
				LOG_WARN(Exception.what());
			}
		}
	}

	void UPixelShader::CreateShader(ID3D11Device* Device, ID3DBlob* ShaderBlob)
	{
		Device->CreatePixelShader(ShaderBlob->GetBufferPointer(), ShaderBlob->GetBufferSize(),
			nullptr, PixelShader.GetAddressOf());
	}

} // namespace DXD