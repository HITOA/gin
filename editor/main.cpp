#include <module.hpp>
#include <nfd.h>
#include "editor.h"

#include "windows/grapheditor.h"
#include "windows/view.h"

int main() {
    Initialize();
    NFD_Init();

    Editor editor{};

    editor.AddEditorWindow(std::make_shared<GraphEditorWindow>());
    editor.AddEditorWindow(std::make_shared<ViewWindow>());

    editor.Initialize();
    editor.Run();
    editor.Uninitialize();

    NFD_Quit();
    Uninitialize();
}