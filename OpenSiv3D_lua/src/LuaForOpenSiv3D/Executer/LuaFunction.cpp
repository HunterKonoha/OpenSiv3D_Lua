#include <LuaForOpenSiv3D/Executer/LuaFunction.h>

s3d::LuaScript::LuaFunction::LuaFunction(const sol::function& func) :m_func(std::move(func)) {
}

bool s3d::LuaScript::LuaFunction::isValid() const {
	return this->m_func.valid();
}
