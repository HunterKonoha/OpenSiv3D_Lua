#pragma once
#include <sol/sol.hpp>
#include <LuaForOpenSiv3D/Executer/ExecuteResult.h>

namespace s3d::LuaScript {
	class LuaFunction {
		//private:
		public: // for debug
			sol::function m_func;

		public:
			LuaFunction() = default;
			LuaFunction(const sol::function& func);
			bool isValid()const;

			template<typename ...Types>
			ExecuteResult operator()(Types&&... arguments) {
				return this->call(std::forward<Types>(arguments)...);
			}

			template<typename ...Types>
			ExecuteResult call(Types&&... arguments)const {
				sol::protected_function_result execute_result = this->m_func.call(std::forward<Types>(arguments)...);
				return ExecuteResult(std::move(execute_result));
			}
	};
}