#pragma once
#include <Siv3D/String.hpp>
#include <string>
#include <concepts>

namespace s3d::LuaScript::Internal {
	using String = s3d::String;


	std::string convertString(const s3d::String& string);
	s3d::String convertString(const std::string& string);
}