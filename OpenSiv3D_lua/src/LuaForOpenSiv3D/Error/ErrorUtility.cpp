#include <LuaForOpenSiv3D/Error/ErrorUtility.h>

std::optional<s3d::LuaScript::Internal::String> s3d::LuaScript::getErrorMessasgeInExecuseResult(const sol::protected_function_result& executed_result) {
	std::optional<Internal::String> err_msg = std::nullopt;
	if (!executed_result.valid()) {
		sol::error err = executed_result;
		err_msg = Internal::convertString(err.what());
	}
	return err_msg;
}

bool s3d::LuaScript::checkExecuteResultWithLogger(const sol::protected_function_result& result) {
	if (result.valid()) {
		return true;
	}
	else {
		Internal::String err_msg = Internal::convertString("Lua.Sol error : ") + getErrorMessasgeInExecuseResult(result).value();
		Logger << err_msg;
		Console << err_msg;
		return false;
	}
}
