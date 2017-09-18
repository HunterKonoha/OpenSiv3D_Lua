#include "IScriptState.hpp"
#include "Binding.hpp"

Optional<sol::state> s3d::Lua::IScriptState::m_state = none;

//パニック関数
namespace s3d::Lua::detail {
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

s3d::Lua::IScriptState::IScriptState() {
  if (!m_state.has_value()) {
    m_state = sol::state{};
    m_state->set_panic(&detail::atPanic);
    Binding::All();
  }
}

sol::state & s3d::Lua::IScriptState::getState() {
  return *m_state;
}
