#include <LuaForOpenSiv3D/Environment/GlobalEnvironment.h>
#include <sol/sol.hpp>

void s3d::LuaScript::detail::GlobalEnvironment_impl::ExecuteFromFile(const Internal::String& file_name) {
	this->m_state.script_file(Internal::convertString(file_name));
	auto f = [](lua_State*, sol::protected_function_result pfr) {
		sol::error err = pfr;
		err.what();
	};
}

void s3d::LuaScript::detail::GlobalEnvironment_impl::ExecuteFromString(const Internal::String& execute_command) {
	this->m_state.script(Internal::convertString(execute_command));
}
