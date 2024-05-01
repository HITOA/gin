#include "editor.hpp"
#include <stdexcept>

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#include <gin/utils/logger.hpp>
#include <iostream>

#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "editorevent.hpp"

void SetupImGuiStyle()
{
    // Rounded Visual Studio style by RedNicStone from ImThemes
    ImGuiStyle& style = ImGui::GetStyle();

    style.Alpha = 1.0f;
    style.DisabledAlpha = 0.6000000238418579f;
    style.WindowPadding = ImVec2(8.0f, 8.0f);
    style.WindowRounding = 4.0f;
    style.WindowBorderSize = 0.0f;
    style.WindowMinSize = ImVec2(32.0f, 32.0f);
    style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
    style.WindowMenuButtonPosition = ImGuiDir_Left;
    style.ChildRounding = 0.0f;
    style.ChildBorderSize = 1.0f;
    style.PopupRounding = 4.0f;
    style.PopupBorderSize = 1.0f;
    style.FramePadding = ImVec2(4.0f, 3.0f);
    style.FrameRounding = 2.5f;
    style.FrameBorderSize = 0.0f;
    style.ItemSpacing = ImVec2(8.0f, 4.0f);
    style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
    style.CellPadding = ImVec2(4.0f, 2.0f);
    style.IndentSpacing = 21.0f;
    style.ColumnsMinSpacing = 6.0f;
    style.ScrollbarSize = 11.0f;
    style.ScrollbarRounding = 2.5f;
    style.GrabMinSize = 10.0f;
    style.GrabRounding = 2.0f;
    style.TabRounding = 3.5f;
    style.TabBorderSize = 0.0f;
    style.TabMinWidthForCloseButton = 0.0f;
    style.ColorButtonPosition = ImGuiDir_Right;
    style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
    style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

    style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.5921568870544434f, 0.5921568870544434f, 0.5921568870544434f, 1.0f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.3058823645114899f, 0.3058823645114899f, 0.3058823645114899f, 1.0f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.3058823645114899f, 0.3058823645114899f, 0.3058823645114899f, 1.0f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2156862765550613f, 1.0f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2156862765550613f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2156862765550613f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.321568638086319f, 0.321568638086319f, 0.3333333432674408f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.3529411852359772f, 0.3529411852359772f, 0.3725490272045135f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.3529411852359772f, 0.3529411852359772f, 0.3725490272045135f, 1.0f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2156862765550613f, 1.0f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2156862765550613f, 1.0f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
    style.Colors[ImGuiCol_Separator] = ImVec4(0.3058823645114899f, 0.3058823645114899f, 0.3058823645114899f, 1.0f);
    style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.3058823645114899f, 0.3058823645114899f, 0.3058823645114899f, 1.0f);
    style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.3058823645114899f, 0.3058823645114899f, 0.3058823645114899f, 1.0f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2156862765550613f, 1.0f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.321568638086319f, 0.321568638086319f, 0.3333333432674408f, 1.0f);
    style.Colors[ImGuiCol_Tab] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_TabHovered] = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
    style.Colors[ImGuiCol_TabActive] = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
    style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
    style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.2000000029802322f, 1.0f);
    style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3490196168422699f, 1.0f);
    style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2274509817361832f, 0.2274509817361832f, 0.2470588237047195f, 1.0f);
    style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.05999999865889549f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
    style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
    style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
    style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
    style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
}

void ErrorCallback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void* GinImguiSettingsReadOpen(ImGuiContext*, ImGuiSettingsHandler* handler, const char* name) {
    Editor* editor = (Editor*)handler->UserData;

    for (size_t i = 0; i < editor->editorWindows.size(); ++i) {
        if (strcmp(name, editor->editorWindows[i].window->GetName().data()) == 0) {
            return &editor->editorWindows[i];
        }
    }

    return nullptr;
}

void GinImguiSettingsReadLine(ImGuiContext*, ImGuiSettingsHandler*, void* entry, const char* line) {
    Editor::EditorWindowEntry* windowEntry = (Editor::EditorWindowEntry*)entry;

    int i;
    if (sscanf(line, "Open=%i", &i) == 1)
        windowEntry->open = i;
}

void GinImguiSettingsWriteAll(ImGuiContext*, ImGuiSettingsHandler* handler, ImGuiTextBuffer* buf)
{
    Editor* editor = (Editor*)handler->UserData;

    for (auto& entry : editor->editorWindows) {
        buf->appendf("[%s][%s]\n", handler->TypeName, entry.window->GetName().data());
        buf->appendf("Open=%i\n", entry.open ? 1 : 0);
        buf->append("\n");
    }
}

