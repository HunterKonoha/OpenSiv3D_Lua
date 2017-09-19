#include "PointBind.hpp"
#include "../GlobalScript.hpp"
#include <Siv3D.hpp>

void s3d::Lua::Binding::PointBind() {
  ClassSetter<Point> point;
  point.value(L"x", &Point::x)
    .value(L"y", &Point::y)
    .constructor<sol::types<>, sol::types<const Point&>, sol::types<int32, int32>>()
    .function(L"xx", &Point::xx)
    .function(L"xy", &Point::xy)
    .function(L"yx", &Point::yx)
    .function(L"yy", &Point::yy)
    .metaFunction(sol::meta_function::unary_minus, (Point(Point::*)()const)(&Point::operator-))
    .metaFunction(sol::meta_function::addition, sol::overload((Point(Point::*)(const Point&)const)(&Point::operator+), &Point::operator+<double>, &Point::operator+<float>))
    .metaFunction(sol::meta_function::subtraction, sol::overload((Point(Point::*)(const Point&)const)(&Point::operator-), &Point::operator-<double>, &Point::operator-<float>))
    .metaFunction(sol::meta_function::multiplication, sol::overload((Point(Point::*)(int32)const)(&Point::operator*), (Float2(Point::*)(float)const)(&Point::operator*), (Vec2(Point::*)(double)const)(&Point::operator*), (Point(Point::*)(const Point&)const)(&Point::operator*), &Point::operator*<double>, &Point::operator*<float>))
    .metaFunction(sol::meta_function::division, sol::overload((Point(Point::*)(int32)const)(&Point::operator/), (Float2(Point::*)(float)const)(&Point::operator/), (Vec2(Point::*)(double)const)(&Point::operator/), (Point(Point::*)(const Point&)const)(&Point::operator/), &Point::operator/<double>, &Point::operator/<float>))
    .metaFunction(sol::meta_function::equal_to, &Point::operator==)
    .metaFunction(sol::meta_function::to_string, [](const Point& p) {return Format(p); })
    .function(L"set", sol::overload((Point&(Point::*)(const int32, const int32))(&Point::set), (Point&(Point::*)(const Point&))(&Point::set)))
    .function(L"movedBy", sol::overload((Point(Point::*)(const int32, const int32)const)(&Point::movedBy), (Point(Point::*)(const Point&)const)(&Point::movedBy), &Point::movedBy<double>, &Point::movedBy<float>))
    .function(L"moveBy", sol::overload((Point&(Point::*)(const int32, const int32))(&Point::moveBy), (Point&(Point::*)(const Point&))(&Point::moveBy)))
    .function(L"isZero", &Point::isZero)
    .function(L"distanceFrom", sol::overload((double(Point::*)(const Point&)const)(&Point::distanceFrom), &Point::distanceFrom<double>, &Point::distanceFrom<float>))
    .function(L"distanceFromSq", sol::overload((double(Point::*)(const Point&)const)(&Point::distanceFromSq), &Point::distanceFromSq<double>, &Point::distanceFromSq<float>))
    .function(L"length", &Point::length<double>)
    .function(L"lengthSq", &Point::lengthSq<double>)
    .function(L"lerp", sol::overload(&Point::lerp<double>, &Point::lerp<float>))
    .function(L"intersects", sol::overload(&Point::intersects<Point>,
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
    .function(L"leftClicked", &Point::leftClicked)
    .function(L"leftPressed", &Point::leftPressed)
    .function(L"leftReleased", &Point::leftReleased)
    .function(L"rightClicked", &Point::rightClicked)
    .function(L"rightPressed", &Point::rightPressed)
    .function(L"rightReleased", &Point::rightReleased)
    .function(L"mouseOver", &Point::mouseOver)
    .function(L"Zero", &Point::Zero)
    .function(L"One", &Point::One)
    .function(L"All", &Point::All)
    .function(L"UnitX", &Point::UnitX)
    .function(L"UnitY", &Point::UnitY)
    .function(L"Left", &Point::Left)
    .function(L"Right", &Point::Right)
    .function(L"Up", &Point::Up)
    .function(L"Down", &Point::Down);

  GlobalScript
    .setClass(L"Point", point)
    .setClass(L"Size", point);
}
