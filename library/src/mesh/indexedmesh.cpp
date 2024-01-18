#include <gin/mesh/indexedmesh.hpp>
#include <Eigen/Dense>

void Gin::Mesh::IndexedMesh::SetVertices(Eigen::Vector3<float>* vertices, size_t count)
{
	this->vertices.resize(count);
	std::memcpy(this->vertices.data(), vertices, count * sizeof(Eigen::Vector3<float>));
}

void Gin::Mesh::IndexedMesh::SetNormals(Eigen::Vector3<float>* normals, size_t count)
{
	this->normals.resize(count);
	std::memcpy(this->normals.data(), normals, count * sizeof(Eigen::Vector3<float>));
}

void Gin::Mesh::IndexedMesh::SetColors(Eigen::Vector4<float>* colors, size_t count)
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
		normals[i] = Eigen::Vector3<float>{ 0.0f, 0.0f, 0.0f };
	}

	for (int i = 0; i < indices.size(); i += 3) {
		Eigen::Vector3<float> a{ vertices[indices[i]] };
		Eigen::Vector3<float> b{ vertices[indices[i + 1]] };
		Eigen::Vector3<float> c{ vertices[indices[i + 2]] };

		Eigen::Vector3<float> ba{ b - a };
		Eigen::Vector3<float> ca{ c - a };

		Eigen::Vector3<float> normal = ba.cross(ca);
		normals[indices[i]] += normal;
		normals[indices[i + 1]] += normal;
		normals[indices[i + 2]] += normal;
	}

	for (int i = 0; i < vertices.size(); ++i) {
		normals[i].normalize();
	}
}

std::vector<Eigen::Vector3<float>>& Gin::Mesh::IndexedMesh::GetVertices()
{
	return vertices;
}

std::vector<Eigen::Vector3<float>>& Gin::Mesh::IndexedMesh::GetNormals()
{
	return normals;
}

std::vector<Eigen::Vector4<float>>& Gin::Mesh::IndexedMesh::GetColors()
{
	return colors;
}

std::vector<unsigned int>& Gin::Mesh::IndexedMesh::GetIndices()
{
	return indices;
}