void Editor::Initialize() {
    Gin::Utils::Logger::GetSingleton()->AddLogOutput(&std::cout);
    Gin::Utils::Logger::GetSingleton()->AddWarnOutput(&std::cout);
    Gin::Utils::Logger::GetSingleton()->AddErrOutput(&std::cerr);

    glfwInit();

    glfwSetErrorCallback(ErrorCallback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(800, 600, "GIN Editor", nullptr, nullptr);
    if (window == nullptr) {
        glfwTerminate();
        throw std::runtime_error{"Window couldn't be created."};
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

    if (!gladLoadGL(glfwGetProcAddress)) {
        glfwTerminate();
        throw std::runtime_error{"Couldn't load GL functions."};
    }
    glViewport(0, 0, 800, 600);

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();

    ImGuiSettingsHandler editorSettingsHandler{};
    editorSettingsHandler.TypeName = "Gin";
    editorSettingsHandler.TypeHash = ImHashStr(editorSettingsHandler.TypeName);
    editorSettingsHandler.ReadOpenFn = GinImguiSettingsReadOpen;
    editorSettingsHandler.ReadLineFn = GinImguiSettingsReadLine;
    editorSettingsHandler.WriteAllFn = GinImguiSettingsWriteAll;
    editorSettingsHandler.UserData = this;

    ImGui::AddSettingsHandler(&editorSettingsHandler);

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    io.Fonts->AddFontDefault();

    static ImWchar ranges[] = { 0xf000, 0xf3ff, 0 };
    ImFontConfig fontConfig{};
    fontConfig.MergeMode = true;
    io.Fonts->AddFontFromFileTTF("data/fonts/fw6s.otf", 13, &fontConfig, ranges);
    io.Fonts->AddFontFromFileTTF("data/fonts/fw6r.otf", 13, &fontConfig, ranges);
    io.Fonts->Build();

    SetupImGuiStyle();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");

    for (auto& entry : editorWindows)
        entry.window->Initialize();
}

void Editor::Uninitialize() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}

void Editor::Run() {
    while(!glfwWindowShouldClose(window))
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        GlobalShortcut();

        DrawDockSpace();
        DrawMainMenuBar();
        DrawBottomBar();

        for (auto& entry : editorWindows) {
            if (entry.open)
                entry.window->Draw(&entry.open);
        }

        ImGui::Render();

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Editor::SendEvent(EventHandler handler) {
    for (auto& entry : editorWindows) {
        if (entry.open)
            entry.window->OnEvent(handler);
    }
}

EditorSettings &Editor::GetEditorSettings() {
    return settings;
}

void Editor::DrawDockSpace() {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("Main DockSpace", nullptr,
                 ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
                 ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground);

    ImGui::PopStyleVar(3);

    ImGui::DockSpace(ImGui::GetID("Main DockSpace"), ImVec2{ 0, 0 }, ImGuiDockNodeFlags_PassthruCentralNode);

    ImGui::End();
}

void Editor::DrawMainMenuBar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {

            if (ImGui::MenuItemEx("New", "\uF07B", settings.GetShortcutSetting(EditorShortcut::New).GetShortcutName().c_str())) {
                SendEvent(EventHandler::create(EditorEvent::New));
            }
            if (ImGui::MenuItemEx("Open", "\uF07C", settings.GetShortcutSetting(EditorShortcut::Open).GetShortcutName().c_str())) {
                SendEvent(EventHandler::create(EditorEvent::Open));
            }

            ImGui::Separator();

            if (ImGui::MenuItemEx("Save", "", settings.GetShortcutSetting(EditorShortcut::Save).GetShortcutName().c_str())) {
                SendEvent(EventHandler::create(EditorEvent::Save));
            }
            if (ImGui::MenuItemEx("Save As", "\uF0C7", settings.GetShortcutSetting(EditorShortcut::SaveAs).GetShortcutName().c_str())) {
                SendEvent(EventHandler::create(EditorEvent::SaveAs));
            }

            ImGui::Separator();

            if (ImGui::MenuItemEx("Quit", "")) {
                glfwSetWindowShouldClose(window, true);
                SendEvent(EventHandler::create(EditorEvent::Quit));
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Window")) {

            for (auto& entry : editorWindows) {
                if (ImGui::MenuItem(entry.window->GetName().data())) {
                    entry.open = true;
                }
            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void Editor::DrawBottomBar() {
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
    if (ImGui::BeginViewportSideBar("BottomBar", ImGui::GetMainViewport(), ImGuiDir_Down, ImGui::GetFrameHeight(), window_flags)) {
        if (ImGui::BeginMenuBar()) {

            ImColor fpsColor = ImColor{0.3f, 1.0f, 0.3f};

            if (ImGui::GetIO().Framerate <= 30)
                fpsColor = ImColor{ 1.0f, 0.3f, 0.3f };
            else if (ImGui::GetIO().Framerate <= 45)
                fpsColor = ImColor{ 1.0f, 1.0f, 0.3f };

            ImGui::TextColored(fpsColor, "FrameRate : %f", ImGui::GetIO().Framerate);

            ImGui::EndMenuBar();
        }
        ImGui::End();
    }
}

void Editor::GlobalShortcut() {
    if (ImGui::Shortcut(settings.GetShortcutSetting(EditorShortcut::New).GetKeyChord(),
                        ImGuiKeyOwner_Any, ImGuiInputFlags_RouteGlobal))
        SendEvent(EventHandler::create(EditorEvent::New));

    if (ImGui::Shortcut(settings.GetShortcutSetting(EditorShortcut::Open).GetKeyChord(),
                        ImGuiKeyOwner_Any, ImGuiInputFlags_RouteGlobal))
        SendEvent(EventHandler::create(EditorEvent::Open));

    if (ImGui::Shortcut(settings.GetShortcutSetting(EditorShortcut::Save).GetKeyChord(),
                        ImGuiKeyOwner_Any, ImGuiInputFlags_RouteGlobal))
        SendEvent(EventHandler::create(EditorEvent::Save));

    if (ImGui::Shortcut(settings.GetShortcutSetting(EditorShortcut::SaveAs).GetKeyChord(),
                        ImGuiKeyOwner_Any, ImGuiInputFlags_RouteGlobal))
        SendEvent(EventHandler::create(EditorEvent::SaveAs));
}