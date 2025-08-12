#include <filesystem>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include <DirectXMath.h>
#include <wrl.h> // UINT

#include "dxd/mesh_loader.h"

namespace DXD
{

	UObjMeshLoader& UObjMeshLoader::GetInstance()
	{
		static UObjMeshLoader MeshLoader;
		return MeshLoader;
	}

	template<>
	std::pair<std::vector<FVERTEX_P>, std::vector<UINT>>
		UObjMeshLoader::LoadMesh<FVERTEX_P>(const std::filesystem::path& FilePath)
	{
		LoadObjMesh(FilePath);

		std::vector<FVERTEX_P> Vertices;
		std::vector<UINT> Indices;
		std::map<std::tuple<int, int, int>, UINT> IndexMap;
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
				FVERTEX_P Vertex;
				if (Face.PositionIndex != -1)
				{
					Vertex.Position = Positions[Face.PositionIndex];
				}
				Vertices.push_back(Vertex);
				Indices.push_back(IndexMap[FaceKey]);
			}
		}
		return { Vertices, Indices };
	}

	template<>
	std::pair<std::vector<FVERTEX_PN>, std::vector<UINT>>
		UObjMeshLoader::LoadMesh<FVERTEX_PN>(const std::filesystem::path& FilePath)
	{
		LoadObjMesh(FilePath);

		std::vector<FVERTEX_PN> Vertices;
		std::vector<UINT> Indices;
		std::map<std::tuple<int, int, int>, UINT> IndexMap;
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
				FVERTEX_PN Vertex;
				if (Face.PositionIndex != -1)
				{
					Vertex.Position = Positions[Face.PositionIndex];
				}
				if (Face.NormalIndex != -1)
				{
					Vertex.Normal = Normals[Face.NormalIndex];
				}
				Vertices.push_back(Vertex);
				Indices.push_back(IndexMap[FaceKey]);
			}
		}
		return { Vertices, Indices };
	}

	template<>
	std::pair<std::vector<FVERTEX_PNT>, std::vector<UINT>>
		UObjMeshLoader::LoadMesh<FVERTEX_PNT>(const std::filesystem::path& FilePath)
	{
		LoadObjMesh(FilePath);

		std::vector<FVERTEX_PNT> Vertices;
		std::vector<UINT> Indices;
		std::map<std::tuple<int, int, int>, UINT> IndexMap;
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
				FVERTEX_PNT Vertex;
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

	void UObjMeshLoader::LoadObjMesh(const std::filesystem::path& FilePath)
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

		Positions.clear();
		Normals.clear();
		TexCoords.clear();
		Faces.clear();

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

		std::stringstream ss;
		ss << std::fixed << std::setprecision(6);
		ss << "// Generated from " << FilePath.filename() << std::endl;
		ss << "FVertexSimple vertices[] = {" << std::endl;

		for (const auto& face : Faces)
		{
			DirectX::XMFLOAT3 pos = { 0.0f, 0.0f, 0.0f };
			if (face.PositionIndex >= 0 && face.PositionIndex < Positions.size())
			{
				pos = Positions[face.PositionIndex];
			}

			DirectX::XMFLOAT3 norm = { 0.0f, 0.0f, 0.0f };
			if (face.NormalIndex >= 0 && face.NormalIndex < Normals.size())
			{
				norm = Normals[face.NormalIndex];
			}

			ss << "    {"
				<< pos.x << "f, " << pos.y << "f, " << pos.z << "f, "
				<< norm.x << "f, " << norm.y << "f, " << norm.z << "f, 0.0f"
				<< "}," << std::endl;
		}

		ss << "};" << std::endl;

		auto output_path = FilePath;
		output_path.replace_extension(".h");
		std::ofstream out_file(output_path);
		out_file << ss.str();
		out_file.close();
	}

	UObjMeshLoader::FFace UObjMeshLoader::ParseFaceBuffer(const std::string& FaceBuffer)
	{
		FFace Face = {};
		std::istringstream Tokenizer(FaceBuffer);
		std::string IndexBuffer;

		std::getline(Tokenizer, IndexBuffer, '/');
		// TODO: if IndexBuffer is empty, then throw exception
		Face.PositionIndex = std::stoi(IndexBuffer) - 1;

		if (std::getline(Tokenizer, IndexBuffer, '/') && !IndexBuffer.empty())
		{
			Face.TexCoordIndex = std::stoi(IndexBuffer) - 1;
		}

		if (std::getline(Tokenizer, IndexBuffer, '/') && !IndexBuffer.empty())
		{
			Face.NormalIndex = std::stoi(IndexBuffer) - 1;
		}

		return Face;
	}


} // namespace DXD
