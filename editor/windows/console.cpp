//
// Created by HITO on 28/04/24.
//

#include "console.hpp"

std::streamsize ConsoleStream::xsputn(const char *s, std::streamsize n) {
    if (callback)
        return callback(s, n);
    return 0;
}

void ConsoleStream::SetCallback(std::function<std::streamsize(const char *, std::streamsize)> callback) {
    this->callback = callback;
}

ConsoleWindow::~ConsoleWindow() noexcept {
    Gin::Utils::Logger::GetSingleton()->RemoveLogOutput(&l);
    Gin::Utils::Logger::GetSingleton()->RemoveWarnOutput(&w);
    Gin::Utils::Logger::GetSingleton()->RemoveErrOutput(&e);
}

void ConsoleWindow::Initialize() {
    log.SetCallback([=](const char* s, std::streamsize n){
        EntryData entry{};
        entry.str = s;
        entry.type = Gin::Utils::Logger::Type::Log;
        items.push_back(entry);
        return n;
    });

    warn.SetCallback([=](const char* s, std::streamsize n){
        EntryData entry{};
        entry.str = s;
        entry.type = Gin::Utils::Logger::Type::Warn;
        items.push_back(entry);
        return n;
    });

    err.SetCallback([=](const char* s, std::streamsize n){
        EntryData entry{};
        entry.str = s;
        entry.type = Gin::Utils::Logger::Type::Err;
        items.push_back(entry);
        return n;
    });

    Gin::Utils::Logger::GetSingleton()->AddLogOutput(&l);
    Gin::Utils::Logger::GetSingleton()->AddWarnOutput(&w);
    Gin::Utils::Logger::GetSingleton()->AddErrOutput(&e);
}

void ConsoleWindow::Draw(bool *open) {
    static bool autoscroll = true;

    if (ImGui::Begin("Debug Console", open)) {

        ImGui::Checkbox("Auto-scroll", &autoscroll);
        ImGui::SameLine();
        if (ImGui::SmallButton("Clear")) {
            items.clear();
            currentItemsPos = 0;
        }
        ImGui::Separator();

        ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));
        for (int i = 0; i < items.size(); ++i) {
            int currentIndex = (currentItemsPos + i) % items.size();
            switch (items[currentIndex].type) {
                case Gin::Utils::Logger::Type::Err: {
                    ImGui::TextColored(ImVec4(1.00f, 0.00f, 0.00f, 1.00f), items[currentIndex].str.c_str());
                    break;
                }
                case Gin::Utils::Logger::Type::Warn: {
                    ImGui::TextColored(ImVec4(1.00f, 1.00f, 0.00f, 1.00f), items[currentIndex].str.c_str());
                    break;
                }
                case Gin::Utils::Logger::Type::Log: {
                    ImGui::TextColored(ImVec4(1.00f, 1.00f, 1.00f, 1.00f), items[currentIndex].str.c_str());
                    break;
                }
            }
        }

        if ((autoscroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
            ImGui::SetScrollHereY(1.0f);

        ImGui::PopStyleVar();
        ImGui::EndChild();
    }
    ImGui::End();
}

std::string_view ConsoleWindow::GetName() {
    return "Console";
}