#pragma once


template<typename T, std::enable_if_t<!s3d::LuaScript::detail::IsTupple<T>::value, void*>>
T s3d::LuaScript::ExecuteResult::getValue(uint32_t stack_offset)const {
	// ポインタ・参照・constの修飾にはsolが対応していないので，ついている場合はビルドエラーにする
	static_assert(!std::is_pointer_v<T> && !std::is_reference_v<T> && !std::is_const_v<T>, "typename T is not pointer, reference, and const at ExecuteResult::getValue.");
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
	if (this->getValueCount() <= stack_offset) {
		std::string error_msg = std::format("'stack_offset' is out of index in Lua execution result at LuaScript::ExecuteResult::getValueType! The exepected range is 0 to {}, but actual input index is {}.", this->getValueCount() - 1, stack_offset);
		throw std::out_of_range(error_msg);
	}
	// 値を返す
	return this->m_execute_result.get<T>(stack_offset);
}
template<typename T, std::enable_if_t<s3d::LuaScript::detail::IsTupple<std::remove_cvref_t<T>>::value, void*>>
T s3d::LuaScript::ExecuteResult::getValue(uint32_t stack_offset)const {
	// ポインタ・参照・constの修飾にはsolが対応していないので，ついている場合はビルドエラーにする
	static_assert(!std::is_pointer_v<T> && !std::is_reference_v<T> && !std::is_const_v<T>, "typename T is not pointer, reference, and const at ExecuteResult::getValue.");
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
	if (this->getValueCount() - std::tuple_size_v<T> < stack_offset) {
		std::string error_msg = std::format("'stack_offset' is out of index in Lua execution result at LuaScript::ExecuteResult::getValueType! The exepected range is 0 to {}. but actual last input index is {}.", this->getValueCount() - 1, stack_offset + (std::tuple_size_v<T> -1));
		throw std::out_of_range(error_msg);
	}
	// 値を返す
	return this->m_execute_result.get<T>(stack_offset);
}