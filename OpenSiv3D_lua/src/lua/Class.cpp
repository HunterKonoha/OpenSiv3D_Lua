#include "Class.hpp"

s3d::Lua::Class::Class(const sol::table & table) :m_core(table) {

}

sol::table& s3d::Lua::Class::getScript() {
  return m_core;
}

const sol::table & s3d::Lua::Class::getScript() const {
  return m_core;
}
