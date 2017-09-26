#pragma once
#include <Siv3D.hpp>
#include <sol.hpp>
#include "BitFieldAccessor.hpp"
#include "RegisterOption.hpp"

namespace s3d {
  namespace Lua {
    template<typename T>
    class ClassSetter {
    private:
      Array<std::function<void(std::reference_wrapper<sol::simple_usertype<T>>)>> m_func;

    public:
      template<typename ...Arg>
      ClassSetter& constructor() {
        m_func.emplace_back([=](auto ref) {ref.get().set(sol::meta_function::construct, sol::constructors<Arg...>()); });
        return *this;
      }

      template<typename V>
      ClassSetter& value(const String& valueName, V&& memberValuePointer, RegisterOption option = RegisterOption::Any) {
        static_assert(std::is_member_object_pointer_v<V>, "Argument must be member object pointer.");
        switch (option) {
          default:
          case s3d::Lua::RegisterOption::Any:
            m_func.emplace_back([=](auto ref) {ref.get().set(valueName.narrow(), memberValuePointer); });
            break;

          case s3d::Lua::RegisterOption::ReadOnly:
            m_func.emplace_back([=](auto ref) {ref.get().set(valueName.narrow(), sol::readonly_property(memberValuePointer)); });
            break;

          case s3d::Lua::RegisterOption::WriteOnly:
            m_func.emplace_back([=](auto ref) {ref.get().set(valueName.narrow(), sol::writeonly_property(memberValuePointer)); });
            break;
        }
        return *this;
      }

      template<std::size_t bitTarget = 0x0, std::size_t size = 0x1>
      ClassSetter& bitFieldValue(const String& valueName, RegisterOption option = RegisterOption::Any) {
        switch (option) {
          default:
          case s3d::Lua::RegisterOption::Any:
            m_func.emplace_back([=](auto ref) {ref.get().set(valueName.narrow(), sol::property(BitFieldAccessor::read<T, bitTarget, size>, BitFieldAccessor::write<T, bitTarget, size>)); });
            break;

          case s3d::Lua::RegisterOption::ReadOnly:
            m_func.emplace_back([=](auto ref) {ref.get().set(valueName.narrow(), sol::readonly_property(BitFieldAccessor::read<T, bitTarget, size>)); });
            break;

          case s3d::Lua::RegisterOption::WriteOnly:
            m_func.emplace_back([=](auto ref) {ref.get().set(valueName.narrow(), sol::writeonly_property(BitFieldAccessor::write<T, bitTarget, size>)); });
            break;
        }
        return *this;
      }

      template<typename F>
      ClassSetter& function(const String& functionName, F&& function){
        m_func.emplace_back([=](auto ref) {ref.get().set(functionName.narrow(), function); });
        return *this;
      }

      template<typename F, typename R>
      ClassSetter& property(const String& valueName, F&& getter, R&& setter) {
        m_func.emplace_back([=](auto ref) {ref.get().set(valueName.narrow(), sol::property(getter, setter)); });
        return *this;
      }

      template<typename F>
      ClassSetter& readOnlyProperty(const String& valueName, F&& getter) {
        m_func.emplace_back([=](auto ref) {ref.get().set(valueName.narrow(), sol::readonly_property(getter)); });
        return *this;
      }

      template<typename F>
      ClassSetter& writeOnlyProperty(const String& valueName, F&& setter) {
        m_func.emplace_back([=](auto ref) {ref.get().set(valueName.narrow(), sol::writeonly_property(setter)); });
        return *this;
      }

      template<typename F>
      ClassSetter& metaFunction(sol::meta_function meta, F&& function) {
        m_func.emplace_back([=](auto ref) {ref.get().set(meta, function); });
        return *this;
      }

      void applyCommand(std::reference_wrapper<sol::simple_usertype<T>> userType)const {
        m_func.each([&](auto& func) {func(userType); });
      }
    };

    template<typename T>
    class ClassSetterWithRAII :public ClassSetter<T> {
    private:
      sol::simple_usertype<T> m_usertype;
      std::function<void(std::string, sol::simple_usertype<T>&)> m_register;
      String m_classname;
      bool m_load = false;

    public:
      ClassSetterWithRAII(const String& className, sol::simple_usertype<T> userType, const std::function<void(std::string, sol::simple_usertype<T>&)>& func)
        :m_usertype(std::move(userType)), m_register(func), m_classname(className) {

      }

      ClassSetterWithRAII(const ClassSetterWithRAII&) = default;
      ClassSetterWithRAII(ClassSetterWithRAII&&) = default;

      ~ClassSetterWithRAII() {
        if (!m_load) {
          registerClass();
        }
      }

      void registerClass() {
        applyCommand(std::ref(m_usertype));
        m_register(m_classname.narrow(), m_usertype);
        m_load = true;
      }
    };
  }
}