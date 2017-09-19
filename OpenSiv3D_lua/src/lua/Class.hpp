#pragma once
#include <Siv3D.hpp>
#include <sol.hpp>
#include "IScriptExecutor.hpp"
#include "IScriptSetter.hpp"

namespace s3d::Lua {
  class Class: public IScriptExecutor<Class>, public IScriptSetter<Class> {
    friend class IScriptExecutor<Class>;
    friend class IScriptSetter<Class>;
    friend struct sol::stack::pusher<Class>;

  private:
    sol::table m_core;

    sol::table& getScript();

  public:
    Class(const sol::table& table);
    Class(const Class&) = default;
    Class(Class&&) = default;
    Class& operator=(const Class&) = default;
    Class& operator=(Class&&) = default;


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
      auto func = getRawFunction(functionName);
      if constexpr(std::is_same_v<T, void>) {
        return [func, this](auto&&... args) {func(m_core, args...); };
      }
      else {
        return [func, this](auto&&... args) {return func(m_core, args...); };
      }
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
      auto coro = getRawCoroutine(functionName);
      if constexpr(std::is_same_v<T, void>) {
        return [coro, this](auto&&... args)mutable {coro.call(m_core, args...); };
      }
      else {
        return [coro, this](auto&&... args)mutable {return coro.call(m_core, args...); };
      }
    }
  };
}

namespace sol {
  template<>
  struct lua_size<Lua::Class> : std::integral_constant<int, 1> {};

  template<>
  struct lua_type_of<Lua::Class> : std::integral_constant<sol::type, sol::type::table> {};

  namespace stack {
    template <>
    struct checker<Lua::Class> {
      template <typename Handler>
      static bool check(lua_State* L, int index, Handler&& handler, record& tracking) {
        return Lua::detail::checkStack<sol::table>(L, index, hanlder, tracking, 1);
      }
    };

    template <>
    struct getter<Lua::Class> {
      static Lua::Class get(lua_State* L, int index, record& tracking) {
        auto table = Lua::detail::getStack<sol::table>(L, index, tracking, 1);
        return { table };
      }
    };

    template <>
    struct pusher<Lua::Class> {
      static int push(lua_State* L, const Lua::Class& obj) {
        return Lua::detail::pushStack(L, obj.m_core);
      }
    };
  }
}