#pragma once
#include <Siv3D.hpp>
#include <sol.hpp>
#include "Binding.hpp"


namespace s3d {
  namespace Lua {
    namespace detail {
      //パニック関数
      inline int atPanic(lua_State* l) {
        try {
          return sol::detail::default_at_panic(l);
        }
        catch (sol::error& err) {
          Log << CharacterSet::Widen(err.what());
          throw err;
        }
      }
    }

    class IScriptState {
    private:
      static Optional<sol::state> m_state;

    protected:
      IScriptState() {
        if (!m_state.has_value()) {
          m_state = sol::state{};
          m_state->set_panic(&detail::atPanic);
          Binding::All();
        }
      }

      sol::state& getState() {
        return *m_state;
      }

    public:
      template<typename ...Arg>
      void  openLibraries(Arg... args) {
        if (m_state) {
          m_state->open_libraries(std::forward<Arg>(args)...);
        }
      }
    };
  }
}
