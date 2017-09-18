#pragma once
#include <Siv3D.hpp>
#include <sol.hpp>
#include "IScriptExecutor.hpp"

namespace s3d::Lua {
  class Class: public IScriptExecutor<Class> {
    friend class IScriptExecutor<Class>;
  private:
    sol::table m_core;

    sol::table& getScript() {
      return m_core;
    }

    template<typename T, typename ...Arg>
    constexpr std::function<T(Arg...)> addSelfArgumentFunction(std::function<T(Arg...)>* ptr, sol::protected_function func) {
      if constexpr(std::is_same_v<T, void>) {
        return [=](Arg... args) {func(m_core, args...); };
      }
      else {
        return [=](Arg... args) {return func(m_core, args...); };
      }
    }

    template<typename T, typename ...Arg>
    constexpr std::function<T(Arg...)> addSelfArgumentCoroutine(std::function<T(Arg...)>* ptr, Coroutine func) {
      if constexpr(std::is_same_v<T, void>) {
        return [=](Arg... args)mutable {func.call(m_core, args...); };
      }
      else {
        return [=](Arg... args)mutable {return func.call(m_core, args...).get<T>(); };
      }
    }

  public:
    Class(const sol::table& table);
    Class(const Class&) = default;
    Class(Class&&) = default;


    template<typename Result, typename ...Arg>
    Result callFunction(const String& functionName, Arg&&... arg) {
      auto func = getRawFunction(functionName);
      if constexpr(std::is_same_v<Result, void>) {
        func(m_core, Lua::detail::makeHelper(arg)...);
      }
      else {
        return func(m_core, Lua::detail::makeHelper(arg)...);
      }
    }

    template<typename T>
    std::function<T> getFunction(const String& functionName) {
      return addSelfArgumentFunction(static_cast<std::function<T>*>(nullptr), getRawFunction(functionName));
    }

    template<typename Ret, typename ...Arg>
    Ret callCoroutine(const String& functionName, const Arg&... arg) {
      auto coro = getRawCoroutine(functionName);
      if constexpr(std::is_same_v<Ret, void>) {
        coro.call(m_core, arg...);
      }
      else {
        return coro.call(m_core, arg...).get<Ret>();
      }
    }

    template<typename T>
    std::function<T> getCoroutine(const String& functionName) {
      return addSelfArgumentCoroutine(static_cast<std::function<T>*>(nullptr), getRawCoroutine(functionName));
    }
  };
}