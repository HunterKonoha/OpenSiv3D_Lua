#include <Siv3D.hpp>
#include <HamFramework.hpp>

#include <LuaScript.h>
#include <sol/sol.hpp>
#include <LuaForOpenSiv3D/InternalStrings.h>
#include <LuaForOpenSiv3D/Environment/GlobalEnvironment.h>

struct A {
	static int B(int a) {
		return a * 2;
	}
};

void Main() {
	Logger.enable();
	Console.open();

	s3d::String str = s3d::LuaScript::Internal::convertString("test string");

	sol::state& state = LuaScript::GlobalEnvironment.m_state;
	state.open_libraries(sol::lib::base);
	sol::environment env(state, sol::create, state.globals()); // state.globals()がないと，sol::stateの変更が反映されない
	sol::environment env2(state, sol::create, state.globals());

	// stateにすると，globalsで指定しているenv全体に反映
	// envにすると，env2やsol::stateには反映されない (globalsで指定していてもsol::stateには影響しない)
	auto a_type = env.new_usertype<A>("A");
	// 下記2つは同じ意味
	//a_type["B"] = &A::B;
	a_type.set_function("B", &A::B);

	auto a_type2 = env2.new_usertype<A>("A");
	a_type2.set_function("B", &A::B);

	LuaScript::GlobalEnvironment.executeFromString(U"a0 = A.B(1)");
	std::optional<int> v0 = LuaScript::GlobalEnvironment.executeFromString<int>(U"return 123");
	auto v01 = LuaScript::GlobalEnvironment.executeFromString<std::tuple<int, double, std::string>>(U"return 123, 0.0, 'test'");

	// scriptの第三引数でenvを指定可能．指定しないとsol::stateで実行されて，globalsで構築されたenvに影響を与える
	auto result_1 = state.script("a = A.B(3)", env);
	sol::call_status valid_1 = result_1.status();
	int v1 = env["a"];
	// globalsでenvを構築していれば，stateにロードされている関数・変数が使える
	// globalsで構築していても，他のenvの結果は取得できない
	auto result_2 = state.script("b = A.B(5)", env2);
	int v3 = env2["b"];
	int v2 = env2["a0"].get_or(0);




	while (s3d::System::Update()) {

	}
	return;
}


//void PrintPoint(const Point& p) {
//  Print << p;
//}
//
//void PrintString(String v) {
//  Print << v;
//}
//
//void PrintBool(bool v) {
//  Print << v;
//}
//
//template<typename T>
//void Print_impl(T v) {
//  Print << v;
//}
//
//struct Test {
//  static void TestFunc() {
//    Print << L"TestFunc";
//  }
//};
//
//void Main() {
//  Console;
//  Lua::Script script_1, script_2;
//
//  String str = L"Before";
//  
//
//  // script_1.setClass<Point>(L"Point", sol::constructors<sol::types<>, sol::types<int, int>>());
//  Lua::GlobalScript
//    .setFunction(L"Print", sol::overload(&PrintPoint, &PrintString, &PrintBool, &Print_impl<double>))
//    .setValue(L"str", &str);
//
//  // script_1.setFunction(L"Print", sol::overload(PrintPoint, PrintString, PrintBool, Print_impl<double>));
//  // script_1.setValue(L"str", &str);
//  //TODO:luaの内部環境テーブルを共有ではなく、1インスタンスに1つにする,アセット管理で共有する
//
//  Lua::GlobalScript.openLibraries();
//
//  Lua::GlobalScript.setClass<Test>(L"Test").function(L"TestFunc", &Test::TestFunc);
//
//  Lua::GlobalScript.setClass(L"Circle",
//    Lua::ClassSetter<Circle>()
//    .constructor<Circle(double, double, double),Circle(const Point&,double)>()
//    .value(L"x", &Circle::x)
//    .value(L"y", &Circle::y)
//    .value(L"r", &Circle::r)
//    .value(L"center", &Circle::center)
//    .function(L"leftClicked", &Circle::leftClicked)
//    .function(L"rightClicked", &Circle::rightClicked)
//    .function(L"mouseOver", &Circle::mouseOver)
//    .function(L"draw", &Circle::draw));
//
//  Lua::GlobalScript
//    .setClass<RectF>(L"RectF");
//
//  script_1
//    .setFunction(L"RandomPoint", sol::resolve<Point(int32, int32)>(&RandomPoint))
//    .setFunction(L"RandomVec2", sol::resolve<Vec2(double, double)>(&RandomVec2));
//
//  script_1
//    .setClass<Color>(L"Color")
//    .constructor<Color(const Color&), Color(), Color(Color&&), Color(double, double, double), Color(double, double, double,double)>()
//    .bitFieldValue<0, 8>(L"r")
//    .bitFieldValue<8, 8>(L"g")
//    .bitFieldValue<16, 8>(L"b")
//    .bitFieldValue<24, 8>(L"a");
//
//  script_1
//    .setClass<ColorF>(L"ColorF")
//    .constructor<ColorF(const ColorF&), ColorF(), ColorF(ColorF&&), ColorF(double, double, double), ColorF(double, double, double,double), ColorF(double)>();
//
//  script_1
//    .setNamespace(L"Palette")
//    .setValue(L"White", Palette::White)
//    .setValue(L"Red", Palette::Red)
//    .setValue(L"Black", Palette::Black);
//
//  script_1.setNamespace(L"Typeface")
//    .setValue(L"Default", Typeface::Default);
//
//  script_1.setNamespace(L"FontStyle")
//    .setValue(L"Default", FontStyle::Default);
//  script_1.loadFromFile(L"lua_test_1.lua");
//
//  Lua::Class p_1 = script_1.createClass(L"Position", 100, 100);
//  Lua::Class p_2 = script_1.createClass(L"Position", 75, 75);
//  Lua::Class o = script_1.createClass(L"Position");
//  Lua::Class p_3 = p_1 + p_2;
//  Lua::Class p_4 = p_1 - p_2;
//  Lua::Class p_5 = p_1 * p_2;
//  Lua::Class p_6 = p_1 / p_2;
//
//  Console << L"原点O:" << o.toString();
//  Console << L"　 P1:" << p_1.toString();
//  Console << L"　 P2:" << p_2.toString();
//  Console << L"　 P3:" << p_3.toString();
//  Console << L"　 P4:" << p_4.toString();
//  Console << L"　 P5:" << p_5.toString();
//  Console << L"　 P6:" << p_6.toString();
//  Console << L"P1_Len:" << p_1.length();
//  Console << L"P1==P2:" << (p_1 == p_2);
//  Console << L"P1<P2:" << (p_1 < p_2);
//  Console << L"P1<=P1:" << (p_1 <= p_1);
//
//  auto mainfunc = script_1.getFunction<void(void)>(L"main");
//  int a = Console.read<int>();
//  script_1.reload();
//  mainfunc();
//}
