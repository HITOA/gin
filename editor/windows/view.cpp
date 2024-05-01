#include "view.hpp"

#include <glad/gl.h>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <GLFW/glfw3.h>
#include <Eigen/Core>

void ViewScene::AddMesh(Gin::Mesh::IndexedMesh& mesh, const std::string &meshName) {
    MeshData meshData{};
    meshData.meshName = meshName;

    glGenBuffers(3, meshData.VBOs);
    glGenVertexArrays(1, &meshData.VAO);
    glBindVertexArray(meshData.VAO);

    //Position
    glBindBuffer(GL_ARRAY_BUFFER, meshData.VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, mesh.GetVertices().size() * sizeof(float) * 3, mesh.GetVertices().data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Normal
    glBindBuffer(GL_ARRAY_BUFFER, meshData.VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, mesh.GetNormals().size() * sizeof(float) * 3, mesh.GetNormals().data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    //Color
    glBindBuffer(GL_ARRAY_BUFFER, meshData.VBOs[2]);
    glBufferData(GL_ARRAY_BUFFER, mesh.GetColors().size() * sizeof(float) * 4, mesh.GetColors().data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(2);

    //EBO
    glGenBuffers(1, &meshData.EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshData.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.GetIndices().size() * sizeof(unsigned int), mesh.GetIndices().data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    meshData.count = mesh.GetIndices().size();

    meshes.push_back(meshData);
}

void ViewScene::Clear() {
    for (auto& mesh : meshes) {
        glDeleteBuffers(3, mesh.VBOs);
        glDeleteBuffers(1, &mesh.EBO);
        glDeleteVertexArrays(1, &mesh.VAO);
    }
    meshes.clear();
}

void ViewCamera::SetViewportSize(float width, float height) {
    this->width = width;
    this->height = height;
}

void ViewCamera::GLViewport() {
    glViewport(0, 0, width, height);
}

void ViewCamera::SetNearFarPlane(float zNear, float zFar) {
    this->zNear = zNear;
    this->zFar = zFar;
}

Eigen::Vector3f ViewCamera::GetUp() {
    return rotation * Eigen::Vector3f::UnitY();
}

Eigen::Vector3f ViewCamera::GetFront() {
    return rotation * Eigen::Vector3f::UnitZ();
}

Eigen::Vector3f ViewCamera::GetRight() {
    return rotation * Eigen::Vector3f::UnitX();
}

void ViewCamera::SetDirection(const Eigen::Vector3f &direction) {
    Eigen::Vector3f up = Eigen::Vector3f::UnitY();
    Eigen::Matrix3f camAxes;
    camAxes.col(2) = (-direction).normalized();
    camAxes.col(0) = up.cross(camAxes.col(2)).normalized();
    camAxes.col(1) = camAxes.col(2).cross(camAxes.col(0)).normalized();
    SetRotation(Eigen::Quaternionf{ camAxes });
}

void ViewCamera::SetTarget(const Eigen::Vector3f &target) {
    Eigen::Vector3f dir = target - position;
    SetDirection(dir.normalized());
}

void ViewCamera::SetRotation(const Eigen::Quaternionf &rotation) {
    this->rotation = rotation;
}

void ViewCamera::SetPosition(const Eigen::Vector3f &position) {
    this->position = position;
}

void ViewCamera::LookAt(const Eigen::Vector3f &target, Eigen::Vector3f front, Eigen::Vector3f up) {
    front = -front;
    Eigen::Vector3f toVector = (target - position).normalized();
    Eigen::Vector3f rotAxis = front.cross(toVector).normalized();
    if (rotAxis.squaredNorm() == 0)
        rotAxis = up;

    float d = front.dot(toVector);
    float a = std::acos(d);

    SetRotation(Eigen::Quaternionf{ Eigen::AngleAxisf{ a, rotAxis } });
}

Eigen::Affine3f ViewCamera::GetViewMatrix() {
    Eigen::Quaternionf q = rotation.conjugate();
    vMatrix.linear() = q.toRotationMatrix();
    vMatrix.translation() = -(vMatrix.linear() * position);
    return vMatrix;
}

Eigen::Matrix4f ViewCamera::GetProjectionMatrix() {
    pMatrix.setIdentity();
    float ar = width / height;
    float t = fov * 0.5f;
    float r = zFar - zNear;
    float invt = 1.0f/std::tan(t);

    pMatrix(0, 0) = invt / ar;
    pMatrix(1, 1) = invt;
    pMatrix(2, 2) = -(zNear + zFar) / r;
    pMatrix(3, 2) = -1;
    pMatrix(2, 3) = -2 * zNear * zFar / r;
    pMatrix(3, 3) = 0;
    return pMatrix;
}

void ViewWindow::Initialize() {
    glGenFramebuffers(1, &renderTargetFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, renderTargetFrameBuffer);

    glGenTextures(1, &renderTargetColorTexture);

    UpdateRenderTargetSize();

    glBindTexture(GL_TEXTURE_2D, renderTargetColorTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTargetColorTexture, 0);

    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw std::runtime_error{ "View Render Target is not complete." };

    glGenFramebuffers(1, &gBufferFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBufferFrameBuffer);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glGenTextures(1, &gBufferNormalTexture);
    glGenTextures(1, &gBufferColorTexture);
    glGenTextures(1, &gBufferMetallicRoughnessTexture);
    glGenTextures(1, &gBufferDepthBuffer);

    UpdateGBufferSize();

    glBindTexture(GL_TEXTURE_2D, gBufferNormalTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gBufferNormalTexture, 0);

    glBindTexture(GL_TEXTURE_2D, gBufferColorTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gBufferColorTexture, 0);

    glBindTexture(GL_TEXTURE_2D, gBufferMetallicRoughnessTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gBufferMetallicRoughnessTexture, 0);

    glBindTexture(GL_TEXTURE_2D, gBufferDepthBuffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, gBufferDepthBuffer, 0);

    uint32_t gBufferAttachments[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, gBufferAttachments);

    //std::cout << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw std::runtime_error{ "View GBuffer is not complete." };

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    lightingPassProgram = CreateShader("data/shaders/lightingpass.vs", "data/shaders/lightingpass.fs");
    gPassProgram = CreateShader("data/shaders/gpass.vs", "data/shaders/gpass.fs");

    glGenVertexArrays(1, &lightingPassVAO);
}

void ViewWindow::Draw(bool *open) {
    if (ImGui::Begin("View", open)) {
        ImGui::BeginChild("ViewRender");

        ImGui::SetItemKeyOwner(ImGuiKey_LeftShift);
        ImGui::SetItemKeyOwner(ImGuiKey_MouseMiddle);
        ImGui::SetItemKeyOwner(ImGuiKey_MouseWheelY);
        ImGui::SetItemKeyOwner(ImGuiKey_F);

        ImVec2 wsize = ImGui::GetWindowSize();
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        ImVec2 pos = ImGui::GetCursorScreenPos();

        if (width != wsize.x || height != wsize.y) {
            width = wsize.x;
            height = wsize.y;
            UpdateRenderTargetSize();
            UpdateGBufferSize();
            camera.SetViewportSize(width, height);
        }

        UpdateCamera();
        Render();

        //ImGui::Image((ImTextureID)renderTargetColorTexture, wsize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        drawList->AddImageRounded((ImTextureID)renderTargetColorTexture, pos, pos + wsize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 }, IM_COL32_WHITE, ImGui::GetStyle().WindowRounding);

        ImGui::BeginVertical("ViewVerticalSpacing");
        ImGui::Spring(1.0f, ImGui::GetStyle().WindowPadding.y);
        ImGui::BeginHorizontal("ViewHorizontalSpacing");
        ImGui::Spring(1.0f, ImGui::GetStyle().WindowPadding.x);

        ImGui::PushItemWidth(125.0f);
        if (ImGui::BeginCombo("##ViewTypeCombo", "\uF013 View")) {

            ImGui::Checkbox("Lighting", &lighting);
            ImGui::Checkbox("Color", &color);
            ImGui::Separator();

            ImGui::Checkbox("Post Processing", &postprocess);
            ImGui::Separator();

            ImGui::Checkbox("Wireframe", &wireframe);

            ImGui::EndCombo();
        }

        ImGui::PopItemWidth();
        ImGui::Spring(1.0f);
        ImGui::PushItemWidth(125.0f);
        if (ImGui::BeginCombo("##ViewCameraCombo", "\uF013 Camera")) {

            ImGui::SliderFloat("Far Plane", &zFar, 100.0f, 10000.0f);

            ImGui::EndCombo();
        }

        ImGui::PopItemWidth();

        ImVec2 size = ImGui::CalcTextSize("Tric Count : XXXXXXXXX") + ImGui::GetStyle().FramePadding * 2;

        ImGui::Spring(1.0f, ImGui::GetWindowContentRegionWidth() - ImGui::GetCursorPosX() - size.x);

        uint32_t triCount{ 0 };
        for (auto& mesh : scene.meshes)
            triCount += mesh.count / 3;

        pos = ImGui::GetCursorScreenPos();
        drawList->AddRectFilled(pos, pos + size, ImColor{ ImGui::GetStyle().Colors[ImGuiCol_FrameBg] });
        ImGui::Text("Tri Count : %u", triCount);

        ImGui::EndHorizontal();
        ImGui::EndVertical();

        ImGui::EndChild();
    }
    ImGui::End();
}

std::string_view ViewWindow::GetName() {
    return "View";
}

void ViewWindow::UpdateRenderTargetSize() {
    glBindTexture(GL_TEXTURE_2D, renderTargetColorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
}

void ViewWindow::UpdateGBufferSize() {
    glBindTexture(GL_TEXTURE_2D, gBufferNormalTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB10, width, height, 0, GL_RGB, GL_FLOAT, nullptr);

    glBindTexture(GL_TEXTURE_2D, gBufferColorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_FLOAT, nullptr);

    glBindTexture(GL_TEXTURE_2D, gBufferMetallicRoughnessTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB4, width, height, 0, GL_RGB, GL_FLOAT, nullptr);

    glBindTexture(GL_TEXTURE_2D, gBufferDepthBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
}

int CheckShaderOrProgram(uint32_t handle, bool program = false) {
    int success{};
    char msg[512]{};
    if (!program) {
        glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(handle, 512, nullptr, msg);
            std::cout << msg << std::endl;
        }
    } else {
      glGetProgramiv(handle, GL_LINK_STATUS, &success);
      if (!success) {
          glGetProgramInfoLog(handle, 512, nullptr, msg);
          std::cout << msg << std::endl;
      }
    }
    return success;
}

uint32_t ViewWindow::CreateShader(std::string_view vsPath, std::string_view fsPath) {
    std::ifstream vsFile{ std::string{ vsPath }, std::ios_base::binary };
    std::ifstream fsFile{ std::string{ fsPath }, std::ios_base::binary };

    if (!vsFile.good() || !fsFile.good())
        throw std::runtime_error{ "Can't read shader file." };

    vsFile.ignore( std::numeric_limits<std::streamsize>::max() );
    std::streamsize vsSize = vsFile.gcount();
    vsFile.clear();
    vsFile.seekg( 0, std::ios_base::beg );

    fsFile.ignore( std::numeric_limits<std::streamsize>::max() );
    std::streamsize fsSize = fsFile.gcount();
    fsFile.clear();
    fsFile.seekg( 0, std::ios_base::beg );

    char* vsData = (char*)malloc(vsSize + 1);
    char* fsData = (char*)malloc(fsSize + 1);

    vsFile.read(vsData, vsSize);
    fsFile.read(fsData, fsSize);
    vsData[vsSize] = 0;
    fsData[fsSize] = 0;

    uint32_t vs = glCreateShader(GL_VERTEX_SHADER);
    uint32_t fs = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vs, 1, &vsData, nullptr);
    glCompileShader(vs);
    glShaderSource(fs, 1, &fsData, nullptr);
    glCompileShader(fs);

    free(vsData);
    free(fsData);

    if (!CheckShaderOrProgram(vs) || !CheckShaderOrProgram(fs))
        return 0;

    uint32_t program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    if (!CheckShaderOrProgram(program, true)) {
        glDeleteProgram(program);
        return 0;
    }

    return program;
}

void ViewWindow::UpdateCamera() {
    camera.SetNearFarPlane(zNear, zFar);

    if (ImGui::IsWindowHovered()) {
        if (ImGui::IsKeyPressed(ImGuiKey_F)) {
            cameraOrbitSettings.orbitPoint = Eigen::Vector3f::Zero();
        }
        if (ImGui::IsKeyDown(ImGuiKey_MouseMiddle)) {
            ImVec2 mouseDelta = ImGui::GetIO().MouseDelta;
            if (ImGui::IsKeyDown(ImGuiKey_LeftShift)) {
                float moveSpeed = cameraOrbitSettings.moveSpeed * cameraOrbitSettings.distance;
                cameraOrbitSettings.orbitPoint -= camera.GetRight() * mouseDelta.x * moveSpeed;
                cameraOrbitSettings.orbitPoint += camera.GetUp() * mouseDelta.y *  moveSpeed;
            } else {
                cameraOrbitSettings.yAngle -= mouseDelta.x * cameraOrbitSettings.orbitSpeed;
                cameraOrbitSettings.xAngle -= mouseDelta.y * cameraOrbitSettings.orbitSpeed;
            }
        }
        cameraOrbitSettings.distance -= ImGui::GetIO().MouseWheel * cameraOrbitSettings.zoomSpeed *
                cameraOrbitSettings.distance;
    }

    cameraOrbitSettings.distance = std::clamp(cameraOrbitSettings.distance, 0.5f, 100000.0f);

    Eigen::AngleAxisf yr{ cameraOrbitSettings.yAngle, Eigen::Vector3f::UnitY() };
    Eigen::AngleAxisf xr{ cameraOrbitSettings.xAngle, Eigen::Vector3f::UnitX() };

    Eigen::Vector3f normal = yr * xr * (Eigen::Vector3f::UnitZ()).normalized();

    camera.SetPosition(cameraOrbitSettings.orbitPoint + normal * cameraOrbitSettings.distance);
    camera.SetRotation(yr * xr);
}

void ViewWindow::Render() {
    camera.GLViewport();
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    if (wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    //GBuffer Pass

    glBindFramebuffer(GL_FRAMEBUFFER, gBufferFrameBuffer);
    glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    static const float gBufferColorClear[3] = { 0.2f, 0.2f, 0.25f };
    glClearTexImage(gBufferColorTexture, 0, GL_RGB, GL_FLOAT, gBufferColorClear);

    glUseProgram(gPassProgram);

    glUniformMatrix4fv(0, 1, GL_FALSE, camera.GetProjectionMatrix().data());
    glUniformMatrix4fv(1, 1, GL_FALSE, camera.GetViewMatrix().matrix().data());

    for (auto& meshData : scene.meshes) {
        glBindVertexArray(meshData.VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshData.EBO);
        glDrawElements(GL_TRIANGLES, meshData.count, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //Lighting Pass

    glBindFramebuffer(GL_FRAMEBUFFER, renderTargetFrameBuffer);
    glClearColor(0.2f, 0.2f, 0.25f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(lightingPassVAO);
    glBindTextureUnit(0, gBufferColorTexture);
    glBindTextureUnit(1, gBufferNormalTexture);
    glBindTextureUnit(2, gBufferMetallicRoughnessTexture);
    glBindTextureUnit(3, gBufferDepthBuffer);
    glUseProgram(lightingPassProgram);

    glUniform1f(0, lighting ? 1.0f : 0.0f);
    glUniform1f(1, color ? 1.0f : 0.0f);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}