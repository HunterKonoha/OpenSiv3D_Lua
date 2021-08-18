#include <LuaForOpenSiv3D/Executer/ExecuteResult.h>
#include <LuaForOpenSiv3D/InternalStrings.h>
#include <Siv3D/Logger.hpp>

s3d::LuaScript::ExecuteResult::ExecuteResult(sol::protected_function_result&& execute_result) :m_execute_result(std::move(execute_result)){
	if (!m_execute_result.valid()) {
		sol::error err = m_execute_result;
		Internal::String err_msg = Internal::convertString(err.what());
		Internal::String out_msg = Internal::convertString("Lua.Sol error : ") + err_msg;
		Logger << out_msg;
	}
}
