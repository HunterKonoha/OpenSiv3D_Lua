﻿#include <Siv3D.hpp>
#include <HamFramework.hpp>

#include <LuaScript.h>
#include <sol/sol.hpp>
#include <LuaForOpenSiv3D/InternalStrings.h>
#include <LuaForOpenSiv3D/Environment/GlobalEnvironment.h>
#include <tuple>

struct A {
	static int B(int a) {
		return a * 2;
	}
};

void outputSolType(std::reference_wrapper<sol::protected_function_result> result) {

	std::cout << "begin" << std::endl;
	for (int i = 0; i < 5; ++i) {
		auto type = result.get().get_type(i);
		switch (type)
		{
		case sol::type::none:
			std::cout << "\tnone" << std::endl;
			break;

		case sol::type::string:
			std::cout << "\tstring(" << result.get().get<std::string>(i) << ")" << std::endl;
			break;

		case sol::type::number:
			std::cout << "\tnumber(" << result.get().get<double>(i) << ")" << std::endl;
			break;

		default:
			std::cout << "\tother(" << std::endl;
			break;
		}
	}
	std::cout << "end" << std::endl;
}

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
	LuaScript::ExecuteResult v0 = LuaScript::GlobalEnvironment.executeFromString(U"return 123");
	int v0_val = v0;
	LuaScript::ExecuteResult v01 = LuaScript::GlobalEnvironment.executeFromString(U"return 123, 0.0, 'test'");
	auto v01_type0 = v01.getValueType();
	auto v01_type1 = v01.getValueType(1);
	auto v01_type2 = v01.getValueType(2);
	auto tuple_000 = v01.m_execute_result.get<int>(0);
	auto tuple_00 = v01.getValue<int>(0);
	auto tuple_00_str = v01.getValue<std::optional<std::string>>(0);
	auto tuple_01_str = v01.getValueOr<std::string>("Not value", 0);
	auto tuple_0 = v01.getValue<int>(0);
	auto tuple_01 = v01.getValue<double>(1);
	auto tuple_02 = v01.getValue<std::string>(2);
	// auto tuple_03 = v01.getValue<std::string>(3); // error OK
	auto tuple_1 = v01.getValue<std::tuple<int, double>>(0);
	auto tuple_2 = v01.getValue<std::tuple<double, std::string>>(1);
	// auto tuple_3 = v01.getValue<std::tuple<std::string, double>>(2); // error OK
	int v01_value_count = v01.getValueCount();
	outputSolType(v01.m_execute_result);
	LuaScript::ExecuteResult file_result = LuaScript::GlobalEnvironment.executeFromFile(U"script/lua_test_3.lua");
	auto type_10 = file_result.m_execute_result.get_type();
	int stack_index0 = file_result.m_execute_result.stack_index();
	outputSolType(file_result.m_execute_result);
	sol::protected_function_result func0 = LuaScript::GlobalEnvironment.m_state["Execute"].call();
	outputSolType(file_result.m_execute_result);
	outputSolType(func0);
	sol::protected_function_result func1 = LuaScript::GlobalEnvironment.m_state["Execute2"].call();
	outputSolType(func0);
	auto type_11 = file_result.m_execute_result.get_type();
	outputSolType(file_result.m_execute_result);

	LuaScript::ExecuteResult file_result2 = LuaScript::GlobalEnvironment.executeFromFile(U"script/lua_test_4.lua");
	outputSolType(file_result2.m_execute_result);

	//std::string func_result0 = func0;
	auto type_0 = v01.m_execute_result.get_type();
	auto type_1 = file_result.m_execute_result.get_type();
	int str_result = file_result.m_execute_result;
	std::string str_result11 = func0;
	std::string str_result12 = func1;
	int str_result2 = file_result2.m_execute_result;
	//auto func = file_result.m_execute_result["Execute"];

	// scriptの第三引数でenvを指定可能．指定しないとsol::stateで実行されて，globalsで構築されたenvに影響を与える

	auto tmp_result_1 = state.script("return 123", env);
	auto tmp_result_2 = state.script("return 456", env);
	int tmp_result_val_1 = tmp_result_1;
	int tmp_result_val_2 = tmp_result_2;


	auto result_1 = state.script("a = A.B(3)", env);
	auto result_4 = state.script("a = A.B(4)", env);
	sol::call_status valid_1 = result_1.status();
	int v1 = env["a"];
	// globalsでenvを構築していれば，stateにロードされている関数・変数が使える
	// globalsで構築していても，他のenvの結果は取得できない
	auto result_2 = state.script("b = A.B(5)", env2);
	auto v3 = env2["b0"];
	//env2.
	
	int v2 = env2["a0"].get_or(0);

	if (!v3.valid()) {
		auto type_2 = v3.get_type();
		//v3.
		std::string err = v3.operator std::string();
		
		//std::string msg = err.what();
		int tmp= 1;
	}




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
