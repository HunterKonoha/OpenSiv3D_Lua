#pragma once
#include <Siv3D.hpp>
#include <sol.hpp>

namespace s3d::Lua {
  template<typename Self>
  class IScriptLoader {
  private:
    Self& selfCast() {
      return *static_cast<Self*>(this);
    }

    decltype(auto) getSolScript() {
      return selfCast().getScript();
    }

    sol::state& getSolState() {
      return selfCast().getState();
    }

  public:
    bool loadFromFile(const FilePath& path) {
      //�ǂݍ��݃G���[�̃R�[���o�b�N�֐�
      auto error_func = [](lua_State* L, sol::protected_function_result pfr) {
        //�ǂ̎�ނ̃G���[���擾
        String error = L"{} error"_fmt(sol::to_string(pfr.status()));
        //�ς܂�Ă���X�^�b�N��������Ȃ�擾���āA�G���[���b�Z�[�W�Ƃ��ď���
        if (sol::type_of(L, pfr.stack_index()) == sol::type::string) {
          error += L":{}"_fmt(sol::stack::get<std::wstring>(L, pfr.stack_index()));
        }
        Log << error;
        return pfr;
      };
      //lua�X�N���v�g�ǂݍ���
      return getSolState().script_file(path.narrow(), getSolScript(), error_func, sol::load_mode::any).valid();
    }
  };
}