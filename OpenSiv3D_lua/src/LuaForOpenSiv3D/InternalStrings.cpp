#include <LuaForOpenSiv3D/InternalStrings.h>
#include <Siv3D/Unicode.hpp>

template<>
std::string s3d::LuaScript::Internal::convertString<std::string, s3d::String>(const s3d::String& string) {
	return string.narrow();
}

template<>
s3d::String s3d::LuaScript::Internal::convertString(const std::string& string) {
	return s3d::Unicode::Widen(string);
}
