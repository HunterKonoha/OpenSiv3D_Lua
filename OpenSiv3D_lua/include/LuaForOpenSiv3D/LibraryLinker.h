#pragma once

#if defined(_DEBUG) || defined(DEBUG)
#pragma comment(lib, "OpenSiv3D_lua_d.lib")
#else
#pragma comment(lib, "OpenSiv3D_lua.lib")
#endif