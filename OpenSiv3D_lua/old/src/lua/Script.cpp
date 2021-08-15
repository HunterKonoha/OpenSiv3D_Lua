#include "Script.hpp"

s3d::Lua::Script::Script() :m_env(getState(), sol::create, getState().globals()) {

}

sol::environment & s3d::Lua::Script::getScript() {
  return m_env;
}

const sol::environment & s3d::Lua::Script::getScript() const {
  return m_env;
}
