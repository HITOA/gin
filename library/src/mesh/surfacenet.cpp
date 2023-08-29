#include <gin/mesh/surfacenet.hpp>

#include <gin/math/interpolation.hpp>

void Gin::Mesh::SurfaceNetMeshBuilder::Build(Mesh& mesh, Spatial::Sampler<float>& volume, Spatial::Sampler<Eigen::Vector4<float>>& colors)
{
    SurfaceNetMeshingData meshingData{};

    meshingData.posToVertexIdx.resize(volume->GetWidth() * volume->GetHeight() * volume->GetDepth());

    meshingData.positions.reserve(volume->GetWidth() * volume->GetDepth());
    meshingData.normals.reserve(volume->GetWidth() * volume->GetDepth());
    meshingData.colors.reserve(volume->GetWidth() * volume->GetDepth());
    meshingData.vertexIdxToPos.reserve(volume->GetWidth() * volume->GetDepth());

    std::fill(meshingData.posToVertexIdx.begin(), meshingData.posToVertexIdx.end(), NULL_VERTEX_IDX);

    for (size_t z = 0; z < volume->GetDepth() - 1; ++z) {
        for (size_t y = 0; y < volume->GetHeight() - 1; ++y) {
            for (size_t x = 0; x < volume->GetWidth() - 1; ++x) {
                int sum = 0;

                size_t idx = x + y * volume->GetWidth() + z * volume->GetWidth() * volume->GetHeight();

                float v000 = volume[idx];
                float v100 = volume[idx + 1];
                float v010 = volume[idx + volume->GetWidth()];
                float v110 = volume[idx + volume->GetWidth() + 1];
                float v001 = volume[idx + volume->GetWidth() * volume->GetHeight()];
                float v101 = volume[idx + volume->GetWidth() * volume->GetHeight() + 1];
                float v011 = volume[idx + volume->GetWidth() * volume->GetHeight() + volume->GetWidth()];
                float v111 = volume[idx + volume->GetWidth() * volume->GetHeight() + volume->GetWidth() + 1];

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

                    meshingData.positions.push_back(p * volume.GetScale() - volume.GetBounds().extent.cast<float>());

                    meshingData.normals.push_back(Eigen::Vector3<float>{ v100 - v000, v010 - v000, v001 - v000 }.normalized());
                    meshingData.colors.push_back(colors[idx]);
                    meshingData.vertexIdxToPos.push_back(Eigen::Vector3<int>{ (int)x, (int)y, (int)z });
                }
            }
        }
    }
    
    meshingData.indices.reserve(meshingData.positions.size() * 2);

    for (size_t i = 0; i < meshingData.positions.size(); ++i) {
        Eigen::Vector3<int> position{ meshingData.vertexIdxToPos[i] };

        size_t idx = position.x() + position.y() * volume->GetWidth() + position.z() * volume->GetWidth() * volume->GetHeight();

        {   //XY Faces
            size_t idxA = (position.x() + 1) + (position.y() + 1) * volume->GetWidth() + position.z() * volume->GetWidth() * volume->GetHeight();
            size_t idxB = (position.x() + 1) + (position.y() + 1) * volume->GetWidth() + (position.z() + 1) * volume->GetWidth() * volume->GetHeight();

            float vA = volume[idxA];
            float vB = volume[idxB];

            if (meshingData.posToVertexIdx[idx + 1] != NULL_VERTEX_IDX && 
                meshingData.posToVertexIdx[idx + volume->GetWidth()] != NULL_VERTEX_IDX &&
                meshingData.posToVertexIdx[idx + volume->GetWidth() + 1] != NULL_VERTEX_IDX) {

                if (vA >= 0 && vB < 0) {
                    meshingData.indices.push_back(i);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + volume->GetWidth()]);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + 1]);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + volume->GetWidth()]);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + volume->GetWidth()] + 1);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + 1]);
                }
                else if (vA < 0 && vB >= 0) {
                    meshingData.indices.push_back(i);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + 1]);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + volume->GetWidth()]);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + volume->GetWidth()]);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + 1]);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + volume->GetWidth()] + 1);
                }
            }
        }

        {   //YZ Faces
            size_t idxA = position.x() + (position.y() + 1) * volume->GetWidth() + (position.z() + 1) * volume->GetWidth() * volume->GetHeight();
            size_t idxB = (position.x() + 1) + (position.y() + 1) * volume->GetWidth() + (position.z() + 1) * volume->GetWidth() * volume->GetHeight();

            float vA = volume[idxA];
            float vB = volume[idxB];

            if (meshingData.posToVertexIdx[idx + volume->GetWidth()] != NULL_VERTEX_IDX &&
                meshingData.posToVertexIdx[idx + volume->GetWidth() + volume->GetWidth() * volume->GetHeight()] != NULL_VERTEX_IDX &&
                meshingData.posToVertexIdx[idx + volume->GetWidth() * volume->GetHeight()] != NULL_VERTEX_IDX) {

                if (vA >= 0 && vB < 0) {
                    meshingData.indices.push_back(i);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + volume->GetWidth() * volume->GetHeight()]);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + volume->GetWidth()]);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + volume->GetWidth() * volume->GetHeight()]);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + volume->GetWidth() * volume->GetHeight() + volume->GetWidth()]);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + volume->GetWidth()]);
                }
                else if (vA < 0 && vB >= 0) {
                    meshingData.indices.push_back(i);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + volume->GetWidth()]);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + volume->GetWidth() * volume->GetHeight()]);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + volume->GetWidth() * volume->GetHeight()]);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + volume->GetWidth()]);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + volume->GetWidth() * volume->GetHeight() + volume->GetWidth()]);
                }
            }
        }

        {   //XZ Faces
            size_t idxA = (position.x() + 1) + position.y() * volume->GetWidth() + (position.z() + 1) * volume->GetWidth() * volume->GetHeight();
            size_t idxB = (position.x() + 1) + (position.y() + 1) * volume->GetWidth() + (position.z() + 1) * volume->GetWidth() * volume->GetHeight();

            float vA = volume[idxA];
            float vB = volume[idxB];

            if (meshingData.posToVertexIdx[idx + 1] != NULL_VERTEX_IDX &&
                meshingData.posToVertexIdx[idx + volume->GetWidth() * volume->GetHeight() + 1] != NULL_VERTEX_IDX &&
                meshingData.posToVertexIdx[idx + volume->GetWidth() * volume->GetHeight()] != NULL_VERTEX_IDX) {

                if (vA >= 0 && vB < 0) {
                    meshingData.indices.push_back(i);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + 1]);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + volume->GetWidth() * volume->GetHeight()]);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + volume->GetWidth() * volume->GetHeight()]);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + 1]);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + volume->GetWidth() * volume->GetHeight() + 1]);
                }
                else if (vA < 0 && vB >= 0) {
                    meshingData.indices.push_back(i);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + volume->GetWidth() * volume->GetHeight()]);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + 1]);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + volume->GetWidth() * volume->GetHeight()]);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + volume->GetWidth() * volume->GetHeight() + 1]);
                    meshingData.indices.push_back(meshingData.posToVertexIdx[idx + 1]);
                }
            }
        }
    }

    mesh.Clear();

    mesh.SetVertices(meshingData.positions.data(), meshingData.positions.size());
    mesh.SetNormals(meshingData.normals.data(), meshingData.normals.size());
    mesh.SetColors(meshingData.colors.data(), meshingData.colors.size());
    mesh.SetIndices(meshingData.indices.data(), meshingData.indices.size());

}
