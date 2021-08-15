#pragma once
#include <Siv3D.hpp>
#include <sol/sol.hpp>
#include "IScriptExecutor.hpp"
#include "IScriptSetter.hpp"

namespace s3d::Lua {
  class Class : public IScriptExecutor<Class>, public IScriptSetter<Class> {
    friend class IScriptExecutor<Class>;
    friend class IScriptSetter<Class>;
    //friend struct sol::stack::pusher<Class>;

  private:
    sol::table m_core;

    sol::table& getScript();
    const sol::table& getScript()const;

  public:
    Class(const sol::table& table);
    Class(const Class&) = default;
    Class(Class&&) = default;
    Class& operator=(const Class&) = default;
    Class& operator=(Class&&) = default;

    template<typename T>
    Class operator+(const T& obj) const {
      return { callMetaFunction<sol::table>(sol::meta_function::addition, obj) };
    }

    template<typename T>
    Class operator-(const T& obj) const {
      return { callMetaFunction<sol::table>(sol::meta_function::subtraction, obj) };
    }

    Class operator-() const {
      return { callMetaFunction<sol::table>(sol::meta_function::unary_minus) };
    }

    template<typename T>
    Class operator*(const T& obj) const {
      return { callMetaFunction<sol::table>(sol::meta_function::multiplication, obj) };
    }

    template<typename T>
    Class operator/(const T& obj) const {
      return { callMetaFunction<sol::table>(sol::meta_function::division, obj) };
    }

    template<typename T>
    Class operator%(const T& obj) const {
      return { callMetaFunction<sol::table>(sol::meta_function::modulus, obj) };
    }

    bool operator==(const Class& obj)const {
      return callMetaFunction<bool>(sol::meta_function::equal_to, obj);
    }

    bool operator!=(const Class& obj)const {
      return !(*this == obj);
    }

    template<typename T>
    bool operator<(const T& obj)const {
      return callMetaFunction<bool>(sol::meta_function::less_than, obj);
    }

    template<typename T>
    bool operator>(const T& obj)const {
      return obj < *this;
    }

    template<typename T>
    bool operator<=(const T& obj)const {
      return callMetaFunction<bool>(sol::meta_function::less_than_or_equal_to, obj);
    }

    template<typename T>
    bool operator>=(const T& obj)const {
      return !(*this < obj);
    }

    template<typename Result, typename ...Arg>
    Result callFunction(const String& functionName, Arg&&... arg) const {
      auto func = getRawFunction(functionName);
      if constexpr(std::is_same_v<Result, void>) {
        func(m_core, Lua::detail::makeHelper(arg)...);
      }
      else {
        return func(m_core, Lua::detail::makeHelper(arg)...);
      }
    }

    template<typename T>
    std::function<T> getFunction(const String& functionName) const {
      auto func = getRawFunction(functionName);
      if constexpr(std::is_same_v<T, void>) {
        return [func, this](auto&&... args) {func(m_core, args...); };
      }
      else {
        return [func, this](auto&&... args) {return func(m_core, args...); };
      }
    }

    //template<typename Ret, typename ...Arg>
    //Ret callCoroutine(const String& functionName, const Arg&... arg) {
    //  auto coro = getRawCoroutine(functionName);
    //  if constexpr(std::is_same_v<Ret, void>) {
    //    coro.call(m_core, arg...);
    //  }
    //  else {
    //    return coro.call(m_core, arg...).get<Ret>();
    //  }
    //}

    //template<typename T>
    //std::function<T> getCoroutine(const String& functionName) {
    //  auto coro = getRawCoroutine(functionName);
    //  if constexpr(std::is_same_v<T, void>) {
    //    return [coro, this](auto&&... args)mutable {coro.call(m_core, args...); };
    //  }
    //  else {
    //    return [coro, this](auto&&... args)mutable {return coro.call(m_core, args...); };
    //  }
    //}

    template<typename Ret, typename ...Arg>
    Ret callMetaFunction(sol::meta_function funcEnum, const Arg&... arg) const {
      auto func = getRawFunction(funcEnum);
      if constexpr(std::is_same_v<Ret, void>) {
        func.call(m_core, arg...);
      }
      else {
        return func.call(m_core, arg...).get<Ret>();
      }
    }

    String toString() const {
      return callMetaFunction<String>(sol::meta_function::to_string);
    }

    template<typename T>
    Class pow(const T& obj) const {
      return { callMetaFunction<sol::table>(sol::meta_function::power_of, obj) };
    }

    template<typename Ret = String, typename T>
    Ret concat(const T& obj) const {
      return { callMetaFunction<Ret>(sol::meta_function::concatenation, obj) };
    }

    double length() const {
      return callMetaFunction<double>(sol::meta_function::length);
    }

    template<typename Ret, typename ...Arg>
    Ret call(const Arg&... args)const {
      return { callMetaFunction<Ret>(sol::meta_function::call_function, args...) };
    }
  };
}

namespace sol {
  template<>
  struct lua_size<Lua::Class> : std::integral_constant<int, 1> {};

  template<>
  struct lua_type_of<Lua::Class> : std::integral_constant<sol::type, sol::type::table> {};

 /* namespace stack {
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
  }*/
}