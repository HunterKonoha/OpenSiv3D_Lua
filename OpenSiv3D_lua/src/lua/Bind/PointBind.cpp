#include "PointBind.hpp"
#include "../GlobalScript.hpp"
#include <Siv3D.hpp>

void s3d::Lua::Binding::PointBind() {
    // meta_functionをなくす
    // overloadをなくして，関数ポインタを投げる感じに
  ClassSetter<Point> point;
  point.value(U"x", &Point::x)
    .value(U"y", &Point::y)
    .constructor<sol::types<>, sol::types<const Point&>, sol::types<int32, int32>>()
    .metaFunction(sol::meta_function::unary_minus, (Point(Point::*)()const)(&Point::operator-))
    .metaFunction(sol::meta_function::addition, sol::overload((Point(Point::*)(const Point)const)(&Point::operator+), &Point::operator+<double>, &Point::operator+<float>))
    .metaFunction(sol::meta_function::subtraction, sol::overload((Point(Point::*)(const Point)const)(&Point::operator-), &Point::operator-<double>, &Point::operator-<float>))
    .metaFunction(sol::meta_function::multiplication, sol::overload((Point(Point::*)(int32)const)(&Point::operator*), (Float2(Point::*)(float)const)(&Point::operator*), (Vec2(Point::*)(double)const)(&Point::operator*), (Point(Point::*)(const Point)const)(&Point::operator*), &Point::operator*<double>, &Point::operator*<float>))
    .metaFunction(sol::meta_function::division, sol::overload((Point(Point::*)(int32)const)(&Point::operator/), (Float2(Point::*)(float)const)(&Point::operator/), (Vec2(Point::*)(double)const)(&Point::operator/), (Point(Point::*)(const Point)const)(&Point::operator/), &Point::operator/<double>, &Point::operator/<float>))
    //.metaFunction(sol::meta_function::equal_to, &Point::operator==)
    .metaFunction(sol::meta_function::to_string, [](const Point& p) {return Format(p); })
    .function(U"xx", &Point::xx)
    .function(U"xy", &Point::xy)
    .function(U"yx", &Point::yx)
    .function(U"yy", &Point::yy)
    .function(U"set", sol::overload((Point&(Point::*)(const int32, const int32))(&Point::set), (Point&(Point::*)(const Point))(&Point::set)))
    .function(U"movedBy", sol::overload((Point(Point::*)(const int32, const int32)const)(&Point::movedBy), (Point(Point::*)(const Point)const)(&Point::movedBy), &Point::movedBy<double>, &Point::movedBy<float>))
    .function(U"moveBy", sol::overload((Point&(Point::*)(const int32, const int32))(&Point::moveBy), (Point&(Point::*)(const Point))(&Point::moveBy)))
    .function(U"isZero", &Point::isZero)
    .function(U"distanceFrom", sol::overload((double(Point::*)(const Point)const)(&Point::distanceFrom), &Point::distanceFrom<double>, &Point::distanceFrom<float>))
    .function(U"distanceFromSq", sol::overload((double(Point::*)(const Point)const)(&Point::distanceFromSq), &Point::distanceFromSq<double>, &Point::distanceFromSq<float>))
    .function(U"length", &Point::length<double>)
    .function(U"lengthSq", &Point::lengthSq<double>)
   // .function(U"lerp", sol::overload(&Point::lerp<double>, &Point::lerp<float>))
    .function(U"intersects", sol::overload(&Point::intersects<Point>,
      &Point::intersects<Vec2>,
      &Point::intersects<Float2>,
      &Point::intersects<Line>,
      &Point::intersects<Rect>,
      &Point::intersects<RectF>,
      &Point::intersects<Circle>,
      &Point::intersects<Ellipse>,
      &Point::intersects<Triangle>,
      &Point::intersects<Quad>,
      &Point::intersects<RoundRect>,
      &Point::intersects<Polygon>))
    .function(U"leftClicked", &Point::leftClicked)
    .function(U"leftPressed", &Point::leftPressed)
    .function(U"leftReleased", &Point::leftReleased)
    .function(U"rightClicked", &Point::rightClicked)
    .function(U"rightPressed", &Point::rightPressed)
    .function(U"rightReleased", &Point::rightReleased)
    .function(U"mouseOver", &Point::mouseOver)
    .function(U"Zero", &Point::Zero)
    .function(U"One", &Point::One)
    .function(U"AlU", &Point::All)
    .function(U"UnitX", &Point::UnitX)
    .function(U"UnitY", &Point::UnitY)
    .function(U"Left", &Point::Left)
    .function(U"Right", &Point::Right)
    .function(U"Up", &Point::Up)
    .function(U"Down", &Point::Down);

  GlobalScript
    .setClass(U"Point", point)
    .setClass(U"Size", point);
}
