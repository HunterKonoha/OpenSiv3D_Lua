#pragma once
#include "IScriptState.hpp"
#include "IScriptExecutor.hpp"

namespace s3d {
  namespace Lua {
    class Script :public IScriptState, public IScriptExecutor<Script> {
      friend class IScriptExecutor<Script>;
    private:
      sol::environment m_env;

    public:
      Script() :m_env(getState(), sol::create, getState().globals()) {

      }

      ~Script() = default;

    private:
      sol::environment& getScript() {
        return m_env;
      }
    };
  }
}