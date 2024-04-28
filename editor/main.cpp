#include <module.hpp>
#include <nfd.h>
#include "editor.hpp"

#include "windows/grapheditor.hpp"
#include "windows/view.hpp"
#include "windows/profiler.hpp"
#include "windows/console.hpp"

int main() {
    Initialize();
    NFD_Init();

    Editor editor{};

    editor.AddEditorWindow(std::make_shared<GraphEditorWindow>());
    editor.AddEditorWindow(std::make_shared<ViewWindow>());
    editor.AddEditorWindow(std::make_shared<ProfilerWindow>());
    editor.AddEditorWindow(std::make_shared<ConsoleWindow>());

    editor.Initialize();
    editor.Run();
    editor.Uninitialize();

    NFD_Quit();
    Uninitialize();
}