#pragma once
#include <Siv3D.hpp>
#include <sol.hpp>
#include "ClassSetter.hpp"

namespace s3d::Lua {
  class NamespaceSetter;

  template<typename Self>
  class IScriptSetter {
  private:
    Self& selfCast() {
      return *static_cast<Self*>(this);
    }

    decltype(auto) getSolScript() {
      return selfCast().getScript();
    }

  public:
    template<typename T, typename std::enable_if_t<std::is_convertible_v<T, String>, std::nullptr_t> = nullptr>
    NamespaceSetter setNamespace(const T& namespaceName) {
      auto path = String(namespaceName).split(L'.');
      sol::table table = getSolScript();
      for (auto&& name : path) {
        table = table.create_named(name.narrow());
      }
      return NamespaceSetter(table);
    }

    template<typename T>
    Self& setClass(const String& className, const ClassSetter<T>& classRegister) {
      auto usertype = getSolScript().create_simple_usertype<T>();
      classRegister.applyCommand(std::ref(usertype));
      getSolScript().set_usertype(className.narrow(), usertype);
      return selfCast();
    }

    template<typename T>
    ClassSetterWithRAII<T> setClass(const String& className) {
      return { className, getSolScript().create_simple_usertype<T>(), std::bind(&std::remove_reference_t<decltype(getSolScript())>::set_usertype<std::string, T>, std::ref(getSolScript()), std::placeholders::_1, std::placeholders::_2) };
    }

    template<typename F>
    Self& setFunction(const String& functionName, F&& function) {
      getSolScript().set_function(functionName.narrow(), function);
      return selfCast();
    }

    template<typename T>
    Self& setValue(const String& valueName, const T& value) {
      getSolScript()[valueName.narrow()].set(Lua::detail::makeHelper(value));
      return selfCast();
    }

  };
}