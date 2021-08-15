#pragma once
#include <sol/sol.hpp>

namespace s3d::Lua {
  template<typename ...Arg>
  constexpr decltype(auto) makeOverload(Arg&&... arg) {
    return sol::overload(std::forward<Arg>(arg)...);
  }

  template<typename T>
  decltype(auto) solveFunctionPtr(T&& ptr) {
    return sol::resolve(ptr);
  }
  
  template<typename... Args, typename R>
    inline constexpr auto solveFunctionPtr(R fun_ptr(Args...)){
    return sol::resolve(fun_ptr);
  }

  template<typename Sig>
  inline constexpr Sig* solveFunctionPtr(Sig* fun_ptr) {
    return sol::resolve(fun_ptr);
  }

  template<typename... Args, typename R, typename C>
  inline constexpr auto solveFunctionPtr(R(C::*mem_ptr)(Args...))->R(C::*)(Args...) {
    return sol::resolve(mem_ptr);
  }

  template<typename Sig, typename C>
  inline constexpr Sig C::* solveFunctionPtr(Sig C::* mem_ptr) {
    return sol::resolve(mem_ptr);
  }

  template<typename... Sig, typename F, sol::meta::disable<std::is_function<sol::meta::unqualified_t<F>>> = sol::meta::enabler>
  inline constexpr auto solveFunctionPtr(F&& f) {
    return sol::resolve(f);
  }
}