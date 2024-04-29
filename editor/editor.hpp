#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>
#include <string_view>
#include <imgui.h>
#include <imgui_internal.h>
#include <typeindex>
#include "eventhandler.hpp"

class Editor;

class EditorWindow {
public:
    virtual ~EditorWindow() = default;
    virtual void Initialize() {};
    virtual void Draw(bool* open) = 0;
    virtual std::string_view GetName() = 0;
    virtual void OnEvent(EventHandler& handler) {};

    void SetEditor(Editor* e) { editor = e; };

protected:
    Editor* editor{ nullptr };
};

class Editor {
public:
    void Initialize();
    void Uninitialize();

    void Run();

    template<typename T>
    void AddEditorWindow(std::shared_ptr<T> window) {
        window->SetEditor(this);
        editorWindows.emplace_back(EditorWindowEntry{ window, std::type_index{ typeid(T) }, false });
    }
    template<typename T>
    std::shared_ptr<T> GetEditorWindow() {
        for (int i = 0; i < editorWindows.size(); ++i) {
            if (editorWindows[i].typeIndex == typeid(T))
                return std::dynamic_pointer_cast<T>(editorWindows[i].window);
        }
        return nullptr;
    }

    void SendEvent(EventHandler handler);

private:
    void DrawDockSpace();
    void DrawMainMenuBar();
    void DrawBottomBar();

private:
    struct EditorWindowEntry {
        std::shared_ptr<EditorWindow> window;
        std::type_index typeIndex;
        bool open;
    };

    GLFWwindow* window{ nullptr };
    std::vector<EditorWindowEntry> editorWindows{};

    friend void* GinImguiSettingsReadOpen(ImGuiContext*, ImGuiSettingsHandler*, const char* name);
    friend void GinImguiSettingsReadLine(ImGuiContext*, ImGuiSettingsHandler*, void* entry, const char* line);
    friend void GinImguiSettingsWriteAll(ImGuiContext* ctx, ImGuiSettingsHandler* handler, ImGuiTextBuffer* buf);
};