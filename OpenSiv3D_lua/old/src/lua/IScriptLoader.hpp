#pragma once
#include <Siv3D.hpp>
#include <sol/sol.hpp>

namespace s3d::Lua {
  template<typename Self>
  class IScriptLoader {
  private:
    Array<FilePath> m_filepath;

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
        using namespace s3d::Literals::FormatLiterals;
      //読み込みエラーのコールバック関数
      auto error_func = [](lua_State* L, sol::protected_function_result pfr) {
        //どの種類のエラーか取得
        String error = U"{} error"_fmt(sol::to_string(pfr.status()));
        //積まれているスタックが文字列なら取得して、エラーメッセージとして処理
        if (sol::type_of(L, pfr.stack_index()) == sol::type::string) {
          error += U":{}"_fmt(sol::stack::get<std::wstring>(L, pfr.stack_index()));
        }
        Logger << error;
        return pfr;
      };
      //luaスクリプト読み込み
      if (!m_filepath.include(path)) {
        m_filepath.push_back(path);
      }
      return getSolState().script_file(path.narrow(), getSolScript(), error_func, sol::load_mode::any).valid();
    }

    const Array<FilePath>& getLoadFilePath()const {
      return m_filepath;
    }

    bool reload() {
      bool ret = true;
      for (auto&& path : m_filepath) {
        if (!loadFromFile(path)) {
          ret = false;
        }
      }
      return ret;
    }
  };
}