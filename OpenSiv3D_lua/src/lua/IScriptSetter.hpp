#pragma once
#include <Siv3D.hpp>
#include <sol/sol.hpp>
#include "ClassSetter.hpp"
//#include "NamespaceSetter.hpp"
#include "RegisterOption.hpp"
#include "StackHelper.hpp"

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
    //template<typename T, typename std::enable_if_t<std::is_convertible_v<T, String>, std::nullptr_t> = nullptr>
    //NamespaceSetter setNamespace(const T& namespaceName) {
    //  auto path = String(namespaceName).split(L'.');
    //  sol::table table;
    //  if constexpr(std::is_same_v<std::decay_t<decltype(getSolScript())>, sol::state>) {
    //    table = getSolScript().create_named_table(path[0].narrow());
    //  }
    //  else {
    //    table = getSolScript().create_named(path[0].narrow());
    //  }
    //  for (auto&& i : step(1, path.size() - 1)) {
    //    table = table.create_named(path[i].narrow());
    //  }
    //  return NamespaceSetter(table);
    //}

    template<typename T>
    Self& setClass(const String& className, const ClassSetter<T>& classRegister) {
      auto usertype = getSolScript().new_usertype<T>(className.narrow());
      classRegister.applyCommand(usertype);
      //getSolScript().set_usertype(usertype);
      return selfCast();
    }

    template<typename T>
    ClassSetterWithRAII<T> setClass(const String& className) {
      //return { className, getSolScript().new_usertype<T>(className.narrow()), std::bind(&std::remove_reference_t<decltype(getSolScript())>::template set_usertype<std::string, T>, std::ref(getSolScript()), std::placeholders::_1, std::placeholders::_2) };
      return { className, getSolScript().new_usertype<T>(className.narrow())};
    }

    template<typename F>
    Self& setFunction(const String& functionName, F&& function) {
      getSolScript().set_function(functionName.narrow(), function);
      return selfCast();
    }

    template<typename T>
    Self& setValue(const String& valueName, const T& value, RegisterOption option = RegisterOption::Any) {
      switch (option) {
        default:
        case s3d::Lua::RegisterOption::Any:
          getSolScript()[valueName.narrow()].set(Lua::detail::makeHelper(value));
          break;

        case s3d::Lua::RegisterOption::ReadOnly:
          getSolScript()[valueName.narrow()].set(sol::readonly_property(Lua::detail::makeHelper(value)));
          break;

        case s3d::Lua::RegisterOption::WriteOnly:
          getSolScript()[valueName.narrow()].set(sol::writeonly_property(Lua::detail::makeHelper(value)));
          break;
      }
      return selfCast();
    }

  };
}

