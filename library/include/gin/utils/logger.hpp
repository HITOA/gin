#ifndef GIN_LOGGER_HPP
#define GIN_LOGGER_HPP

#include <ostream>
#include <vector>
#include <mutex>
#include <gin/utils/formatter.hpp>

namespace Gin::Utils {

    class Logger {
    public:
        enum class Type {
            Log,
            Err,
            Warn
        };
    public:
        static Logger* GetSingleton();

        void AddLogOutput(std::ostream* output);
        void AddWarnOutput(std::ostream* output);
        void AddErrOutput(std::ostream* output);

        void RemoveLogOutput(std::ostream* output);
        void RemoveWarnOutput(std::ostream* output);
        void RemoveErrOutput(std::ostream* output);

        void Write(std::string_view str);
        void Commit(Type type);

        template<typename Arg>
        static inline void Print(Arg arg) {
            GetSingleton()->Write(Format<Arg>(arg));
        }

        template<typename Arg, typename... Args>
        static inline void Print(Arg arg, Args... args) {
            GetSingleton()->Write(Format<Arg>(arg));
            Print(args...);
        }

        template<typename... Args>
        static inline void Log(Args... args) {
            time_t t = time(nullptr);
            tm* tm = localtime(&t);
            Print("[Log][", tm->tm_hour, ":", tm->tm_min, ":", tm->tm_sec, "] : ", args..., "\n");
            GetSingleton()->Commit(Type::Log);
        }

        template<typename... Args>
        static inline void Warn(Args... args) {
            time_t t = time(nullptr);
            tm* tm = localtime(&t);
            Print("[Warning][", tm->tm_hour, ":", tm->tm_min, ":", tm->tm_sec, "] : ", args..., "\n");
            GetSingleton()->Commit(Type::Warn);
        }

        template<typename... Args>
        static inline void Err(Args... args) {
            time_t t = time(nullptr);
            tm* tm = localtime(&t);
            Print("[ERROR][", tm->tm_hour, ":", tm->tm_min, ":", tm->tm_sec, "] : ", args..., "\n");
            GetSingleton()->Commit(Type::Err);
        }

    private:
        std::vector<std::ostream*> log{};
        std::vector<std::ostream*> warn{};
        std::vector<std::ostream*> err{};
    };

}

#endif //GIN_LOGGER_HPP
