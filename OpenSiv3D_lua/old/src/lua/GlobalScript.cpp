#include "GlobalScript.hpp"

sol::state & s3d::Lua::detail::GlobalScript_impl::getScript() {
  return IScriptState::getState();
}

const sol::state & s3d::Lua::detail::GlobalScript_impl::getScript() const {
  return IScriptState::getState();
}
