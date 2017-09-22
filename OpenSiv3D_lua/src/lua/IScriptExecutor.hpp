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

      const Self& selfCast() const {
        return *static_cast<const Self*>(this);
      }

      decltype(auto) getSolScript() {
        return selfCast().getScript();
      }

      decltype(auto) getSolScript() const {
        return selfCast().getScript();
      }

    protected:
      sol::protected_function getRawFunction(const String& functionName) const {
        return getSolScript()[functionName.narrow()].get<sol::protected_function>();
      }

      sol::protected_function getRawFunction(sol::meta_function func) const {
        return getSolScript()[func].get<sol::protected_function>();
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

      decltype(auto) operator[](const String& name) {
        return getSolScript()[name.narrow()];
      }

      template<typename T>
      T getValue(const String& valueName) const{
        return getSolScript()[valueName.narrow()].get<T>();
      }

      template<typename Result, typename ...Arg>
      Result callFunction(const String& functionName, Arg&&... arg) const{
        auto func = getRawFunction(functionName);
        if constexpr(std::is_same_v<T, void>) {
          func(Lua::detail::makeHelper(arg)...);
        }
        else {
          return func(Lua::detail::makeHelper(arg)...).get<Result>();
        }
      }

      template<typename T>
      std::function<T> getFunction(const String& functionName) const{
        return static_cast<std::function<T>>(getRawFunction(functionName));
      }

      template<typename T>
      std::function<T> getCoroutine(const String& functionName) {
        auto coro = getRawCoroutine(functionName);
        if constexpr(std::is_same_v<T, void>) {
          return [coro](auto&&... args) {coro.call(args...); };
        }
        else {
          return [coro](auto&&... args) {return coro.call(args...).get<T>(); };
        }
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
      Class createClass(const String& className, const Arg&... arg) const{
        return { getSolScript()[className.narrow()]["new"](arg...).get<sol::table>()};
      }
    };
  }
}