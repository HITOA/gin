#include <gin/mesh/surfacenet.hpp>

#include <gin/math/interpolation.hpp>

void Gin::Mesh::SurfaceNetMeshBuilder::Build(MeshBuildData& data)
{
    SurfaceNetMeshingData meshingData{};

    Field::ScalarField<Math::Scalar>& volume{ *data.volume };
    Field::VectorizedVector4Field& color{ *data.color };

    uint32_t width{ volume.GetWidth() };
    uint32_t height{ volume.GetHeight() };
    uint32_t depth{ volume.GetDepth() };

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

                float v000 = volume[idx];
                float v100 = volume[idx + 1];
                float v010 = volume[idx + width];
                float v110 = volume[idx + width + 1];
                float v001 = volume[idx + width * height];
                float v101 = volume[idx + width * height + 1];
                float v011 = volume[idx + width * height + width];
                float v111 = volume[idx + width * height + width + 1];

                sum = (v000 >= 0) + (v100 >= 0) + (v010 >= 0) + (v110 >= 0) + (v001 >= 0) + (v101 >= 0) + (v011 >= 0) + (v111 >= 0);

                if (sum % 8 != 0) {
                    meshingData.posToVertexIdx[idx] = meshingData.positions.size();

                    Eigen::Vector3<float> p000{ (float)x, (float)y, (float)z };
                    Eigen::Vector3<float> p100{ (float)x + 1.0f, (float)y, (float)z };
                    Eigen::Vector3<float> p010{ (float)x, (float)y + 1.0f , (float)z };
                    Eigen::Vector3<float> p110{ (float)x + 1.0f, (float)y + 1.0f, (float)z };
                    Eigen::Vector3<float> p001{ (float)x, (float)y, (float)z + 1.0f };
                    Eigen::Vector3<float> p101{ (float)x + 1.0f, (float)y, (float)z + 1.0f };
                    Eigen::Vector3<float> p011{ (float)x, (float)y + 1.0f, (float)z + 1.0f };
                    Eigen::Vector3<float> p111{ (float)x + 1.0f, (float)y + 1.0f, (float)z + 1.0f };

                    Eigen::Vector3<float> p{ 0.0f, 0.0f, 0.0f };
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

                    meshingData.positions.push_back(Math::Vector3{ p.x(), p.y(), p.z() } * data.scale - data.bounds.extent);
                    auto& n = Eigen::Vector3<float>{ v100 - v000, v010 - v000, v001 - v000 }.normalized();
                    meshingData.normals.push_back(Math::Vector3(n.x(), n.y(), n.z()));
                    meshingData.colors.push_back(color.GetVector4(x, y, z));
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
            size_t idxA = (position.x + 1) + (position.y + 1) * width + position.z * width * height;
            size_t idxB = (position.x + 1) + (position.y + 1) * width + (position.z + 1) * width * height;

            float vA = volume[idxA];
            float vB = volume[idxB];

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
            size_t idxA = position.x + (position.y + 1) * width + (position.z + 1) * width * height;
            size_t idxB = (position.x + 1) + (position.y + 1) * width + (position.z + 1) * width * height;

            float vA = volume[idxA];
            float vB = volume[idxB];

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
            size_t idxA = (position.x + 1) + position.y * width + (position.z + 1) * width * height;
            size_t idxB = (position.x + 1) + (position.y + 1) * width + (position.z + 1) * width * height;

            float vA = volume[idxA];
            float vB = volume[idxB];

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
