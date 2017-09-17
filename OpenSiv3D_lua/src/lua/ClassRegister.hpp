#pragma once
#include <Siv3D.hpp>
#include <sol.hpp>

namespace s3d {
  namespace Lua {

    enum class RegisterOption {
      Any,
      ReadOnly,
      WriteOnly
    };

    template<typename T>
    class ClassRegister {
    private:
      Array<std::function<void(std::reference_wrapper<sol::simple_usertype<T>>)>> m_func;

    public:
      template<typename ...Arg>
      ClassRegister& constructor() {
        m_func.emplace_back([=](auto ref) {ref.get().set(sol::meta_function::construct, sol::constructors<Arg...>()); });
        return *this;
      }

      template<typename V>
      ClassRegister& value(const String& valueName, V&& memberValuePointer, RegisterOption option = RegisterOption::Any) {
        static_assert(std::is_member_object_pointer_v<V>, "Argument must be member object pointer.");
        switch (option) {
          default:
          case s3d::Lua::RegisterOption::Any:
            m_func.emplace_back([=](auto ref) {ref.get().set(valueName.narrow(), memberValuePointer); });
            break;

          case s3d::Lua::RegisterOption::ReadOnly:
            m_func.emplace_back([=](auto ref) {ref.get().set(valueName.narrow(), sol::readonly(memberValuePointer)); });
            break;

          case s3d::Lua::RegisterOption::WriteOnly:
            m_func.emplace_back([=](auto ref) {ref.get().set(valueName.narrow(), sol::writeonly_property(memberValuePointer)); });
            break;
        }
        return *this;
      }

      template<typename F>
      ClassRegister& function(const String& functionName, F&& function) {
        m_func.emplace_back([=](auto ref) {ref.get().set(functionName.narrow(), function); });
        return *this;
      }

      template<typename F, typename R>
      ClassRegister& property(const String& valueName, F&& getter, R&& setter) {
        m_func.emplace_back([=](auto ref) {ref.get().set(valueName.narrow(), sol::property(getter, setter)); });
        return *this;
      }

      template<typename F>
      ClassRegister& readOnlyProperty(const String& valueName, F&& getter) {
        m_func.emplace_back([=](auto ref) {ref.get().set(valueName.narrow(), sol::readonly_property(getter)); });
        return *this;
      }

      template<typename F>
      ClassRegister& writeOnlyProperty(const String& valueName, F&& setter) {
        m_func.emplace_back([=](auto ref) {ref.get().set(valueName.narrow(), sol::writeonly_property(setter)); });
        return *this;
      }

      template<typename F>
      ClassRegister& metaFunction(sol::meta_function meta, F&& function) {
        m_func.emplace_back([=](auto ref) {ref.get().set(meta, function); });
        return *this;
      }

      void applyCommand(std::reference_wrapper<sol::simple_usertype<T>> userType)const {
        m_func.each([&](auto& func) {func(userType); });
      }
    };

    template<typename T>
    class ClassRegisterWithRAII :public ClassRegister<T> {
    private:
      sol::simple_usertype<T> m_usertype;
      std::function<void(std::string, sol::simple_usertype<T>&)> m_register;
      String m_classname;
      bool m_load = false;

    public:
      ClassRegisterWithRAII(const String& className, sol::simple_usertype<T> userType, const std::function<void(std::string, sol::simple_usertype<T>&)>& func)
        :m_usertype(std::move(userType)), m_register(func), m_classname(className) {

      }

      ClassRegisterWithRAII(const ClassRegisterWithRAII&) = default;
      ClassRegisterWithRAII(ClassRegisterWithRAII&&) = default;

      ~ClassRegisterWithRAII() {
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