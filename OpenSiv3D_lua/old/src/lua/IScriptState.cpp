#include "IScriptState.hpp"
#include "Binding.hpp"
#include <Siv3D/Unicode.hpp>
#include <Siv3D/Logger.hpp>

Optional<sol::state> s3d::Lua::IScriptState::m_state = none;

//�p�j�b�N�֐�
namespace s3d::Lua::detail {
  inline int atPanic(lua_State* l) {
    try {
      return sol::default_at_panic(l);
    }
    catch (sol::error& err) {
      Logger << s3d::Unicode::Widen(err.what());
      throw err;
    }
  }
}

s3d::Lua::IScriptState::IScriptState() {
  if (!m_state.has_value()) {
    m_state = sol::state{};
    m_state->set_panic(&detail::atPanic);
    // Binding::All();
  }
}

sol::state & s3d::Lua::IScriptState::getState() {
  return *m_state;
}

const sol::state & s3d::Lua::IScriptState::getState() const {
  return *m_state;
}