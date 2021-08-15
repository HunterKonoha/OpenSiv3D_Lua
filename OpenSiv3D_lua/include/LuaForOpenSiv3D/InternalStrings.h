#pragma once
#include <Siv3D/String.hpp>
#include <string>

namespace s3d::LuaScript::Internal {
	using String = s3d::String;

	template<typename ToType, typename FromType>
	ToType convertString(const FromType&) {
		static_assert("");
	}

	template<>
	std::string convertString<std::string, s3d::String>(const s3d::String& string);
	template<>
	s3d::String convertString<s3d::String, std::string>(const std::string& string);
}