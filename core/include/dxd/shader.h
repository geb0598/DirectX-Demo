#pragma once

#pragma comment(lib, "user32")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

#include <filesystem>
#include <string>
#include <vector>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include <wrl.h>

namespace dxd 
{
	struct VS_CONSTANT_BUFFER_DATA
	{
		DirectX::XMMATRIX World;
		DirectX::XMMATRIX View;
		DirectX::XMMATRIX Projection;
	};

	struct PS_CONSTANT_BUFFER_DATA
	{
		DirectX::XMFLOAT4 LightPosition;
	};

	struct SHADER_DESC
	{
		std::filesystem::path vsFilePath;
		std::filesystem::path psFilePath;
		std::vector<D3D11_INPUT_ELEMENT_DESC> layout;
	};

	class UShader
	{
	public:
		~UShader() = default;
		UShader(ID3D11Device* Device, const SHADER_DESC& shaderdesc);

		UShader(const UShader&) = delete;
		UShader(UShader&&) = default;

		UShader& operator=(const UShader&) = delete;
		UShader& operator=(UShader&&) = default;

		void UpdateVSConstants(ID3D11DeviceContext* deviceContext, const VS_CONSTANT_BUFFER_DATA& constantBufferData);
		void UpdatePSConstants(ID3D11DeviceContext* deviceContext, const PS_CONSTANT_BUFFER_DATA& constantBufferData);
		void Bind(ID3D11DeviceContext* deviceContext);

	private:
		void CreateShader(ID3D11Device* device, const std::filesystem::path& vsFilePath, const std::filesystem::path& psFilePath,
			const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout);

		Microsoft::WRL::ComPtr<ID3D11VertexShader> VertexShader = nullptr;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> PixelShader = nullptr;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayout = nullptr;
		Microsoft::WRL::ComPtr<ID3D11Buffer> vsConstantBuffer = nullptr;
		Microsoft::WRL::ComPtr<ID3D11Buffer> psConstantBuffer = nullptr;
	};

} // namespace dxd
