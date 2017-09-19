#pragma once


#pragma comment(lib,"lua53.lib")
#include <sol.hpp>

#include "lua/SolStack.hpp"
#include "lua/NamespaceSetter.hpp"
#include "lua/Class.hpp"
#include "lua/ClassSetter.hpp"
#include "lua/Coroutine.hpp"
#include "lua/GlobalScript.hpp"
#include "lua/Script.hpp"

namespace s3d {
  namespace Lua {
    using lib = sol::lib;
  }
}