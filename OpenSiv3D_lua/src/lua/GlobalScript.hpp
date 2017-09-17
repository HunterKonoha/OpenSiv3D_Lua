#pragma once
#include "IScriptState.hpp"
#include "IScriptExecutor.hpp"

namespace s3d {
  namespace Lua {


    class GlobalScript : public IScriptState, public IScriptExecutor<GlobalScript> {
      friend class IScriptExecutor<GlobalScript>;
    public:
      GlobalScript() = default;
      ~GlobalScript() = default;

    private:
      sol::state& getScript() {
        return IScriptState::getState();
      }
    };
  }
}