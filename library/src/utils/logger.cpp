//
// Created by HITO on 28/04/24.
//

#include <gin/utils/logger.hpp>

thread_local std::string msg{};
static Gin::Utils::Logger* logger{ nullptr };
static std::mutex mutex{};

Gin::Utils::Logger *Gin::Utils::Logger::GetSingleton() {
    std::unique_lock<std::mutex> lock{ mutex };
    if (logger)
        return logger;
    logger = new Gin::Utils::Logger{};
    return logger;
}

void Gin::Utils::Logger::AddLogOutput(std::ostream *output) {
    std::unique_lock<std::mutex> lock{ mutex };
    log.push_back(output);
}

void Gin::Utils::Logger::AddWarnOutput(std::ostream *output) {
    std::unique_lock<std::mutex> lock{ mutex };
    warn.push_back(output);
}

void Gin::Utils::Logger::AddErrOutput(std::ostream *output) {
    std::unique_lock<std::mutex> lock{ mutex };
    err.push_back(output);
}

void Gin::Utils::Logger::RemoveLogOutput(std::ostream *output) {
    std::unique_lock<std::mutex> lock{ mutex };
    for (int i = 0; i < log.size(); ++i) {
        if (log[i] == output) {
            log.erase(log.begin() + i);
            break;
        }
    }
}

void Gin::Utils::Logger::RemoveWarnOutput(std::ostream *output) {
    std::unique_lock<std::mutex> lock{ mutex };
    for (int i = 0; i < warn.size(); ++i) {
        if (warn[i] == output) {
            warn.erase(warn.begin() + i);
            break;
        }
    }
}

void Gin::Utils::Logger::RemoveErrOutput(std::ostream *output) {
    std::unique_lock<std::mutex> lock{ mutex };
    for (int i = 0; i < err.size(); ++i) {
        if (err[i] == output) {
            err.erase(err.begin() + i);
            break;
        }
    }
}

void Gin::Utils::Logger::Write(std::string_view str) {
    msg += str;
}

void Gin::Utils::Logger::Commit(Gin::Utils::Logger::Type type) {
    std::unique_lock<std::mutex> lock{ mutex };
    switch (type) {
        case Type::Log:
        {
            for (auto output : log)
                (*output) << msg;
            break;
        }
        case Type::Warn:
        {
            for (auto output : warn)
                (*output) << msg;
            break;
        }
        case Type::Err:
        {
            for (auto output : err)
                (*output) << msg;
            break;
        }
    }

    msg.clear();
}