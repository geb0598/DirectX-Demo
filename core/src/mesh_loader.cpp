#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include <DirectXMath.h>
#include <wrl.h> // UINT

#include "dxd/mesh_loader.h"

namespace dxd
{

	const OBJMeshLoader& OBJMeshLoader::GetInstance()
	{
		static OBJMeshLoader MeshLoader;
		return MeshLoader;
	}

	std::pair<std::vector<FVertex>, std::vector<UINT>> 
		OBJMeshLoader::LoadMesh(const std::filesystem::path& FilePath) const 
	{
		if (!std::filesystem::exists(FilePath))
		{
			throw std::invalid_argument("File not exist: " + FilePath.string());
		}

		std::ifstream File(FilePath);
		if (!File)
		{
			throw std::runtime_error("Failed to open file: " + FilePath.string());
		}

		std::vector<DirectX::XMFLOAT3> Positions;
		std::vector<DirectX::XMFLOAT3> Normals;
		std::vector<DirectX::XMFLOAT2> TexCoords;
		std::vector<FFace> Faces;

		std::string Line;
		while (std::getline(File, Line))
		{
			std::istringstream Tokenizer(Line);

			std::string Prefix;
			Tokenizer >> Prefix;

			if (Prefix == "v")
			{
				DirectX::XMFLOAT3 Position;
				Tokenizer >> Position.x >> Position.y >> Position.z;
				Positions.push_back(Position);
			}
			else if (Prefix == "vn")
			{
				DirectX::XMFLOAT3 Normal;
				Tokenizer >> Normal.x >> Normal.y >> Normal.z;
				Normals.push_back(Normal);
			}
			else if (Prefix == "vt")
			{
				DirectX::XMFLOAT2 TexCoord;
				Tokenizer >> TexCoord.x >> TexCoord.y;
				TexCoords.push_back(TexCoord);
			}
			else if (Prefix == "f")
			{
				std::string FaceBuffer;
				while (Tokenizer >> FaceBuffer)
				{
					// TODO: if more than 3 faces are pushed, then throw exception
					Faces.push_back(ParseFaceBuffer(FaceBuffer));
				}
			}
		}

		std::vector<FVertex> Vertices;
		std::vector<UINT> Indices;
		std::map<std::tuple<int, int, int>, int> IndexMap;
		for (const auto& Face : Faces)
		{
			auto FaceKey = std::make_tuple(Face.PositionIndex, Face.NormalIndex, Face.TexCoordIndex);
			if (IndexMap.find(FaceKey) != IndexMap.end())
			{
				Indices.push_back(IndexMap[FaceKey]);
			}
			else
			{
				IndexMap[FaceKey] = Vertices.size();
				FVertex Vertex;
				if (Face.PositionIndex != -1)
				{
					Vertex.Position = Positions[Face.PositionIndex];
				}
				if (Face.NormalIndex != -1)
				{
					Vertex.Normal = Normals[Face.NormalIndex];
				}
				if (Face.TexCoordIndex != -1)
				{
					Vertex.TexCoord = TexCoords[Face.TexCoordIndex];
				}
				Vertices.push_back(Vertex);
				Indices.push_back(IndexMap[FaceKey]);
			}
		}

		return { Vertices, Indices };
	}

	OBJMeshLoader::FFace OBJMeshLoader::ParseFaceBuffer(const std::string& FaceBuffer) const
	{
		FFace Face = {};
		std::istringstream Tokenizer(FaceBuffer);
		std::string IndexBuffer;

		std::getline(Tokenizer, IndexBuffer, '/');
		// TODO: if IndexBuffer is empty, then throw exception
		Face.PositionIndex = std::stoi(IndexBuffer) - 1;

		if (std::getline(Tokenizer, IndexBuffer, '/') && !IndexBuffer.empty())
		{
			Face.NormalIndex = std::stoi(IndexBuffer) - 1;
		}

		if (std::getline(Tokenizer, IndexBuffer, '/') && !IndexBuffer.empty())
		{
			Face.TexCoordIndex = std::stoi(IndexBuffer) - 1;
		}

		return Face;
	}



} // namespace dxd