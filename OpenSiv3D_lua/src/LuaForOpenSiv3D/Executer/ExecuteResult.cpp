#include <LuaForOpenSiv3D/Executer/ExecuteResult.h>
#include <LuaForOpenSiv3D/InternalStrings.h>
#include <Siv3D/Logger.hpp>
#include <format>

s3d::LuaScript::ExecuteResult::ExecuteResult(sol::protected_function_result&& execute_result) :m_execute_result(std::move(execute_result)){
	// スクリプト/関数の実行時に値を返さないものを実行すると，戻り値はsol::type::noneとなるが，これはスタックに積まれない様子
	// また，同じスクリプトの関数を実行した場合は，その関数の戻り値がスクリプト読み込み時のsol::protected_function_resultのスタックに積まれるため，
	// スクリプトでの実行結果がnone→関数呼び出してその結果が存在した場合に，最初のスクリプトのsol::protected_function_resultのgetを呼び出すと，関数の実行結果が得られてしまう
	// そのため，ここで値が存在しているか確認を行う
	this->m_has_value = (this->m_execute_result.get_type() != sol::type::none);
	// 読み込み/実行時のエラーをLoggerに出力
	if (!m_execute_result.valid()) {
		sol::error err = m_execute_result;
		Internal::String err_msg = Internal::convertString(err.what());
		Internal::String out_msg = Internal::convertString("Lua.Sol error : ") + err_msg;
		Logger << out_msg;
	}
}

bool s3d::LuaScript::ExecuteResult::isExecutionSucceeded() const {
	return this->m_execute_result.valid();
}

bool s3d::LuaScript::ExecuteResult::hasValue() const {
	return this->m_has_value;
}

sol::type s3d::LuaScript::ExecuteResult::getValueType(uint32_t stack_offset) const {
	if (this->m_has_value) {
		if (this->getValueCount() <= stack_offset) {
			std::string error_msg = std::format("'stack_offset' is out of index in Lua execution result at LuaScript::ExecuteResult::getValueType! The exepected range is 0 to {}, but actual input index is {}.", this->getValueCount() - 1, stack_offset);
			throw std::out_of_range(error_msg);
		}
		return this->m_execute_result.get_type(stack_offset);
	}
	else {
		return sol::type::none;
	}
}

int s3d::LuaScript::ExecuteResult::getValueCount() const {
	return this->m_execute_result.return_count();
}
