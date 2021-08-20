#pragma once
#include <sol/sol.hpp>
#include <type_traits>
#include <format>


namespace s3d::LuaScript {
	namespace detail {
		template<typename T>
		struct IsTupple : public std::false_type{};
		template<typename ...Types>
		struct IsTupple<std::tuple<Types...>> :public std::true_type {};
	}

	class ExecuteResult {
		// private:
		public: // for debug
			sol::protected_function_result m_execute_result;
			bool m_has_value;
			bool e = detail::IsTupple<std::tuple<int, double>>::value;

		public:
			ExecuteResult() = delete;
			ExecuteResult(sol::protected_function_result&& execute_result);
			template<typename T>
			operator T()const;
			bool isExecutionSucceeded()const;
			bool hasValue()const;
			sol::type getValueType(uint32_t stack_offset = 0)const;
			int getValueCount()const;
			template<typename T, std::enable_if_t<!detail::IsTupple<T>::value, void*> = nullptr>
			T getValue(uint32_t stack_offset = 0)const;
			template<typename T, std::enable_if_t<detail::IsTupple<std::remove_cvref_t<T>>::value, void*> = nullptr>
			T getValue(uint32_t stack_offset = 0)const;
			
			
	};
}

#include "detail/ExecuteResult.ipp"