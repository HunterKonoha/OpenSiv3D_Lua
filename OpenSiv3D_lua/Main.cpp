# include <Siv3D.hpp>
# include <HamFramework.hpp>

#include "src/lua.hpp"

void PrintPoint(const Point& p) {
  Print << p;
}

void PrintString(String v) {
  Print << v;
}

void PrintBool(bool v) {
  Print << v;
}

template<typename T>
void Print_impl(T v) {
  Print << v;
}

struct Test {
  static void TestFunc() {
    Print << L"TestFunc";
  }
};

void Main() {
  Console;
  Lua::Script script_1, script_2;

  String str = L"Before";
  

  // script_1.setClass<Point>(L"Point", sol::constructors<sol::types<>, sol::types<int, int>>());
  Lua::GlobalScript
    .setFunction(L"Print", sol::overload(PrintPoint, PrintString, PrintBool, Print_impl<double>))
    .setValue(L"str", &str);

  // script_1.setFunction(L"Print", sol::overload(PrintPoint, PrintString, PrintBool, Print_impl<double>));
  // script_1.setValue(L"str", &str);
  //TODO:luaの内部環境テーブルを共有ではなく、1インスタンスに1つにする,アセット管理で共有する

  Lua::GlobalScript.openLibraries();

  Lua::GlobalScript.setClass<Test>(L"Test").function(L"TestFunc", &Test::TestFunc);

  Lua::GlobalScript.setClass(L"Circle",
    Lua::ClassSetter<Circle>()
    .constructor<Circle(double, double, double),Circle(const Point&,double)>()
    .value(L"x", &Circle::x)
    .value(L"y", &Circle::y)
    .value(L"r", &Circle::r)
    .value(L"center", &Circle::center)
    .function(L"leftClicked", &Circle::leftClicked)
    .function(L"rightClicked", &Circle::rightClicked)
    .function(L"mouseOver", &Circle::mouseOver)
    .function(L"draw", &Circle::draw));

  Lua::GlobalScript
    .setClass<RectF>(L"RectF");

  script_1
    .setFunction(L"RandomPoint", sol::resolve<Point(int32, int32)>(&RandomPoint))
    .setFunction(L"RandomVec2", sol::resolve<Vec2(double, double)>(&RandomVec2));

  script_1
    .setClass<Color>(L"Color")
    .constructor<Color(const Color&), Color(), Color(Color&&),Color(double, double, double)>();

  script_1
    .setClass<ColorF>(L"ColorF")
    .constructor<ColorF(const ColorF&), ColorF(), ColorF(ColorF&&), ColorF(double, double, double), ColorF(double, double, double,double), ColorF(double)>();

  script_1
    .setNamespace(L"Palette")
    .setValue(L"White", Palette::White)
    .setValue(L"Red", Palette::Red)
    .setValue(L"Black", Palette::Black);

  script_1.setNamespace(L"Typeface")
    .setValue(L"Default", Typeface::Default);

  script_1.setNamespace(L"FontStyle")
    .setValue(L"Default", FontStyle::Default);
  script_1.loadFromFile(L"lua_test_1.lua");

  Lua::Class test_class = script_1.createClass(L"TestClass", 10, 20);

  test_class = script_1.getValue<Lua::Class>(L"testclass");
  
  Console << test_class.getFunction<String()>(L"getString")();
  Console << test_class.callFunction<int>(L"getInt");
  Console << test_class.getFunction<int(int)>(L"getValue")(1);
  Console << test_class.callFunction<int>(L"getValue", 2);
  test_class.getFunction<void(void)>(L"empty")();

  for (auto&& i : step(10)) {
    Console << test_class.callCoroutine<String>(L"coroutine", 1);
  }

  for (auto&& i : step(10)) {
    Console << test_class.getCoroutine<String(int)>(L"coroutine")(1);
  }
  

  auto mainfunc = script_1.getFunction<void(void)>(L"main");
  mainfunc();
}