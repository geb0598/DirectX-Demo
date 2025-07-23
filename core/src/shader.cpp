#include "dxd/shader.h"

namespace dxd
{

	UShader::UShader(ID3D11Device* Device, const SHADER_DESC& shaderdesc)
	{
		CreateShader(Device, shaderdesc.vsFilePath, shaderdesc.psFilePath, shaderdesc.layout);
	}

	void UShader::UpdateVSConstants(ID3D11DeviceContext* deviceContext, const VS_CONSTANT_BUFFER_DATA& constantBufferData)
	{
		deviceContext->UpdateSubresource(vsConstantBuffer.Get(), 0, nullptr, &constantBufferData, 0, 0);
	}
		
	void UShader::UpdatePSConstants(ID3D11DeviceContext* deviceContext, const PS_CONSTANT_BUFFER_DATA& constantBufferData)
	{
		deviceContext->UpdateSubresource(psConstantBuffer.Get(), 0, nullptr, &constantBufferData, 0, 0);
	}

	void UShader::Bind(ID3D11DeviceContext* deviceContext)
	{
		deviceContext->IASetInputLayout(InputLayout.Get());

		deviceContext->VSSetShader(VertexShader.Get(), nullptr, 0);
		deviceContext->VSSetConstantBuffers(0, 1, vsConstantBuffer.GetAddressOf());

		deviceContext->PSSetShader(PixelShader.Get(), nullptr, 0);
		deviceContext->PSSetConstantBuffers(0, 1, psConstantBuffer.GetAddressOf());
	}

	void UShader::CreateShader(ID3D11Device* device, const std::filesystem::path& vsFilePath, const std::filesystem::path& psFilePath,
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout)	
	{
		if (!std::filesystem::exists(vsFilePath))
		{
			throw std::invalid_argument("Vertex Shader not exist: " + vsFilePath.string());
		}
		if (!std::filesystem::exists(psFilePath))
		{
			throw std::invalid_argument("Pixel Shader not exist: " + psFilePath.string());
		}

		Microsoft::WRL::ComPtr<ID3DBlob> vsCSO;
		Microsoft::WRL::ComPtr<ID3DBlob> psCSO;
		Microsoft::WRL::ComPtr<ID3DBlob> vserrorBlob;
		Microsoft::WRL::ComPtr<ID3DBlob> pserrorBlob;

		HRESULT vs_hr = D3DCompileFromFile(vsFilePath.wstring().c_str(), nullptr, nullptr, "mainVS", "vs_5_0", 0, 0, vsCSO.GetAddressOf(), vserrorBlob.GetAddressOf());
		if (FAILED(vs_hr))
		{
			if (vserrorBlob)
			{
				OutputDebugStringA(static_cast<const char*>(vserrorBlob->GetBufferPointer()));
				throw std::runtime_error("Vertex Shader Compilation Failed: " + std::string(static_cast<const char*>(vserrorBlob->GetBufferPointer())));
			}
			else
			{
				throw std::runtime_error("Vertex Shader Compilation Failed (No error messages available). HRESULT: " + std::to_string(vs_hr));
			}
		}
		device->CreateVertexShader(vsCSO->GetBufferPointer(), vsCSO->GetBufferSize(), nullptr, VertexShader.GetAddressOf());

		HRESULT ps_hr = D3DCompileFromFile(psFilePath.wstring().c_str(), nullptr, nullptr, "mainPS", "ps_5_0", 0, 0, psCSO.GetAddressOf(), pserrorBlob.GetAddressOf());
		if (FAILED(ps_hr))
		{
			if (pserrorBlob)
			{
				OutputDebugStringA(static_cast<const char*>(pserrorBlob->GetBufferPointer()));
				throw std::runtime_error("Pixel Shader Compilation Failed: " + std::string(static_cast<const char*>(pserrorBlob->GetBufferPointer())));
			}
			else
			{
				throw std::runtime_error("Pixel Shader Compilation Failed (No error messages available). HRESULT: " + std::to_string(ps_hr));
			}
		}
		device->CreatePixelShader(psCSO->GetBufferPointer(), psCSO->GetBufferSize(), nullptr, PixelShader.GetAddressOf());

		device->CreateInputLayout(layout.data(), layout.size(), vsCSO->GetBufferPointer(), vsCSO->GetBufferSize(), InputLayout.GetAddressOf());

		D3D11_BUFFER_DESC vsConstantBufferDesc = {};
		vsConstantBufferDesc.ByteWidth = sizeof(VS_CONSTANT_BUFFER_DATA);
		vsConstantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vsConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		vsConstantBufferDesc.CPUAccessFlags = 0;
		vsConstantBufferDesc.MiscFlags = 0;
		vsConstantBufferDesc.StructureByteStride = 0;

		device->CreateBuffer(&vsConstantBufferDesc, nullptr, vsConstantBuffer.GetAddressOf());

		D3D11_BUFFER_DESC psConstantBufferDesc = {};
		psConstantBufferDesc.ByteWidth = sizeof(PS_CONSTANT_BUFFER_DATA);
		psConstantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		psConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		psConstantBufferDesc.CPUAccessFlags = 0;
		psConstantBufferDesc.MiscFlags = 0;
		psConstantBufferDesc.StructureByteStride = 0;

		device->CreateBuffer(&psConstantBufferDesc, nullptr, psConstantBuffer.GetAddressOf());
	}

} // namespace dxd