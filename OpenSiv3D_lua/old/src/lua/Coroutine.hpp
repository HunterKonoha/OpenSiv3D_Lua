#pragma once
#include <Siv3D.hpp>
#include <sol/sol.hpp>
#include "SolStack.hpp"

namespace s3d {
  namespace Lua {

    class Coroutine {
    private:
      sol::protected_function m_func;
      std::shared_ptr<sol::coroutine> m_coroutine;

    public:
      Coroutine(sol::protected_function func) : m_func(func) {
        if (!m_coroutine) {
          m_coroutine = std::make_shared<sol::coroutine>(m_func);
        }
        if (m_coroutine->status() == sol::call_status::ok) {
          throw std::runtime_error("Coroutine function does not contain yeild.");
        }
      }

      explicit operator bool()const {
        return m_coroutine->valid();
      }

      template<typename ...Arg>
      sol::protected_function_result call(const Arg&... arg) {
        auto ret = (*m_coroutine)(arg...);
        if (m_coroutine->status() == sol::call_status::ok) {
          reset();
        }
        return ret;
      }

      Coroutine& reset() {
        *m_coroutine = m_func;
        return *this;
      }

      sol::call_status getStatus() const {
        return m_coroutine->status();
      }
    };
  }
}