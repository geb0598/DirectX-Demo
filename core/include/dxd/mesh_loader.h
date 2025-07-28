#pragma once

#include <filesystem>
#include <utility>
#include <vector>
#include <map>

#include "dxd/vertex.h"

namespace DXD
{

	class UObjMeshLoader 
	{
	public:
		static UObjMeshLoader& GetInstance();

		virtual ~UObjMeshLoader() = default;

		UObjMeshLoader(const UObjMeshLoader&) = delete;
		UObjMeshLoader& operator=(const UObjMeshLoader&) = delete;

		UObjMeshLoader(UObjMeshLoader&&) = delete;
		UObjMeshLoader& operator=(UObjMeshLoader&&) = delete;

		template<typename TVertex>
		std::pair<std::vector<TVertex>, std::vector<UINT>>
			LoadMesh(const std::filesystem::path& FilePath);

	private:
		struct FFace
		{
			int PositionIndex = -1;
			int NormalIndex = -1;
			int TexCoordIndex = -1;
		};

		UObjMeshLoader() = default;

		void LoadObjMesh(const std::filesystem::path& FilePath);

		FFace ParseFaceBuffer(const std::string& FaceBuffer);

		std::vector<DirectX::XMFLOAT3> Positions;
		std::vector<DirectX::XMFLOAT3> Normals;
		std::vector<DirectX::XMFLOAT2> TexCoords;
		std::vector<FFace> Faces;
	};

} // namespace DXD
