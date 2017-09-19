#pragma once
#include <sol.hpp>
#include <Siv3D.hpp>
#include "StackHelper.hpp"

namespace sol {
  //スタックを専有するサイズを指定
  template <>
  struct lua_size<String> : lua_size<std::string> {};

  template<typename T, typename Alloc>
  struct lua_size<Array<T, Alloc>> : std::integral_constant<int, 1> {};

  template<class Key, class T, class Hash, class KeyEqual, class Alloc, unsigned int NeighborhoodSize, bool StoreHash, class GrowthPolicy>
  struct lua_size<HashMap<Key, T, Hash, KeyEqual, Alloc, NeighborhoodSize, StoreHash, GrowthPolicy>> : std::integral_constant<int, 1> {};

  template<typename T, typename Alloc>
  struct lua_size<Grid<T, Alloc>> : std::integral_constant<int, 1> {};

  //sol側でどのような型なのかを指定
  template <>
  struct lua_type_of<String> : lua_type_of<std::string> {};

  template<typename T, typename Alloc>
  struct lua_type_of<Array<T, Alloc>> : std::integral_constant<sol::type, sol::type::table> {};

  template<class Key, class T, class Hash, class KeyEqual, class Alloc, unsigned int NeighborhoodSize, bool StoreHash, class GrowthPolicy>
  struct lua_type_of<HashMap<Key, T, Hash, KeyEqual, Alloc, NeighborhoodSize, StoreHash, GrowthPolicy>> : std::integral_constant<sol::type, sol::type::table> {};

  template<typename T, typename Alloc>
  struct lua_type_of<Grid<T, Alloc>> : std::integral_constant<sol::type, sol::type::table> {};


  //スタックの出し入れをする際の特殊化
  namespace stack {
    //変換の際のスタックの変数の型チェック関数
    template <>
    struct checker<String> {
      template <typename Handler>
      static bool check(lua_State* L, int index, Handler&& handler, record& tracking) {
        return  Lua::detail::checkStack<std::string>(L, index, handler, tracking, 1);
      }
    };

    template <typename T, typename Alloc>
    struct checker<Array<T, Alloc>, sol::type::table> {
      template <typename Handler>
      static bool check(lua_State* L, int index, Handler&& handler, record& tracking) {
        return Lua::detail::checkStack<sol::table>(L, index, handler, tracking, 1);
      }
    };

    template<class Key, class T, class Hash, class KeyEqual, class Alloc, unsigned int NeighborhoodSize, bool StoreHash, class GrowthPolicy>
    struct checker<HashMap<Key, T, Hash, KeyEqual, Alloc, NeighborhoodSize, StoreHash, GrowthPolicy>, sol::type::table> {
      template <typename Handler>
      static bool check(lua_State* L, int index, Handler&& handler, record& tracking) {
        return Lua::detail::checkStack<sol::table>(L, index, hanlder, tracking, 1);
      }
    };

    template <typename T, typename Alloc>
    struct checker<Grid<T, Alloc>, sol::type::table> {
      template <typename Handler>
      static bool check(lua_State* L, int index, Handler&& handler, record& tracking) {
        return Lua::detail::checkStack<sol::table>(L, index, hanlder, tracking, 1);
      }
    };

    //スタックからある型へ変換する関数
    template <>
    struct getter<String> {
      static String get(lua_State* L, int index, record& tracking) {
        auto obj = Lua::detail::getStack<sol::userdata>(L, index, tracking, 1);
        if (obj.is<String>()) {
          return CharacterSet::FromUTF8(obj.as<std::string>());
        }
        else {
          auto to_string = obj[sol::meta_function::to_string];
          if (to_string.valid()) {
            return CharacterSet::FromUTF8(obj[sol::meta_function::to_string](obj).get<std::string>());
          }
          else {
            return CharacterSet::FromUTF8(obj.as<std::string>());
          }
        }
      }
    };

    template<>
    struct getter<ColorF> {
      static ColorF get(lua_State* L, int index, record& tracking) {
        sol::userdata obj = Lua::detail::getStack<sol::userdata>(L, index, tracking, 1);
        if (obj.is<Color>()) {
          return ColorF(*obj.as<Color*>());
        }
        else {
          return *obj.as<ColorF*>();
        }
      }
    };

