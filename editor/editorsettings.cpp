//
// Created by HITO on 29/04/24.
//

#include "editorsettings.hpp"

std::string EditorShortcutSetting::GetShortcutName() {
    if (!mod1 || !key)
        return "None";

    std::string name{};
    name += &ImGui::GetKeyName(mod1)[3];
    name += "+";
    if (mod2) {
        name += &ImGui::GetKeyName(mod2)[3];
        name += "+";
    }
    name += ImGui::GetKeyName(key);
    return name;
}

ImGuiKeyChord EditorShortcutSetting::GetKeyChord() {
    if (!mod1 || !key)
        return 0;
    ImGuiKeyChord kc = mod1 | key;
    if (mod2)
        kc |= mod2;
    return kc;
}

EditorShortcutSetting EditorSettings::GetShortcutSetting(EditorShortcut shortcut) {
    if (shortcut >= EditorShortcut::MAX)
        return EditorShortcutSetting{};
    return shortcutSettings[(int)shortcut];
}

void EditorSettings::SetShortcutSetting(EditorShortcut shortcut, EditorShortcutSetting setting) {
    if (shortcut >= EditorShortcut::MAX)
        return;
    shortcutSettings[(int)shortcut] = setting;
}