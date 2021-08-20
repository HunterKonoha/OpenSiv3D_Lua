#include <LuaForOpenSiv3D/Executer/ExecuteResult.h>
#include <LuaForOpenSiv3D/InternalStrings.h>
#include <Siv3D/Logger.hpp>
#include <format>

s3d::LuaScript::ExecuteResult::ExecuteResult(sol::protected_function_result&& execute_result) :m_execute_result(std::move(execute_result)){
	// �X�N���v�g/�֐��̎��s���ɒl��Ԃ��Ȃ����̂����s����ƁC�߂�l��sol::type::none�ƂȂ邪�C����̓X�^�b�N�ɐς܂�Ȃ��l�q
	// �܂��C�����X�N���v�g�̊֐������s�����ꍇ�́C���̊֐��̖߂�l���X�N���v�g�ǂݍ��ݎ���sol::protected_function_result�̃X�^�b�N�ɐς܂�邽�߁C
	// �X�N���v�g�ł̎��s���ʂ�none���֐��Ăяo���Ă��̌��ʂ����݂����ꍇ�ɁC�ŏ��̃X�N���v�g��sol::protected_function_result��get���Ăяo���ƁC�֐��̎��s���ʂ������Ă��܂�
	// ���̂��߁C�����Œl�����݂��Ă��邩�m�F���s��
	this->m_has_value = (this->m_execute_result.get_type() != sol::type::none);
	// �ǂݍ���/���s���̃G���[��Logger�ɏo��
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
