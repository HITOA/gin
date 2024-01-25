#include <gin/mesh/indexedmesh.hpp>
#include <gin/math/math.hpp>
#include <Eigen/Dense>

void Gin::Mesh::IndexedMesh::SetVertices(Math::Vector3* vertices, size_t count)
{
	this->vertices.resize(count);
	std::memcpy(this->vertices.data(), vertices, count * sizeof(Eigen::Vector3<float>));
}

void Gin::Mesh::IndexedMesh::SetNormals(Math::Vector3* normals, size_t count)
{
	this->normals.resize(count);
	std::memcpy(this->normals.data(), normals, count * sizeof(Eigen::Vector3<float>));
}

void Gin::Mesh::IndexedMesh::SetColors(Math::Vector4* colors, size_t count)
{
	this->colors.resize(count);
	std::memcpy(this->colors.data(), colors, count * sizeof(Eigen::Vector4<float>));
}

void Gin::Mesh::IndexedMesh::SetIndices(unsigned int* indices, size_t count)
{
	this->indices.resize(count);
	std::memcpy(this->indices.data(), indices, count * sizeof(int));
}

void Gin::Mesh::IndexedMesh::Clear()
{
	vertices.clear();
	normals.clear();
	indices.clear();
}

void Gin::Mesh::IndexedMesh::BuildIndex() {
	indices.resize(vertices.size());

	for (int i = 0; i < vertices.size(); ++i) {
		indices[i] = i;
	}
}

void Gin::Mesh::IndexedMesh::RecalculateNormals()
{
	for (int i = 0; i < vertices.size(); ++i) {
		normals[i] = Math::Vector3{ 0.0f, 0.0f, 0.0f };
	}

	for (int i = 0; i < indices.size(); i += 3) {
		Math::Vector3 a{ vertices[indices[i]] };
		Math::Vector3 b{ vertices[indices[i + 1]] };
		Math::Vector3 c{ vertices[indices[i + 2]] };

		Math::Vector3 ba{ b - a };
		Math::Vector3 ca{ c - a };

		Math::Vector3 normal = Math::Cross(ba, ca);
		normals[indices[i]] += normal;
		normals[indices[i + 1]] += normal;
		normals[indices[i + 2]] += normal;
	}

	for (int i = 0; i < vertices.size(); ++i) {
		normals[i] = Math::Normalize(normals[i]);
	}
}

std::vector<Gin::Math::Vector3>& Gin::Mesh::IndexedMesh::GetVertices()
{
	return vertices;
}

std::vector<Gin::Math::Vector3>& Gin::Mesh::IndexedMesh::GetNormals()
{
	return normals;
}

std::vector<Gin::Math::Vector4>& Gin::Mesh::IndexedMesh::GetColors()
{
	return colors;
}

std::vector<unsigned int>& Gin::Mesh::IndexedMesh::GetIndices()
{
	return indices;
}
