#include "Vec2Bind.h"
#include "../Utility.hpp"
#include "../GlobalScript.hpp"
#include <Siv3D.hpp>


void s3d::Lua::Binding::Vec2Bind() {
  GlobalScript
    .setClass<Vec2>(U"Vec2")
    .value(U"x", &Vec2::x)
    .value(U"y", &Vec2::y)
    .constructor<sol::types<>, sol::types<const Vec2&>, sol::types<double, double>, sol::types<const Point&>>()
    .metaFunction(sol::meta_function::unary_minus, solveFunctionPtr<Vec2()const>(&Vec2::operator-))
    .metaFunction(sol::meta_function::addition, solveFunctionPtr<Vec2(const Vec2)const>(&Vec2::operator+))
    .metaFunction(sol::meta_function::subtraction, solveFunctionPtr<Vec2(const Vec2)const>(&Vec2::operator-))
    .metaFunction(sol::meta_function::multiplication, makeOverload(solveFunctionPtr<Vec2(const Vec2)const>(&Vec2::operator*), solveFunctionPtr<Vec2(double)const>(&Vec2::operator*)))
    .metaFunction(sol::meta_function::division, makeOverload(solveFunctionPtr<Vec2(const Vec2)const>(&Vec2::operator/), solveFunctionPtr<Vec2(double)const>(&Vec2::operator/)))
    //.metaFunction(sol::meta_function::equal_to, &Vec2::operator==)
    .metaFunction(sol::meta_function::to_string, [](const Vec2& v) {return Format(v); })
    .function(U"xx", &Vec2::xx)
    .function(U"xy", &Vec2::xy)
    .function(U"yx", &Vec2::yx)
    .function(U"yy", &Vec2::yy)
    .function(U"asPoint", &Vec2::asPoint)
    .function(U"set", makeOverload(solveFunctionPtr<Vec2&(double, double)>(&Vec2::set), solveFunctionPtr<Vec2&(const Vec2)>(&Vec2::set)))
    .function(U"movedBy", makeOverload(solveFunctionPtr<Vec2(double, double)const>(&Vec2::movedBy), solveFunctionPtr<Vec2(const Vec2)const>(&Vec2::movedBy)))
    .function(U"moveBy", makeOverload(solveFunctionPtr<Vec2&(double, double)>(&Vec2::moveBy), solveFunctionPtr<Vec2&(const Vec2)>(&Vec2::moveBy)))
    .function(U"clamp", &Vec2::clamp)
    .function(U"clamped", &Vec2::clamped)
    .function(U"isZero", &Vec2::isZero)
    .function(U"hasNaN", &Vec2::hasNaN)
    .function(U"dot", &Vec2::dot)
    .function(U"cross", &Vec2::cross)
    .function(U"length", &Vec2::length)
    .function(U"lengthSq", &Vec2::lengthSq)
   // .function(U"lengthInv", &Vec2::lengthInv)
    .function(U"setLength", &Vec2::setLength)
    .function(U"distanceFrom", makeOverload(solveFunctionPtr<double(double, double)const>(&Vec2::distanceFrom), solveFunctionPtr<double(const Vec2)const>(&Vec2::distanceFrom)))
    .function(U"distanceFromSq", makeOverload(solveFunctionPtr<double(double, double)const>(&Vec2::distanceFromSq), solveFunctionPtr<double(const Vec2)const>(&Vec2::distanceFromSq)))
    .function(U"normalize", &Vec2::normalize)
    .function(U"normalized", &Vec2::normalized)
    .function(U"rotate", &Vec2::rotate)
    .function(U"rotated", &Vec2::rotated)
   // .function(U"getAngle", &Vec2::getAngle)
    .function(U"projection", &Vec2::projection)
    .function(U"lerp", &Vec2::lerp)
    .function(U"projection", makeOverload(&Vec2::intersects<Point>,
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
    .function(U"Zero", &Vec2::Zero)
    .function(U"One", &Vec2::One)
    .function(U"Zero", &Vec2::All)
    .function(U"UnitX", &Vec2::UnitX)
    .function(U"UnitY", &Vec2::UnitY)
    .function(U"Left", &Vec2::Left)
    .function(U"Right", &Vec2::Right)
    .function(U"Up", &Vec2::Up)
    .function(U"Down", &Vec2::Down);
}
