#pragma once
#include <Siv3D.hpp>
#include <sol.hpp>
#include "Coroutine.hpp"

namespace s3d {
  namespace Lua {
    class Class;

    template<typename Self>
    class IScriptExecutor {
    private:
      HashMap<String, Coroutine> m_coroutine;

      Self& selfCast() {
        return *static_cast<Self*>(this);
      }

      decltype(auto) getSolScript() {
        return selfCast().getScript();
      }

      template<typename T, typename ...Arg>
      constexpr std::function<T(Arg...)> bindCoroutine(std::function<T(Arg...)>* ptr, Coroutine coro) {
        if constexpr(std::is_same_v<T, void>) {
          return [=](Arg... args) {coro.call(args...); };
        }
        else {
          return [=](Arg... args) {return coro.call(args...).get<T>(); };
        }
      }

    protected:
      sol::protected_function getRawFunction(const String& functionName) {
        return getSolScript()[functionName.narrow()].get<sol::protected_function>();
      }

      Coroutine getRawCoroutine(const String& functionName) {
        auto it = m_coroutine.find(functionName);
        if (it != m_coroutine.end() && !it->second) {
          m_coroutine.erase(it);
          it = m_coroutine.insert(std::make_pair(functionName, Coroutine(getSolScript()[functionName.narrow()].get<sol::protected_function>()))).first;
        }
        if (it == m_coroutine.end()) {
          it = m_coroutine.insert(std::make_pair(functionName, Coroutine(getSolScript()[functionName.narrow()].get<sol::protected_function>()))).first;
        }
        return it->second;
      }

    public:
      explicit operator bool()const {
        return getSolScript().valid();
      }

      template<typename T>
      T getValue(const String& valueName) {
        return getSolScript()[valueName.narrow()].get<T>();
      }

      template<typename Result, typename ...Arg>
      Result callFunction(const String& functionName, Arg&&... arg) {
        auto func = getRawFunction(functionName);
        if constexpr(std::is_same_v<T, void>) {
          func(Lua::detail::makeHelper(arg)...);
        }
        else {
          return func(Lua::detail::makeHelper(arg)...).get<Result>();
        }
      }

      template<typename T>
      std::function<T> getFunction(const String& functionName) {
        return static_cast<std::function<T>>(getRawFunction(functionName));
      }

      template<typename T>
      std::function<T> getCoroutine(const String& functionName) {
        return bindCoroutine(static_cast<std::function<T>*>(nullptr), getRawCoroutine(functionName));
      }

      template<typename Ret, typename ...Arg>
      Ret callCoroutine(const String& functionName, const Arg&... arg) {
        auto coro = getRawCoroutine(functionName);
        if constexpr(std::is_same_v<T, void>) {
          coro.call(arg...);
        }
        else {
          return coro.call(arg...).get<Ret>();
        }
      }

      template<typename ...Arg>
      Class createClass(const String& className, const Arg&... arg) {
        return { getSolScript()[className.narrow()]["new"](arg...).get<sol::table>()};
      }
    };
  }
}