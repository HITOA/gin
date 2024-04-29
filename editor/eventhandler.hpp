//
// Created by HITO on 28/04/24.
//

#ifndef GIN_EVENTHANDLER_HPP
#define GIN_EVENTHANDLER_HPP

#include <memory>
#include <typeindex>

class EventHandler {
public:
    template<typename T>
    inline void SetEvent(std::shared_ptr<T> event) {
        ptr = event;
        typeIndex = typeid(T);
    }

    template<typename T>
    inline T* GetEvent() {
        if (typeIndex == typeid(T))
            return (T*)ptr.get();
        return nullptr;
    }

    template<typename T>
    static inline EventHandler create(std::shared_ptr<T> event) {
        EventHandler handler{};
        handler.SetEvent(event);
        return handler;
    }

    template<typename T>
    static inline EventHandler create(T event) {
        EventHandler handler{};
        handler.SetEvent(std::make_shared<T>(event));
        return handler;
    }

private:
    std::type_index typeIndex{ typeid(void) };
    std::shared_ptr<void> ptr{};
};

#endif //GIN_EVENTHANDLER_HPP
