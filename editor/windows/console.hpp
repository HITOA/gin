//
// Created by HITO on 28/04/24.
//

#ifndef GIN_CONSOLE_HPP
#define GIN_CONSOLE_HPP

#include "../editor.hpp"
#include <gin/utils/logger.hpp>
#include <sstream>
#include <functional>

class ConsoleStream : public std::stringbuf {
public:
    std::streamsize xsputn(const char* s, std::streamsize n) override;
    void SetCallback(std::function<std::streamsize(const char*, std::streamsize)> callback);

private:
    std::function<std::streamsize(const char*, std::streamsize)> callback{};
};

class ConsoleWindow : public EditorWindow {
private:
    struct EntryData {
        std::string str{};
        Gin::Utils::Logger::Type type{};
    };

public:
    ~ConsoleWindow() noexcept;

    void Initialize() final;
    void Draw(bool *open) final;
    std::string_view GetName() final;

private:
    int currentItemsPos{ 0 };
    std::vector<EntryData> items{};
    ConsoleStream log{};
    ConsoleStream warn{};
    ConsoleStream err{};
    std::ostream l{ &log };
    std::ostream w{ &warn };
    std::ostream e{ &err };
};

#endif //GIN_CONSOLE_HPP
