#pragma once
#include <sol/sol.hpp>


namespace s3d::LuaScript {
	class ExecuteResult {
		// private:
		public: // for debug
			sol::protected_function_result m_execute_result;

		public:
			ExecuteResult() = delete;
			ExecuteResult(sol::protected_function_result&& execute_result);
	};
}