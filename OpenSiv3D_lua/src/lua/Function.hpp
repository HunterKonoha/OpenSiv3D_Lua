#pragma once
#include <Siv3D.hpp>
#include <sol.hpp>
#include "SolStack.hpp"

namespace s3d {
  namespace Lua {
    class Function {
    private:
      sol::protected_function m_func;

    public:
      Function() = default;
      explicit Function(sol::protected_function func) :m_func(func) {

      };

      ~Function() = default;

      explicit operator bool()const {
        return m_func.valid();
      }

      template<typename ...Arg>
      auto operator()(Arg&&... arg)const {
        return call(arg...);
      }

      template<typename ...Arg>
      auto call(Arg&&... arg)const {
        if (!m_func.valid()) {
          throw std::runtime_error("Lua::Function has not value!");
        }
        else {
          return m_func(arg...);
        }
      }
    };
  }
}