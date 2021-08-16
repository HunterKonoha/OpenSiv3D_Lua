#pragma once

template<typename ResultType>
std::conditional_t<std::is_same_v<ResultType, void>, void, std::optional<ResultType>> s3d::LuaScript::detail::GlobalEnvironment_impl::executeFromString(const Internal::String& execute_command) {
	sol::protected_function_result execute_result = this->m_state.script(Internal::convertString(execute_command));
	
	if constexpr (std::is_same_v<ResultType, void>) {
		s3d::LuaScript::checkExecuteResultWithLogger(execute_result);
	}
	else {
		if (s3d::LuaScript::checkExecuteResultWithLogger(execute_result)) {
			return s3d::LuaScript::getExecuteResultFromRawSolResult<ResultType>(execute_result);
		}
		else {
			return std::nullopt;
		}
	}
}
