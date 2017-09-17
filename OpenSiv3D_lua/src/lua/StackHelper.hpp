#pragma once
#include <sol.hpp>

//sol内部でstd::unordered_map やstd::vectorの特殊化がされており、ArrayやHashMapもsolの特殊化に引っかかるため、ヘルパを経由する
namespace s3d {
  namespace Lua {
    namespace detail {
      //ヘルパクラス
      template<class T>
      struct SpecializationHelper {
        const T& value;
      };

      //ヘルパクラスを生成する関数
      template<typename T>
      const T& makeHelper(const T& value) {
        return value;
      }

      template<typename T, typename Alloc>
      detail::SpecializationHelper<Array<T, Alloc>> makeHelper(const Array<T, Alloc>& arr) {
        return { arr };
      }

      template<class Key, class T, class Hash, class KeyEqual, class Alloc, unsigned int NeighborhoodSize, bool StoreHash, class GrowthPolicy>
      detail::SpecializationHelper<HashMap<Key, T, Hash, KeyEqual, Alloc, NeighborhoodSize, StoreHash, GrowthPolicy>> makeHelper(const HashMap<Key, T, Hash, KeyEqual, Alloc, NeighborhoodSize, StoreHash, GrowthPolicy>& map) {
        return { map };
      }

      template<typename T, typename Alloc>
      detail::SpecializationHelper<Grid<T, Alloc>> makeHelper(const Grid<T, Alloc>& arr) {
        return { arr };
      }

      //sol::tableからArrayへ変換する関数
      template<typename T, typename Alloc>
      void tableGetter(Array<T, Alloc>& arr, const sol::table& table) {
        arr.resize(table.size());
        //arr[i]がArray<T>の時に、再帰的に処理を行うため、tableの型がテーブルの時はテーブルとして、それ以外の時はobjectとして再帰
        for (auto&& i : step(arr.size())) {
          if (table[i + 1].get_type() == sol::type::table) {
            tableGetter(arr[i], table[i + 1].get<sol::table>());
          }
          else {
            tableGetter(arr[i], table[i + 1].get<sol::object>());
          }
        }
      }

      //sol::tableからArrayへ変換する関数
      template<typename T, typename Alloc>
      void tableGetter(Array<T, Alloc>& arr, const sol::object& obj) {
        arr.emplace_back(obj.as<T>());
      }

      //sol::tableからGridへ変換する関数
      template<typename T, typename Alloc>
      void tableGetter(Grid<T, Alloc>& arr, const sol::table& table) {
        //Gridの高さはテーブルの高さ、幅はテーブルの1番目の型がテーブルの時はその大きさ、それ以外なら1とする
        std::size_t h = table.size();
        std::size_t w = (table[1].get_type() == sol::type::table ? table[1].get<sol::table>().size() : 1);
        arr.resize(w, h, T{});
        for (auto&& y : step(h)) {
          auto line = table[y + 1];
          //テーブル内部が配列なら、そのままループで、配列以外ならそのまま代入する
          if (line.get_type() == sol::type::table) {
            auto line_table = line.get<sol::table>();
            for (auto&& x : step(Min(w, line_table.size()))) {
              //arr[y][x]が配列の時は、再帰的に処理を行うため、tableの型で分岐する
              if (line_table[x + 1].get_type() == sol::type::table) {
                tableGetter(arr[y][x], line_table[x + 1].get<sol::table>());
              }
              else {
                tableGetter(arr[y][x], line_table[x + 1].get<sol::object>());
              }
            }
          }
          else {
            tableGetter(arr[y][0], line.get<sol::object>());
          }
        }
      }

      //sol::tableからHashMapへ変換する関数
      template<class Key, class T, class Hash, class KeyEqual, class Alloc, unsigned int NeighborhoodSize, bool StoreHash, class GrowthPolicy>
      void tableGetter(HashMap<Key, T, Hash, KeyEqual, Alloc, NeighborhoodSize, StoreHash, GrowthPolicy>& map, const sol::table& table) {
        map.reserve(table.size());
        Key key;
        T value;
        for (auto&& pair : table) {
          tableGetter(key, pair.first);
          //valueが配列の時は、再帰的に処理を行うため、テーブルの内部がテーブルかどうかで分岐する
          if (pair.second.get_type() == sol::type::table) {
            tableGetter(value, pair.second.as<sol::table>());
          }
          else {
            tableGetter(value, pair.second);
          }
          map[key] = value;
        }
      }

