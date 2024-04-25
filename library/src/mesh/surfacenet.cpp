#include <gin/mesh/surfacenet.hpp>

#include <gin/math/interpolation.hpp>
#include <gin/math/math.hpp>

void Gin::Mesh::SurfaceNetMeshBuilder::Build(MeshBuildData& data)
{
    SurfaceNetMeshingData meshingData{};

    Math::Vector3 size{ data.bounds.extent * 2.0 / data.scale };
    size.x = std::ceil(size.x);
    size.y = std::ceil(size.y);
    size.z = std::ceil(size.z);

    uint32_t width{ (uint32_t)std::ceil(size.x) };
    uint32_t height{ (uint32_t)std::ceil(size.y) };
    uint32_t depth{ (uint32_t)std::ceil(size.z) };

    meshingData.posToVertexIdx.resize(width * height * depth);

    meshingData.positions.reserve(width * depth);
    meshingData.normals.reserve(width * depth);
    meshingData.colors.reserve(width * depth);
    meshingData.vertexIdxToPos.reserve(width * depth);

    std::fill(meshingData.posToVertexIdx.begin(), meshingData.posToVertexIdx.end(), NULL_VERTEX_IDX);

    for (size_t z = 0; z < depth - 1; ++z) {
        for (size_t y = 0; y < height - 1; ++y) {
            for (size_t x = 0; x < width - 1; ++x) {
                int sum = 0;

                size_t idx = x + y * width + z * width * height;

                float v000 = data.volume.GetScalar(x, y, z);
                float v100 = data.volume.GetScalar(x + 1, y, z);
                float v010 = data.volume.GetScalar(x, y + 1, z);
                float v110 = data.volume.GetScalar(x + 1, y + 1, z);
                float v001 = data.volume.GetScalar(x, y, z + 1);
                float v101 = data.volume.GetScalar(x + 1, y, z + 1);
                float v011 = data.volume.GetScalar(x, y + 1, z + 1);
                float v111 = data.volume.GetScalar(x + 1, y + 1, z + 1);

                sum = (v000 >= 0) + (v100 >= 0) + (v010 >= 0) + (v110 >= 0) + (v001 >= 0) + (v101 >= 0) + (v011 >= 0) + (v111 >= 0);

                if (sum % 8 != 0) {
                    meshingData.posToVertexIdx[idx] = meshingData.positions.size();

                    Math::Vector3 p000{ (float)x, (float)y, (float)z };
                    Math::Vector3 p100{ (float)x + 1.0f, (float)y, (float)z };
                    Math::Vector3 p010{ (float)x, (float)y + 1.0f , (float)z };
                    Math::Vector3 p110{ (float)x + 1.0f, (float)y + 1.0f, (float)z };
                    Math::Vector3 p001{ (float)x, (float)y, (float)z + 1.0f };
                    Math::Vector3 p101{ (float)x + 1.0f, (float)y, (float)z + 1.0f };
                    Math::Vector3 p011{ (float)x, (float)y + 1.0f, (float)z + 1.0f };
                    Math::Vector3 p111{ (float)x + 1.0f, (float)y + 1.0f, (float)z + 1.0f };

                    Math::Vector3 p{ 0.0f, 0.0f, 0.0f };
                    size_t c = 0;

                    if ((v000 >= 0) ^ (v100 >= 0)) {
                        float r = Math::InverseLerp<float>(v000, v100, 0.0f);
                        p += Math::Linear(p000, p100, r);
                        ++c;
                    }
                    if ((v000 >= 0) ^ (v010 >= 0)) {
                        float r = Math::InverseLerp<float>(v000, v010, 0.0f);
                        p += Math::Linear(p000, p010, r);
                        ++c;
                    }
                    if ((v000 >= 0) ^ (v001 >= 0)) {
                        float r = Math::InverseLerp<float>(v000, v001, 0.0f);
                        p += Math::Linear(p000, p001, r);
                        ++c;
                    }

                    if ((v101 >= 0) ^ (v001 >= 0)) {
                        float r = Math::InverseLerp<float>(v101, v001, 0.0f);
                        p += Math::Linear(p101, p001, r);
                        ++c;
                    }
                    if ((v101 >= 0) ^ (v111 >= 0)) {
                        float r = Math::InverseLerp<float>(v101, v111, 0.0f);
                        p += Math::Linear(p101, p111, r);
                        ++c;
                    }
                    if ((v101 >= 0) ^ (v100 >= 0)) {
                        float r = Math::InverseLerp<float>(v101, v100, 0.0f);
                        p += Math::Linear(p101, p100, r);
                        ++c;
                    }

                    if ((v011 >= 0) ^ (v001 >= 0)) {
                        float r = Math::InverseLerp<float>(v011, v001, 0.0f);
                        p += Math::Linear(p011, p001, r);
                        ++c;
                    }
                    if ((v011 >= 0) ^ (v010 >= 0)) {
                        float r = Math::InverseLerp<float>(v011, v010, 0.0f);
                        p += Math::Linear(p011, p010, r);
                        ++c;
                    }
                    if ((v011 >= 0) ^ (v111 >= 0)) {
                        float r = Math::InverseLerp<float>(v011, v111, 0.0f);
                        p += Math::Linear(p011, p111, r);
                        ++c;
                    }

                    if ((v110 >= 0) ^ (v100 >= 0)) {
                        float r = Math::InverseLerp<float>(v110, v100, 0.0f);
                        p += Math::Linear(p110, p100, r);
                        ++c;
                    }
                    if ((v110 >= 0) ^ (v010 >= 0)) {
                        float r = Math::InverseLerp<float>(v110, v010, 0.0f);
                        p += Math::Linear(p110, p010, r);
                        ++c;
                    }
                    if ((v110 >= 0) ^ (v111 >= 0)) {
                        float r = Math::InverseLerp<float>(v110, v111, 0.0f);
                        p += Math::Linear(p110, p111, r);
                        ++c;
                    }

                    p = p / c;

                    meshingData.positions.push_back(Math::Vector3{ p.x, p.y, p.z } * data.scale - data.bounds.extent);
                    Math::Vector3 n = Math::Normalize(Math::Vector3{ v100 - v000, v010 - v000, v001 - v000 });
                    meshingData.normals.push_back(Math::Vector3(n.x, n.y, n.z));
                    meshingData.colors.push_back(data.color.GetVector4(x, y, z));
                    meshingData.vertexIdxToPos.push_back(Math::Vector3Int{ (int)x, (int)y, (int)z });
                }
            }
        }
    }
    
    meshingData.indices.reserve(meshingData.positions.size() * 2);

    for (size_t i = 0; i < meshingData.positions.size(); ++i) {
        Math::Vector3Int position{ meshingData.vertexIdxToPos[i] };

        size_t idx = position.x + position.y * width + position.z * width * height;

        {   //XY Faces
            float vA = data.volume.GetScalar(position.x + 1, position.y + 1, position.z);
            float vB = data.volume.GetScalar(position.x + 1, position.y + 1, position.z + 1);

            if (triangleWindingOrder == TriangleWindingOrder::CLOCK_WISE)
                std::swap(vA, vB);

            if (meshingData.posToVertexIdx[idx + 1] != NULL_VERTEX_IDX && 
                meshingData.posToVertexIdx[idx + width] != NULL_VERTEX_IDX &&
                meshingData.posToVertexIdx[idx + width + 1] != NULL_VERTEX_IDX) {

                if (vA >= 0 && vB < 0) {
                    meshingData.indices.push_back(i);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + width]);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + 1]);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + width]);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + width] + 1);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + 1]);
                }
                else if (vA < 0 && vB >= 0) {
                    meshingData.indices.push_back(i);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + 1]);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + width]);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + width]);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + 1]);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + width] + 1);
                }
            }
        }

        {   //YZ Faces
            float vA = data.volume.GetScalar(position.x, position.y + 1, position.z + 1);
            float vB = data.volume.GetScalar(position.x + 1, position.y + 1, position.z + 1);

            if (triangleWindingOrder == TriangleWindingOrder::CLOCK_WISE)
                std::swap(vA, vB);

            if (meshingData.posToVertexIdx[idx + width] != NULL_VERTEX_IDX &&
                meshingData.posToVertexIdx[idx + width + width * height] != NULL_VERTEX_IDX &&
                meshingData.posToVertexIdx[idx + width * height] != NULL_VERTEX_IDX) {

                if (vA >= 0 && vB < 0) {
                    meshingData.indices.push_back(i);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + width * height]);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + width]);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + width * height]);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + width * height + width]);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + width]);
                }
                else if (vA < 0 && vB >= 0) {
                    meshingData.indices.push_back(i);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + width]);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + width * height]);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + width * height]);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + width]);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + width * height + width]);
                }
            }
        }

        {   //XZ Faces
            float vA = data.volume.GetScalar(position.x + 1, position.y, position.z + 1);
            float vB = data.volume.GetScalar(position.x + 1, position.y + 1, position.z + 1);

            if (triangleWindingOrder == TriangleWindingOrder::CLOCK_WISE)
                std::swap(vA, vB);

            if (meshingData.posToVertexIdx[idx + 1] != NULL_VERTEX_IDX &&
                meshingData.posToVertexIdx[idx + width * height + 1] != NULL_VERTEX_IDX &&
                meshingData.posToVertexIdx[idx + width * height] != NULL_VERTEX_IDX) {

                if (vA >= 0 && vB < 0) {
                    meshingData.indices.push_back(i);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + 1]);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + width * height]);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + width * height]);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + 1]);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + width * height + 1]);
                }
                else if (vA < 0 && vB >= 0) {
                    meshingData.indices.push_back(i);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + width * height]);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + 1]);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + width * height]);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + width * height + 1]);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + 1]);
                }
            }
        }
    }

    data.mesh->Clear();

    data.mesh->SetVertices(meshingData.positions.data(), meshingData.positions.size());
    data.mesh->SetNormals(meshingData.normals.data(), meshingData.normals.size());
    data.mesh->SetColors(meshingData.colors.data(), meshingData.colors.size());
    data.mesh->SetIndices(meshingData.indices.data(), meshingData.indices.size());

}
