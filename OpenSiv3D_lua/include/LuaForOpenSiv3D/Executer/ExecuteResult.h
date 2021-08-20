#pragma once
#include <sol/sol.hpp>
#include <Siv3D/Optional.hpp>
#include <optional>
#include <type_traits>
#include <format>


namespace s3d::LuaScript {
	namespace detail {
		//template<typename T>
		//constexpr inline bool is_optional_v = sol::meta::any_v<sol::meta::is_specialization_of<T, std::optional>, sol::meta::is_specialization_of<T, s3d::Optional>>;
	}

	class ExecuteResult {
		// private:
		public: // for debug
			sol::protected_function_result m_execute_result;
			bool m_has_value;

		public:
			ExecuteResult() = delete;
			ExecuteResult(sol::protected_function_result&& execute_result);
			template<typename T>
			operator T()const;
			bool isExecutionSucceeded()const;
			bool hasValue()const;
			sol::type getValueType(uint32_t stack_offset = 0)const;
			int getValueCount()const;

			template<typename T>
			decltype(auto) getValue(uint32_t stack_offset = 0)const {
				if constexpr (sol::meta::any_v<sol::meta::is_specialization_of<T, std::optional>, sol::meta::is_specialization_of<T, s3d::Optional>>) {
					std::size_t getting_size = 1;
					if constexpr(sol::meta::is_specialization_of_v<T::value_type, std::tuple>) {
						getting_size = std::tuple_size_v<T::value_type>;
					}
					// 例外時は無効値を返す
					if (!this->hasValue() || !this->isExecutionSucceeded() || this->getValueCount() - static_cast<int>(getting_size) < static_cast<int>(stack_offset)) {
						return T{ std::nullopt };
					}
					else {
						return this->m_execute_result.get<T>(stack_offset);
					}
				}
				else {
					std::size_t getting_size = 1;
					if constexpr (sol::meta::is_specialization_of_v<T, std::tuple>) {
						getting_size = std::tuple_size_v<T>;
					}
					// 値がないのに値を参照しようとしているので例外を投げる
					if (!this->hasValue()) {
						std::string error_msg = "The access of none of the value at ExecuteResult::getValue.";
						throw std::exception(error_msg.c_str());
					}
					// 読み込み/実行に失敗しているのに値を参照しようとしているので例外を投げる
					if (!this->isExecutionSucceeded()) {
						std::string error_msg = "Execution is failed, but the result is getting at ExecuteResult::getValue.";
						throw std::exception(error_msg.c_str());
					}
					// 戻り値の数よりも多い数を取得しようとしているので例外を投げる
					if (this->getValueCount() - static_cast<int>(getting_size) < static_cast<int>(stack_offset)) {
						std::string error_msg = std::format("'stack_offset' is out of index in Lua execution result at LuaScript::ExecuteResult::getValueType! The exepected range is 0 to {}, but actual input index is {}.", this->getValueCount() - 1, stack_offset);
						throw std::out_of_range(error_msg);
					}
					// 値を返す
					return this->m_execute_result.get<T>(stack_offset);
				}
			}

			//template<typename T, std::enable_if_t<!detail::IsTupple<T>::value, void*> = nullptr>
			//T getValue(uint32_t stack_offset = 0)const;
			//template<typename T, std::enable_if_t<detail::IsTupple<std::remove_cvref_t<T>>::value, void*> = nullptr>
			//T getValue(uint32_t stack_offset = 0)const;
			
			
	};
}

#include "detail/ExecuteResult.ipp"