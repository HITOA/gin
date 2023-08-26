#include <gin/mesh/indexedmesh.hpp>

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

void Gin::Mesh::IndexedMesh::SetIndices(int* indices, size_t count)
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

std::vector<Eigen::Vector3<float>>& Gin::Mesh::IndexedMesh::GetVertices()
{
	return vertices;
}

std::vector<Eigen::Vector3<float>>& Gin::Mesh::IndexedMesh::GetNormals()
{
	return normals;
}

std::vector<Gin::Mesh::IndexedMeshVertexData> Gin::Mesh::IndexedMesh::GetIndexedMeshVertexData()
{
	std::vector<IndexedMeshVertexData> vertexData{};

	if (vertices.size() != normals.size())
		return vertexData;

	vertexData.resize(vertices.size());

	for (size_t i = 0; i < vertexData.size(); ++i) {
		IndexedMeshVertexData t{};

		t.position = vertices[i];
		t.normals = normals[i];

		vertexData[i] = t;
	}

	return vertexData;
}

std::vector<int>& Gin::Mesh::IndexedMesh::GetIndices()
{
	return indices;
}
