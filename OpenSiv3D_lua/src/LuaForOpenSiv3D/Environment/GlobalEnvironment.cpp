#include <LuaForOpenSiv3D/Environment/GlobalEnvironment.h>
#include <LuaForOpenSiv3D/Error/ErrorUtility.h>
#include <sol/sol.hpp>
#include <Siv3D/Logger.hpp>
#include <Siv3D/Console.hpp>

s3d::LuaScript::ExecuteResult s3d::LuaScript::detail::GlobalEnvironment_impl::executeFromFile(const Internal::String& file_name) {
	sol::protected_function_result execute_result = this->m_state.safe_script_file(Internal::convertString(file_name));
	return s3d::LuaScript::ExecuteResult(std::move(execute_result));
}

s3d::LuaScript::ExecuteResult s3d::LuaScript::detail::GlobalEnvironment_impl::executeFromString(const Internal::String& execute_command) {
	sol::protected_function_result execute_result = this->m_state.safe_script(Internal::convertString(execute_command));
	return s3d::LuaScript::ExecuteResult(std::move(execute_result));
}
