#include <LuaForOpenSiv3D/Environment/GlobalEnvironment.h>
#include <LuaForOpenSiv3D/Error/ErrorUtility.h>
#include <sol/sol.hpp>
#include <Siv3D/Logger.hpp>
#include <Siv3D/Console.hpp>

void s3d::LuaScript::detail::GlobalEnvironment_impl::executeFromFile(const Internal::String& file_name) {
	sol::protected_function_result execute_result = this->m_state.script_file(Internal::convertString(file_name));

	if (execute_result.valid()) {
		Console << U"Success!";
	}
	else {
		Internal::String err_msg = Internal::convertString("Lua.Sol error : ") + getErrorMessasgeInExecuseResult(execute_result).value();
		Logger << err_msg;
	}
}
