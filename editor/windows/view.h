#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#include "../editor.h"
#include <gin/mesh/indexedmesh.hpp>
#include <Eigen/Geometry>

class ViewScene {
public:
    void AddMesh(Gin::Mesh::IndexedMesh& mesh, const std::string& meshName);
    void Clear();

private:
    struct MeshData {
        std::string meshName{};
        uint32_t VBOs[3]{}; //Position, Normal, Color
        uint32_t EBO{};
        uint32_t VAO{};
        uint32_t count{};
    };

    std::vector<MeshData> meshes{};
    friend class ViewWindow;
};

class ViewCamera {
public:
    void SetViewportSize(float width, float height);
    void GLViewport();

    void SetNearFarPlane(float zNear, float zFar);

    Eigen::Vector3f GetUp();
    Eigen::Vector3f GetFront();
    Eigen::Vector3f GetRight();

    void SetDirection(const Eigen::Vector3f& direction);
    void SetTarget(const Eigen::Vector3f& target);
    void SetRotation(const Eigen::Quaternionf& rotation);
    void SetPosition(const Eigen::Vector3f& position);
    void LookAt(const Eigen::Vector3f& target, Eigen::Vector3f front, Eigen::Vector3f up);

    Eigen::Affine3f GetViewMatrix();
    Eigen::Matrix4f GetProjectionMatrix();

private:
    float width{}, height{};
    float zNear{ 0.1f }, zFar{ 1000.0f };
    float fov{ 70.0f };
    Eigen::Quaternionf rotation{};
    Eigen::Vector3f position{};

    Eigen::Affine3f vMatrix{};
    Eigen::Matrix4f pMatrix{};
};

class ViewWindow : public EditorWindow {
public:
    void Initialize() final;
    void Draw(bool* open) final;
    std::string_view GetName() final;

    inline ViewScene& GetCurrentScene() { return scene; };

private:
    void UpdateRenderTargetSize();
    void UpdateGBufferSize();
    uint32_t CreateShader(std::string_view vsPath, std::string_view fsPath);
    void UpdateCamera();
    void Render();

private:
    uint32_t width{ 1920 };
    uint32_t height{ 1080 };

    uint32_t gBufferNormalTexture{};
    uint32_t gBufferColorTexture{};
    uint32_t gBufferMetallicRoughnessTexture{};
    uint32_t gBufferDepthBuffer{};
    uint32_t gBufferFrameBuffer{};

    uint32_t renderTargetColorTexture{};
    uint32_t renderTargetFrameBuffer{};

    uint32_t lightingPassProgram{};
    uint32_t lightingPassVAO{};

    uint32_t gPassProgram{};

    bool postprocess{ true };
    bool color{ true };
    bool lighting{ true };
    bool wireframe{ false };

    float zNear{ 0.1f };
    float zFar{ 1000.0f };

    ViewScene scene{};
    ViewCamera camera{};

    struct ViewCameraOrbitSettings {
        Eigen::Vector3f orbitPoint{ 0.0f, 0.0f, 0.0f };
        float distance{ 10.0f };
        float xAngle{};
        float yAngle{};

        float zoomSpeed{ 0.1f };
        float orbitSpeed{ 0.01f };
        float moveSpeed{ 0.0015f };
    } cameraOrbitSettings;
};