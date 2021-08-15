#pragma once
#include "IScriptState.hpp"
#include "IScriptExecutor.hpp"
#include "IScriptSetter.hpp"
#include "IScriptLoader.hpp"

namespace s3d {
  namespace Lua {
    class Script :public IScriptState, public IScriptExecutor<Script>, public IScriptSetter<Script>, public IScriptLoader<Script> {
      friend class IScriptExecutor<Script>;
      friend class IScriptSetter<Script>;
      friend class IScriptLoader<Script>;

    private:
      sol::environment m_env;

    public:
      Script();
      ~Script() = default;

    private:
      sol::environment& getScript();
      const sol::environment& getScript()const;
    };
  }
}