    template<>
    struct getter<Color> {
      static Color get(lua_State* L, int index, record& tracking) {
        sol::userdata obj = Lua::detail::getStack<sol::userdata>(L, index, tracking, 1);
        if (obj.is<Color>()) {
          return *obj.as<Color*>();
        }
        else {
          return obj.as<ColorF*>()->toColor();
        }
      }
    };

    template<>
    struct getter<Point> {
      static Point get(lua_State* L, int index, record& tracking) {
        auto obj = Lua::detail::getStack<sol::userdata>(L, index, tracking, 1);
        if (obj.is<Point>()) {
          return *obj.as<Point*>();
        }
        else {
          return obj.as<Vec2*>()->asPoint();
        }
      }
    };

    template<>
    struct getter<Vec2> {
      static Vec2 get(lua_State* L, int index, record& tracking) {
        auto obj = Lua::detail::getStack<sol::userdata>(L, index, tracking, 1);
        if (obj.is<Point>()) {
          return { *obj.as<Point*>() };
        }
        else {
          return *obj.as<Vec2*>();
        }
      }
    };

    template <typename T, typename Alloc>
    struct getter<Array<T, Alloc>> {
      static Array<T, Alloc> get(lua_State* L, int index, record& tracking) {
        //sol::tableからArrayへの変換
        Array<T, Alloc> ret;
        auto table = Lua::detail::getStack<sol::table>(L, index, tracking, 1);
        Lua::detail::tableGetter(ret, table);
        return ret;
      }
    };

    template<class Key, class T, class Hash, class KeyEqual, class Alloc, unsigned int NeighborhoodSize, bool StoreHash, class GrowthPolicy>
    struct getter<HashMap<Key, T, Hash, KeyEqual, Alloc, NeighborhoodSize, StoreHash, GrowthPolicy>> {
      static HashMap<Key, T, Hash, KeyEqual, Alloc, NeighborhoodSize, StoreHash, GrowthPolicy> get(lua_State* L, int index, record& tracking) {
        //sol::tableからHashMapへの変換
        HashMap<Key, T, Hash, KeyEqual, Alloc, NeighborhoodSize, StoreHash, GrowthPolicy> ret;
        auto table = Lua::detail::getStack<sol::table>(L, index, tracking, 1);
        Lua::detail::tableGetter(ret, table);
        return ret;
      }
    };

    template <typename T, typename Alloc>
    struct getter<Grid<T, Alloc>> {
      static Grid<T, Alloc> get(lua_State* L, int index, record& tracking) {
        //sol::tableからArrayへの変換
        Grid<T, Alloc> ret;
        auto table = Lua::detail::getStack<sol::table>(L, index, tracking, 1);
        Lua::detail::tableGetter(ret, table);
        return ret;
      }
    };

    //ある型をスタックへ積む関数
    template <>
    struct pusher<String> {
      static int push(lua_State* L, const String& string) {
        return Lua::detail::pushStack(L, CharacterSet::ToUTF8(string));
      }
    };

    template <class T, class Alloc>
    struct pusher<Lua::detail::SpecializationHelper<Array<T, Alloc>>> {
      static int push(lua_State* L, const Lua::detail::SpecializationHelper<Array<T, Alloc>>& array) {
        return Lua::detail::pushStack(L, array);
      }
    };

    template<class Key, class T, class Hash, class KeyEqual, class Alloc, unsigned int NeighborhoodSize, bool StoreHash, class GrowthPolicy>
    struct pusher<Lua::detail::SpecializationHelper<HashMap<Key, T, Hash, KeyEqual, Alloc, NeighborhoodSize, StoreHash, GrowthPolicy>>> {
      static int push(lua_State* L, const Lua::detail::SpecializationHelper<HashMap<Key, T, Hash, KeyEqual, Alloc, NeighborhoodSize, StoreHash, GrowthPolicy>>& hashMap) {
        return Lua::detail::pushStack(L, hashMap);
      }
    };

    template <class T, class Alloc>
    struct pusher<Lua::detail::SpecializationHelper<Grid<T, Alloc>>> {
      static int push(lua_State* L, const Lua::detail::SpecializationHelper<Grid<T, Alloc>>& array) {
        return Lua::detail::pushStack(L, array);
      }
    };
  }
}