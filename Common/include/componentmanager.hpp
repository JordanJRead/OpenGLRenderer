#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H
#include "boost/pfr/pfr.hpp"
#include <Windows.h>
#include <cstddef>
#include <functional>
#include <iterator>
#include <map>
#include <memory>
#include <nlohmann/json.hpp>
#include <optional>
#include <source_location>
#include <string>

template <typename T>
consteval std::string_view type_name() {
    // Capture the signature of this exact template function instantiation
    const auto&      loc  = std::source_location::current();
    std::string_view name = loc.function_name();

    // Parse out the type T from the compiler-specific signature string
#if defined(__clang__)
    // Clang format: "auto type_name() [T = YourType]"
    size_t prefix = name.find("T = ") + 4;
    size_t suffix = name.rfind("]");
    return name.substr(prefix, suffix - prefix);
#elif defined(__GNUC__)
    // GCC format: "consteval auto type_name() [with T = YourType]"
    size_t prefix = name.find("with T = ") + 9;
    size_t suffix = name.rfind("]");
    return name.substr(prefix, suffix - prefix);
#elif defined(_MSC_VER)
    // MSVC format: "auto __cdecl type_name<YourType>(void)"
    size_t prefix = name.find("type_name<") + 10;
    size_t suffix = name.rfind(">");
    return name.substr(prefix, suffix - prefix);
#else
#error Unsupported compiler!
#endif
}

class Component;

class ComponentManager {
public:
    class Iterator;

    static ComponentManager& instance();
    std::unique_ptr<Component>
    createComponentFromName(std::string_view  componentTypeName,
                            const JSON* const json = nullptr);

    template <typename ComponentType>
    requires std::is_base_of_v<Component, ComponentType>
    std::unique_ptr<Component> createComponent(const JSON* const json) {
        createComponentFromName(type_name<ComponentType>(), json);
    }

    Iterator begin();
    Iterator end();

    void loadScripts();

private:
    ComponentManager();

    std::map<std::string,
             std::function<std::unique_ptr<Component>(const JSON& json)>>
      mStaticComponentFactories;
    std::map<std::string,
             std::function<std::unique_ptr<Component>(const JSON& json)>>
            mDynamicComponentFactories;
    HMODULE mLibraryHandle{ nullptr };

public:
    struct Iterator {
        friend class ComponentManager;

        using iterator_category = std::forward_iterator_tag;
        using value_type        = std::string;
        using difference_type   = std::ptrdiff_t;
        using pointer           = const std::string*;
        using reference         = const std::string&;

        reference operator*() const;
        Iterator& operator++();
        Iterator  operator++(int);
        bool      operator==(const Iterator& other) const;
        bool      operator!=(const Iterator& other) const;

    private:
        Iterator(ComponentManager*                                owner,
                 std::map<std::string, std::function<std::unique_ptr<Component>(
                                         const JSON&)>>::iterator iterator,
                 bool                                             inStatic);

        ComponentManager*                                mOwner;
        bool                                             mInStaticNames{ true };
        std::map<std::string, std::function<std::unique_ptr<Component>(
                                const JSON&)>>::iterator mMapIterator;
    };
};

#endif