#pragma once

template<typename ResultType>
ResultType s3d::LuaScript::getExecuteResultFromRawSolResult(const sol::protected_function_result& result) {
	return result.get<ResultType>();
}