#pragma once
#include <Siv3D.hpp>
#include <sol.hpp>
#include "NamespaceRegister.hpp"
#include "Coroutine.hpp"
#include "Function.hpp"

namespace s3d {
  namespace Lua {
    template<typename Self>
    class IScriptExecutor {
    private:
      HashMap<String, Coroutine> m_coroutine;

      Self& selfCast() {
        return *static_cast<Self*>(this);
      }

      decltype(auto) getScript() {
        return selfCast().getScript();
      }

      decltype(auto) getSolState() {
        return selfCast().getState();
      }

    public:
      IScriptExecutor() {

      }

      explicit operator bool()const {
        return getScript().valid();
      }

      bool loadFromFile(const FilePath& path) {
        //読み込みエラーのコールバック関数
        auto error_func = [](lua_State* L, sol::protected_function_result pfr) {
          //どの種類のエラーか取得
          String error = L"{} error"_fmt(sol::to_string(pfr.status()));
          //積まれているスタックが文字列なら取得して、エラーメッセージとして処理
          if (sol::type_of(L, pfr.stack_index()) == sol::type::string) {
            error += L":{}"_fmt(sol::stack::get<std::wstring>(L, pfr.stack_index()));
          }
          Log << error;
          return pfr;
        };
        //luaスクリプト読み込み
        return getSolState().script_file(path.narrow(), getScript(), error_func, sol::load_mode::any).valid();
      }

      template<typename T>
      ClassRegisterWithRAII<T> setClass(const String& className) {
        return { className, getScript().create_simple_usertype<T>(), std::bind(&std::remove_reference_t<decltype(getScript())>::set_usertype<std::string, T>, std::ref(getScript()), std::placeholders::_1, std::placeholders::_2) };
      }

      template<typename T>
      Self& setClass(const String& className, const ClassRegister<T>& classRegister) {
        auto usertype = getScript().create_simple_usertype<T>();
        classRegister.applyCommand(std::ref(usertype));
        getScript().set_usertype(className.narrow(), usertype);
        return selfCast();
      }

      NamespaceRegister setNamespace(const String& namespaceName) {
        auto arr = namespaceName.split(L'.');
        NamespaceRegister regist;
        if constexpr(std::is_same_v<std::remove_reference_t<decltype(getScript())>, sol::state>){
          regist = { getScript().create_named_table(arr.front().narrow()) };
        }
        else {
          regist = { getScript().create_named(arr.front().narrow()) };
        }
        for (auto&& i : step(1, arr.size() - 1)) {
          regist = regist.setNamespace(arr[i]);
        }
        return regist;
      }

      template<typename F>
      Self& setFunction(const String& functionName, F&& function) {
        getScript().set_function(functionName.narrow(), function);
        return selfCast();
      }

      template<typename T>
      Self& setValue(const String& valueName, const T& value) {
        getScript()[valueName.narrow()].set(Lua::detail::makeHelper(value));
        return selfCast();
      }

      template<typename T>
      T getValue(const String& valueName) {
        return getValue(valueName);
      }

      auto getValue(const String& valueName) {
        return getScript()[valueName.narrow()];
      }

      template<typename Result, typename ...Arg>
      std::enable_if_t<!std::is_same_v<Result, void>, Result> callFunction(const String& functionName, Arg&&... arg) {
        return callFunction(functionName, arg...);
      }

      template<typename Result, typename ...Arg>
      std::enable_if_t<std::is_same_v<Result, void>, Result> callFunction(const String& functionName, Arg&&... arg) {
        callFunction(functionName, arg...);
      }

      template<typename ...Arg>
      sol::function_result callFunction(const String& functionName, const Arg&... arg) {
        return getScript()[functionName.narrow()](Lua::detail::makeHelper(arg)...);
      }

      template<typename T>
      Function getFuntion(const String& functionName) {
        auto func = getScript()[functionName.narrow()];
        if (func.get_type() == sol::type::function) {
          return Function{ func.get<sol::protect_function>() };
        }
        else {
          return Function{};
        }
      }

      Coroutine getCoroutine(const String& functionName) {
        auto it = m_coroutine.find(functionName);
        if (it != m_coroutine.end() && !it->second) {
          m_coroutine.erase(it);
          it = m_coroutine.insert(std::make_pair(functionName, Coroutine(getScript()[functionName.narrow()].get<sol::protected_function>()))).first;
        }
        if (it == m_coroutine.end()) {
          it = m_coroutine.insert(std::make_pair(functionName, Coroutine(getScript()[functionName.narrow()].get<sol::protected_function>()))).first;
        }
        return it->second;
      }

      template<typename ...Arg>
      sol::protected_function_result callCoroutine(const String& functionName, const Arg&... arg) {
        auto it = m_coroutine.find(functionName);
        if (it != m_coroutine.end() && !it->second) {
          m_coroutine.erase(it);
          it = m_coroutine.insert(std::make_pair(functionName, Coroutine(getScript()[functionName.narrow()].get<sol::protected_function>()))).first;
        }
        if (it == m_coroutine.end()) {
          it = m_coroutine.insert(std::make_pair(functionName, Coroutine(getScript()[functionName.narrow()].get<sol::protected_function>()))).first;
        }
        return it->second.call(arg...);
      }
    };
  }
}