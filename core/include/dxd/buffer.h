#pragma once

#include <memory>
#include <string>
#include <type_traits>
#include <vector>

#include <DirectXMath.h>

namespace DXD
{
	// -------------------------------------------------------------------------------- //
	//						  Constant Buffer Data Structures						    //
	// -------------------------------------------------------------------------------- //
	namespace CB
	{

		struct TRANSFORM_DATA
		{
			DirectX::XMMATRIX World		 = DirectX::XMMatrixIdentity();
			DirectX::XMMATRIX View		 = DirectX::XMMatrixIdentity();
			DirectX::XMMATRIX Projection = DirectX::XMMatrixIdentity();

			static std::string GetBufferDataName()
			{
				return "TRANSFORM_DATA";
			}
		};

		struct MATERIAL_DATA
		{
			DirectX::XMFLOAT4 M_DiffuseColor  = { 1.0f, 1.0f, 1.0f, 1.0f };
			DirectX::XMFLOAT4 M_SpecularColor = { 1.0f, 1.0f, 1.0f, 1.0f };
			DirectX::XMFLOAT4 M_AmbientColor  = { 1.0f, 1.0f, 1.0f, 1.0f };
			// Only 'x' value of Shininess is used for Phong Shading
			DirectX::XMFLOAT4 Shininess		  = { 10.0f, 0.0f, 0.0f, 0.0f };

			static std::string GetBufferDataName()
			{
				return "MATERIAL_DATA";
			}
		};

		struct LIGHTING_DATA
		{
			DirectX::XMFLOAT4 L_DiffuseColor   = { 1.0f, 1.0f, 1.0f, 1.0f };
			DirectX::XMFLOAT4 L_SpecularColor  = { 1.0f, 1.0f, 1.0f, 1.0f };
			DirectX::XMFLOAT4 L_AmbientColor   = { 1.0f, 1.0f, 1.0f, 1.0f };
			DirectX::XMFLOAT4 LightingPosition = { 0.0f, 0.0f, 0.0f, 1.0f };
			DirectX::XMFLOAT4 CameraPosition   = { 0.0f, 0.0f, 0.0f, 1.0f };
	
			static std::string GetBufferDataName()
			{
				return "LIGHTING_DATA";
			}
		};

		// NOTE: Constant Buffer Data should have 'GetBufferName()' method returning a string corresponds to cbuffer name
		template<typename TBufferData, typename = void>
		struct IsConstantBufferData : std::false_type {};

		template<typename TBufferData>
		struct IsConstantBufferData<TBufferData, std::void_t<decltype(TBufferData::GetBufferDataName())>> : std::true_type {};

	} // namespace CB

	// -------------------------------------------------------------------------------- //
	//				   Buffer Wrapper Interface and Helper functions				    //
	// -------------------------------------------------------------------------------- //

	// NOTE: Wrapper doesn't manage life-time of internal data structure
	class IBufferDataWrapper
	{
	public:
		virtual ~IBufferDataWrapper() = default;

		virtual void* GetBufferData() = 0;
		virtual std::string GetBufferDataName() const = 0;
	};

	template<typename TBufferData,
		typename TEnable = std::enable_if_t<CB::IsConstantBufferData<TBufferData>::value>>
	std::unique_ptr<IBufferDataWrapper> WrapBufferData(TBufferData BufferData);

	template<typename... TBufferDataPack>
	std::vector<std::unique_ptr<IBufferDataWrapper>> WrapBufferDataPack(TBufferDataPack&&... Buffers);

	// -------------------------------------------------------------------------------- //
	//							Implmentations and Details								//
	// -------------------------------------------------------------------------------- //

	template<typename TBufferData>
	class UBufferDataWrapper : public IBufferDataWrapper
	{
	public:
		virtual ~UBufferDataWrapper() = default;

		UBufferDataWrapper() = default;
		UBufferDataWrapper(TBufferData BufferDataWrapper);

		UBufferDataWrapper(const UBufferDataWrapper&) = default;
		UBufferDataWrapper& operator=(const UBufferDataWrapper&) = default;

		UBufferDataWrapper(UBufferDataWrapper&&) noexcept = default;
		UBufferDataWrapper& operator=(UBufferDataWrapper&&) noexcept = default;

		virtual void* GetBufferData() override;
		virtual std::string GetBufferDataName() const override;

		void SetBufferData(TBufferData BufferData);

	private:
		TBufferData BufferData;
	};

	template<typename TBufferData>
	UBufferDataWrapper<TBufferData>::UBufferDataWrapper(TBufferData BufferData) : BufferData(BufferData) {}

	template<typename TBufferData>
	void* UBufferDataWrapper<TBufferData>::GetBufferData()
	{
		return reinterpret_cast<void*>(&BufferData);
	}

	template<typename TBufferData>
	std::string UBufferDataWrapper<TBufferData>::GetBufferDataName() const
	{
		return TBufferData::GetBufferDataName();
	}

	template<typename TBufferData>
	void UBufferDataWrapper<TBufferData>::SetBufferData(TBufferData BufferData)
	{
		this->BufferData = BufferData;
	}

	template<typename TBufferData, typename TEnable>
	std::unique_ptr<IBufferDataWrapper> WrapBufferData(TBufferData BufferData)
	{
		return std::make_unique<UBufferDataWrapper<TBufferData>>(BufferData);
	}

	template<typename... TBufferDataPack>
	std::vector<std::unique_ptr<IBufferDataWrapper>> WrapBufferDataPack(TBufferDataPack&&... BufferDataPack)
	{
		std::vector<std::unique_ptr<IBufferDataWrapper>> Result;
		Result.reserve(sizeof...(BufferDataPack));
		(Result.push_back(WrapBufferData(std::forward<TBufferDataPack>(BufferDataPack))), ...);
		return Result;
	}

} // namespace DXD
