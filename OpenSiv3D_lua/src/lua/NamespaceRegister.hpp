#pragma once
#include <Siv3D.hpp>
#include <sol.hpp>
#include "ClassRegister.hpp"

namespace s3d {
  namespace Lua {

    class NamespaceRegister {
    private:
      sol::table m_table;

    public:
      explicit NamespaceRegister(const sol::table& table)
        :m_table(table) {

      }

      NamespaceRegister() = default;
      NamespaceRegister(const NamespaceRegister&) = default;
      NamespaceRegister(NamespaceRegister&&) = default;
      ~NamespaceRegister() = default;
      NamespaceRegister& operator=(const NamespaceRegister&) = default;
      NamespaceRegister& operator=(NamespaceRegister&&) = default;

      template<typename V>
      NamespaceRegister& setValue(const String& valueName, V&& value) {
        m_table.set(valueName.narrow(), value);
        return *this;
      }

      NamespaceRegister setNamespace(const String& namespaceName) {
        auto arr = namespaceName.split(L'.');
        NamespaceRegister regist(m_table.create_named(arr.front().narrow()));
        for (auto&& i : step(1, arr.size() - 1)) {
          regist = regist.setNamespace(arr[i]);
        }
        return regist;
      }


      template<typename T>
      ClassRegisterWithRAII<T> setClass(const String& className) {
        return { className, m_table.create_simple_usertype<T>(), std::bind(&sol::table::set_usertype<std::string, T>, m_table, std::placeholders::_1, std::placeholders::_2) };
      }

      template<typename F>
      NamespaceRegister& setFunction(const String& functionName, F&& function) {
        m_table.set_function(functionName.narrow(), function);
        return *this;
      }

      template<typename T>
      NamespaceRegister& setClass(const String& className, const ClassRegister<T>& classRegister) {
        auto usertype = m_table.create_simple_usertype<T>();
        classRegister.applyCommand(std::ref(usertype));
        m_table.set_usertype(className.narrow(), usertype);
        return *this;
      }

    };
  }
}