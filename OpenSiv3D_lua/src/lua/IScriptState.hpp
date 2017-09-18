#pragma once
#include <Siv3D.hpp>
#include <sol.hpp>

namespace s3d {
  namespace Lua {
    class IScriptState {
    private:
      static Optional<sol::state> m_state;

    protected:
      IScriptState();
      sol::state& getState();

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
