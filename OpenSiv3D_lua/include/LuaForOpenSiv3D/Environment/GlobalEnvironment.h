#pragma once
#include <sol/sol.hpp>
#include <LuaForOpenSiv3D/Environment/IEnvironment.h>
#include <LuaForOpenSiv3D/InternalStrings.h>

namespace s3d::LuaScript {
	namespace detail {
		class GlobalEnvironment_impl : public s3d::LuaScript::IEnvironment {
			//private:
			public: // for test
				sol::state m_state;
				//sol::environment m_global_env;

			public:
				void ExecuteFromFile(const Internal::String& file_name);
		};
	}

	static s3d::LuaScript::detail::GlobalEnvironment_impl GlobalEnvironment;
}