      //valueに値を代入する再帰関数の末尾
      template<typename T>
      void tableGetter(T& value, const sol::object& obj) {
        value = obj.as<T>();
      }

      //HashMapからtableへ変換する関数
      template<class Key, class T, class Hash, class KeyEqual, class Alloc, unsigned int NeighborhoodSize, bool StoreHash, class GrowthPolicy, typename Table>
      void tableSetter(lua_State* l, const HashMap<Key, T, Hash, KeyEqual, Alloc, NeighborhoodSize, StoreHash, GrowthPolicy>& map, Table& table) {
        table = sol::table(l, sol::new_table{ 0 });
        for (auto&& pair : map) {
          auto proxy = table[pair.first];
          //内部の型が配列でも変換できるように再帰する
          tableSetter(l, pair.second, proxy);
        }
      }

      //Arrayからtableへ変換する関数
      template<typename T, typename Alloc, typename Table>
      void tableSetter(lua_State* l, const Array<T, Alloc>& arr, Table& table) {
        table = sol::table(l, sol::new_table{ static_cast<int>(arr.size()) });
        for (auto&& i : step(arr.size())) {
          //内部の型が配列でも変換できるように、ヘルパ関数を通してテーブルに追加
          table.add(Lua::detail::makeHelper(arr[i]));
        }
      }

      template<typename T, typename Proxy>
      void tableSetter(lua_State* l, const T& value, Proxy& proxy) {
        proxy = value;
      }

      //Gridからtableへ変換する関数
      template<typename T, typename Alloc, typename Table>
      void tableSetter(lua_State* l, const Grid<T, Alloc>& arr, Table& table) {
        table = sol::table(l, sol::new_table{ static_cast<int>(arr.height()) });
        for (auto&& y : step(arr.height())) {
          table[y + 1] = sol::table(l, sol::new_table{ static_cast<int>(arr.width()) });
          for (auto&& x : step(arr.width())) {
            //内部の型が配列でも変換できるように再帰する
            tableSetter(l, arr[y][x], table[y + 1][x + 1]);
          }
        }
      }

      //luaのスタックがT型かどうか確認する関数
      template<typename T, typename Handler>
      inline bool checkStack(lua_State* L, int index, Handler&& handler, sol::stack::record& tracking, int stackSize) {
        //負の値のインデックスなどを処理するため。luaの関数を呼び出す
        int absolute_index = lua_absindex(L, index);
        //スタックの指定されたインデックスがT型かどうか確認
        bool success = sol::stack::check<T>(L, absolute_index, handler);
        //スタックから取得した量を記録
        tracking.use(stackSize);
        return success;
      }

      //luaのスタックへヘルパクラスの値を積む関数
      template<typename T>
      inline int pushStack(lua_State* l, const SpecializationHelper<T>& helper) {
        //Tからsol::tableへの変換
        const auto& value = helper.value;
        sol::table table;
        Lua::detail::tableSetter(l, value, table);
        //sol::tableとしてスタックにプッシュ
        return sol::stack::push(l, table);
      }

      //luaのスタックへ値を積む関数
      template<typename T>
      inline int pushStack(lua_State* l, const T& value) {
        return sol::stack::push(l, value);
      }

      //luaのスタックから値を取得する関数
      template<typename T>
      inline auto getStack(lua_State* l, int index, sol::stack::record& tracking, int stackSize) {
        int absolute_index = lua_absindex(l, index);
        //スタックの指定された位置をT型として取得
        auto ret = sol::stack::get<T>(l, absolute_index);
        //スタックから取得した量を記録
        tracking.use(stackSize);
        return ret;
      }
    }
  }
}