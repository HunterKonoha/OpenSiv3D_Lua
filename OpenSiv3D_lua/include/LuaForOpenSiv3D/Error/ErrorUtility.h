#pragma once
#include <LuaForOpenSiv3D/InternalStrings.h>
#include <sol/sol.hpp>
#include <optional>
#include <Siv3D/Logger.hpp>
#include <Siv3D/Console.hpp> // for debug

namespace s3d::LuaScript {
	std::optional<Internal::String> getErrorMessasgeInExecuseResult(const sol::protected_function_result& executed_result);

	template<typename ResultType>
	ResultType getExecuteResultFromRawSolResult(const sol::protected_function_result& result);
	bool checkExecuteResultWithLogger(const sol::protected_function_result& result);
}

#include "detail/ErrorUtility.ipp"