//
// Created by HITO on 28/04/24.
//

#ifndef GIN_EVENT_HPP
#define GIN_EVENT_HPP

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
    inline std::shared_ptr<T> GetEvent() {
        if (typeIndex == typeid(T))
            return std::dynamic_pointer_cast<T>(ptr);
        return nullptr;
    }

private:
    std::type_index typeIndex{ typeid(void) };
    std::shared_ptr<void> ptr{};
};

#endif //GIN_EVENT_HPP
