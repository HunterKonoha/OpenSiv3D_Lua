#pragma once
#include <Siv3D.hpp>
#include <sol.hpp>
#include "NamespaceRegister.hpp"
#include "ClassRegister.hpp"

namespace s3d::Lua {
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
    NamespaceRegister setNamespace(const String& namespaceName) {
      auto arr = namespaceName.split(L'.');
      NamespaceRegister regist;
      if constexpr(std::is_same_v<std::remove_reference_t<decltype(getSolScript())>, sol::state>) {
        regist = { getSolScript().create_named_table(arr.front().narrow()) };
      }
      else {
        regist = { getSolScript().create_named(arr.front().narrow()) };
      }
      for (auto&& i : step(1, arr.size() - 1)) {
        regist = regist.setNamespace(arr[i]);
      }
      return regist;
    }

    template<typename T>
    Self& setClass(const String& className, const ClassRegister<T>& classRegister) {
      auto usertype = getSolScript().create_simple_usertype<T>();
      classRegister.applyCommand(std::ref(usertype));
      getSolScript().set_usertype(className.narrow(), usertype);
      return selfCast();
    }

    template<typename T>
    ClassRegisterWithRAII<T> setClass(const String& className) {
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