#include <gin/mesh/mesh.hpp>

#include <Eigen/Core>
#include <Eigen/Dense>

void Gin::Mesh::IndexedMesh::AddVertexData(VertexData& vertexData)
{
	if (vertexIndexTable.count(vertexData.position)) {
		indices.push_back(vertexIndexTable[vertexData.position]);
		return;
	}

	vertexIndexTable.insert({ vertexData.position, vertices.size() });
	indices.push_back(vertices.size());
	vertices.push_back(vertexData);
}

void Gin::Mesh::IndexedMesh::ClearVertexData()
{
	vertexIndexTable.clear();
	vertices.clear();
	indices.clear();
}

size_t Gin::Mesh::IndexedMesh::GetVertexBufferSize()
{
	return vertices.size();
}

size_t Gin::Mesh::IndexedMesh::GetIndexBufferSize()
{
	return indices.size();
}

Gin::Mesh::VertexData* Gin::Mesh::IndexedMesh::GetVertexBuffer()
{
	return vertices.data();
}

int* Gin::Mesh::IndexedMesh::GetIndexBuffer()
{
	return indices.data();
}

void Gin::Mesh::IndexedMesh::RecalculateNormal() {
	for (int i = 0; i < vertices.size(); ++i) {
		vertices[i].normal = Eigen::Vector3<float>{ 0.0f, 0.0f, 0.0f };
	}

	for (int i = 0; i < indices.size(); i += 3) {
		Eigen::Vector3<float> a{ vertices[indices[i]].position };
		Eigen::Vector3<float> b{ vertices[indices[i + 1]].position };
		Eigen::Vector3<float> c{ vertices[indices[i + 2]].position };

		Eigen::Vector3<float> ba{ b - a };
		Eigen::Vector3<float> ca{ c - a };

		Eigen::Vector3<float> normal = ba.cross(ca);
		vertices[indices[i]].normal += normal;
		vertices[indices[i + 1]].normal += normal;
		vertices[indices[i + 2]].normal += normal;
	}

	for (int i = 0; i < vertices.size(); ++i) {
		vertices[i].normal.normalize();
	}
}
