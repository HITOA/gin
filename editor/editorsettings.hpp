//
// Created by HITO on 29/04/24.
//

#ifndef GIN_EDITORSETTINGS_HPP
#define GIN_EDITORSETTINGS_HPP

#include <imgui.h>
#include <unordered_map>
#include <string>

struct EditorShortcutSetting {
    ImGuiKey mod1{ ImGuiKey_None };
    ImGuiKey mod2{ ImGuiKey_None };
    ImGuiKey key{ ImGuiKey_None };

    std::string GetShortcutName();
    ImGuiKeyChord GetKeyChord();
};

enum class EditorShortcut {
    New,
    Open,
    Save,
    SaveAs,
    MAX
};

class EditorSettings {
public:
    EditorShortcutSetting GetShortcutSetting(EditorShortcut shortcut);
    void SetShortcutSetting(EditorShortcut shortcut, EditorShortcutSetting setting);

private:
    EditorShortcutSetting shortcutSettings[(int)EditorShortcut::MAX]{
        EditorShortcutSetting{ ImGuiMod_Ctrl, ImGuiKey_None, ImGuiKey_N },
        EditorShortcutSetting{ ImGuiMod_Ctrl, ImGuiKey_None, ImGuiKey_O },
        EditorShortcutSetting{ ImGuiMod_Ctrl, ImGuiKey_None, ImGuiKey_S },
        EditorShortcutSetting{ ImGuiMod_Ctrl, ImGuiMod_Shift, ImGuiKey_S }
    };
};

#endif //GIN_EDITORSETTINGS_HPP
