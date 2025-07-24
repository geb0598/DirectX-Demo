#pragma once

#include <filesystem>
#include <utility>
#include <vector>

#include "dxd/vertex.h"

namespace dxd
{

	// NOTE: Interface for further expansion(e.g., factory method pattern, strategy pattern, etc)
	class IMeshLoader
	{
	public:
		virtual ~IMeshLoader() = default;

		virtual std::pair<std::vector<FVertex>, std::vector<UINT>> 
			LoadMesh(const std::filesystem::path& FilePath) const = 0;
	};

	class OBJMeshLoader : public IMeshLoader
	{
	public:
		virtual ~OBJMeshLoader() = default;

		static const OBJMeshLoader& GetInstance();

		virtual std::pair<std::vector<FVertex>, std::vector<UINT>> 
			LoadMesh(const std::filesystem::path& FilePath) const override;

	private:
		struct FFace
		{
			int PositionIndex = -1;
			int NormalIndex = -1;
			int TexCoordIndex = -1;
		};

		FFace ParseFaceBuffer(const std::string& FaceBuffer) const;

		OBJMeshLoader() = default;
	};

} // namespace dxd
