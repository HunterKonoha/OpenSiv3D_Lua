#pragma once
#include <sol/sol.hpp>
#include <LuaForOpenSiv3D/Environment/IEnvironment.h>
#include <LuaForOpenSiv3D/Error/ErrorUtility.h>
#include <LuaForOpenSiv3D/InternalStrings.h>
#include <optional>

namespace s3d::LuaScript {
	namespace detail {
		class GlobalEnvironment_impl : public s3d::LuaScript::IEnvironment {
			//private:
			public: // for test
				sol::state m_state;
				//sol::environment m_global_env;

			public:
				void executeFromFile(const Internal::String& file_name);
				template<typename ResultType = void>
				std::conditional_t<std::is_same_v<ResultType, void>, void, std::optional<ResultType>> executeFromString(const Internal::String& execute_command);
		};
	}

	static s3d::LuaScript::detail::GlobalEnvironment_impl GlobalEnvironment;
}

#include "detail/GlobalEnvironment.ipp"