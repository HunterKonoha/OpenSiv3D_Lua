#include <LuaForOpenSiv3D/InternalStrings.h>
#include <Siv3D/Unicode.hpp>

std::string s3d::LuaScript::Internal::convertString(const s3d::String& string) {
	return string.narrow();
}

s3d::String s3d::LuaScript::Internal::convertString(const std::string& string) {
	return s3d::Unicode::Widen(string);
}
