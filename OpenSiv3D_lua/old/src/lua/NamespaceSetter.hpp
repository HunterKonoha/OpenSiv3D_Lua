#pragma once
#include <Siv3D.hpp>
#include <sol/sol.hpp>
#include "IScriptSetter.hpp"

namespace s3d {
  namespace Lua {
    class NamespaceSetter :public s3d::Lua::IScriptSetter<s3d::Lua::NamespaceSetter> {
      friend class IScriptSetter<NamespaceSetter>;

    private:
      sol::table m_table;

      sol::table& getScript() {
        return m_table;
      }

    public:
      explicit NamespaceSetter(const sol::table& table)
        :m_table(table) {

      }

      NamespaceSetter(const NamespaceSetter&) = default;
      NamespaceSetter(NamespaceSetter&&) = default;
      NamespaceSetter& operator=(const NamespaceSetter&) = default;
      NamespaceSetter& operator=(NamespaceSetter&&) = default;
    };
  }
}