#include <LuaForOpenSiv3D/Environment/GlobalEnvironment.h>

void s3d::LuaScript::detail::GlobalEnvironment_impl::ExecuteFromFile(const Internal::String& file_name) {
	IEnvironment::ExecuteFromFile(file_name, this->m_state);
}
