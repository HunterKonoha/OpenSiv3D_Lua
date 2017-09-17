#include "Vec2Bind.h"
#include "../GlobalScript.hpp"
#include <Siv3D.hpp>


void s3d::Lua::Binding::Vec2Bind() {
  GlobalScript()
    .setClass<Vec2>(L"Vec2")
    .value(L"x", &Vec2::x)
    .value(L"y", &Vec2::y)
    .constructor<sol::types<>, sol::types<const Vec2&>, sol::types<double, double>, sol::types<const Point&>>()
    .metaFunction(sol::meta_function::unary_minus, sol::resolve<Vec2()const>(&Vec2::operator-))
    .metaFunction(sol::meta_function::addition, sol::resolve<Vec2(const Vec2&)const>(&Vec2::operator+))
    .metaFunction(sol::meta_function::subtraction, sol::resolve<Vec2(const Vec2&)const>(&Vec2::operator-))
    .metaFunction(sol::meta_function::multiplication, sol::overload(sol::resolve<Vec2(const Vec2&)const>(&Vec2::operator*), sol::resolve<Vec2(double)const>(&Vec2::operator*)))
    .metaFunction(sol::meta_function::division, sol::overload(sol::resolve<Vec2(const Vec2&)const>(&Vec2::operator/), sol::resolve<Vec2(double)const>(&Vec2::operator/)))
    .metaFunction(sol::meta_function::equal_to, &Vec2::operator==)
    .metaFunction(sol::meta_function::to_string, [](const Vec2& v) {return Format(v); })
    .function(L"xx", &Vec2::xx)
    .function(L"xy", &Vec2::xy)
    .function(L"yx", &Vec2::yx)
    .function(L"yy", &Vec2::yy)
    .function(L"asPoint", &Vec2::asPoint)
    .function(L"set", sol::overload(sol::resolve<Vec2&(double, double)>(&Vec2::set), sol::resolve<Vec2&(const Vec2&)>(&Vec2::set)))
    .function(L"movedBy", sol::overload(sol::resolve<Vec2(double, double)const>(&Vec2::movedBy), sol::resolve<Vec2(const Vec2&)const>(&Vec2::movedBy)))
    .function(L"moveBy", sol::overload(sol::resolve<Vec2&(double, double)>(&Vec2::moveBy), sol::resolve<Vec2&(const Vec2&)>(&Vec2::moveBy)))
    .function(L"clamp", &Vec2::clamp)
    .function(L"clamped", &Vec2::clamped)
    .function(L"isZero", &Vec2::isZero)
    .function(L"hasNaN", &Vec2::hasNaN)
    .function(L"dot", &Vec2::dot)
    .function(L"cross", &Vec2::cross)
    .function(L"length", &Vec2::length)
    .function(L"lengthSq", &Vec2::lengthSq)
    .function(L"lengthInv", &Vec2::lengthInv)
    .function(L"setLength", &Vec2::setLength)
    .function(L"distanceFrom", sol::overload(sol::resolve<double(double, double)const>(&Vec2::distanceFrom), sol::resolve<double(const Vec2&)const>(&Vec2::distanceFrom)))
    .function(L"distanceFromSq", sol::overload(sol::resolve<double(double, double)const>(&Vec2::distanceFromSq), sol::resolve<double(const Vec2&)const>(&Vec2::distanceFromSq)))
    .function(L"normalize", &Vec2::normalize)
    .function(L"normalized", &Vec2::normalized)
    .function(L"rotate", &Vec2::rotate)
    .function(L"rotated", &Vec2::rotated)
    .function(L"getAngle", &Vec2::getAngle)
    .function(L"projection", &Vec2::projection)
    .function(L"lerp", &Vec2::lerp)
    .function(L"projection", sol::overload(&Vec2::intersects<Point>,
      &Vec2::intersects<Vec2>,
      &Vec2::intersects<Line>,
      &Vec2::intersects<Rect>,
      &Vec2::intersects<RectF>,
      &Vec2::intersects<Circle>,
      &Vec2::intersects<Ellipse>,
      &Vec2::intersects<Triangle>,
      &Vec2::intersects<Quad>,
      &Vec2::intersects<RoundRect>,
      &Vec2::intersects<Polygon>))
    .function(L"Zero", &Vec2::Zero)
    .function(L"One", &Vec2::One)
    .function(L"Zero", &Vec2::All)
    .function(L"UnitX", &Vec2::UnitX)
    .function(L"UnitY", &Vec2::UnitY)
    .function(L"Left", &Vec2::Left)
    .function(L"Right", &Vec2::Right)
    .function(L"Up", &Vec2::Up)
    .function(L"Down", &Vec2::Down);
}
