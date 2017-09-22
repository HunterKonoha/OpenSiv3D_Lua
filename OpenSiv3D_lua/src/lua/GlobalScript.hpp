#pragma once
#include "IScriptState.hpp"
#include "IScriptExecutor.hpp"
#include "IScriptSetter.hpp"
#include "IScriptLoader.hpp"

namespace s3d {
  namespace Lua {
    namespace detail {
      class GlobalScript_impl : public IScriptState, public IScriptExecutor<GlobalScript_impl>, public IScriptSetter<GlobalScript_impl>, public IScriptLoader<GlobalScript_impl> {
        friend class IScriptExecutor<GlobalScript_impl>;
        friend class IScriptSetter<GlobalScript_impl>;
        friend class IScriptLoader<GlobalScript_impl>;

      public:
        GlobalScript_impl() = default;
        ~GlobalScript_impl() = default;

      private:
        sol::state& getScript();
        const sol::state& getScript()const;
      };
    }

    static detail::GlobalScript_impl GlobalScript;
  }
}