#pragma once
#include <sol.hpp>

//sol������std::unordered_map ��std::vector�̓��ꉻ������Ă���AArray��HashMap��sol�̓��ꉻ�Ɉ��������邽�߁A�w���p���o�R����
namespace s3d {
  namespace Lua {
    namespace detail {
      //�w���p�N���X
      template<class T>
      struct SpecializationHelper {
        const T& value;
      };

      //�w���p�N���X�𐶐�����֐�
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

      //sol::table����Array�֕ϊ�����֐�
      template<typename T, typename Alloc>
      void tableGetter(Array<T, Alloc>& arr, const sol::table& table) {
        arr.resize(table.size());
        //arr[i]��Array<T>�̎��ɁA�ċA�I�ɏ������s�����߁Atable�̌^���e�[�u���̎��̓e�[�u���Ƃ��āA����ȊO�̎���object�Ƃ��čċA
        for (auto&& i : step(arr.size())) {
          if (table[i + 1].get_type() == sol::type::table) {
            tableGetter(arr[i], table[i + 1].get<sol::table>());
          }
          else {
            tableGetter(arr[i], table[i + 1].get<sol::object>());
          }
        }
      }

      //sol::table����Array�֕ϊ�����֐�
      template<typename T, typename Alloc>
      void tableGetter(Array<T, Alloc>& arr, const sol::object& obj) {
        arr.emplace_back(obj.as<T>());
      }

      //sol::table����Grid�֕ϊ�����֐�
      template<typename T, typename Alloc>
      void tableGetter(Grid<T, Alloc>& arr, const sol::table& table) {
        //Grid�̍����̓e�[�u���̍����A���̓e�[�u����1�Ԗڂ̌^���e�[�u���̎��͂��̑傫���A����ȊO�Ȃ�1�Ƃ���
        std::size_t h = table.size();
        std::size_t w = (table[1].get_type() == sol::type::table ? table[1].get<sol::table>().size() : 1);
        arr.resize(w, h, T{});
        for (auto&& y : step(h)) {
          auto line = table[y + 1];
          //�e�[�u���������z��Ȃ�A���̂܂܃��[�v�ŁA�z��ȊO�Ȃ炻�̂܂ܑ������
          if (line.get_type() == sol::type::table) {
            auto line_table = line.get<sol::table>();
            for (auto&& x : step(Min(w, line_table.size()))) {
              //arr[y][x]���z��̎��́A�ċA�I�ɏ������s�����߁Atable�̌^�ŕ��򂷂�
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

      //sol::table����HashMap�֕ϊ�����֐�
      template<class Key, class T, class Hash, class KeyEqual, class Alloc, unsigned int NeighborhoodSize, bool StoreHash, class GrowthPolicy>
      void tableGetter(HashMap<Key, T, Hash, KeyEqual, Alloc, NeighborhoodSize, StoreHash, GrowthPolicy>& map, const sol::table& table) {
        map.reserve(table.size());
        Key key;
        T value;
        for (auto&& pair : table) {
          tableGetter(key, pair.first);
          //value���z��̎��́A�ċA�I�ɏ������s�����߁A�e�[�u���̓������e�[�u�����ǂ����ŕ��򂷂�
          if (pair.second.get_type() == sol::type::table) {
            tableGetter(value, pair.second.as<sol::table>());
          }
          else {
            tableGetter(value, pair.second);
          }
          map[key] = value;
        }
      }

      //value�ɒl��������ċA�֐��̖���
      template<typename T>
      void tableGetter(T& value, const sol::object& obj) {
        value = obj.as<T>();
      }

      //HashMap����table�֕ϊ�����֐�
      template<class Key, class T, class Hash, class KeyEqual, class Alloc, unsigned int NeighborhoodSize, bool StoreHash, class GrowthPolicy, typename Table>
      void tableSetter(lua_State* l, const HashMap<Key, T, Hash, KeyEqual, Alloc, NeighborhoodSize, StoreHash, GrowthPolicy>& map, Table& table) {
        table = sol::table(l, sol::new_table{ 0 });
        for (auto&& pair : map) {
          auto proxy = table[pair.first];
          //�����̌^���z��ł��ϊ��ł���悤�ɍċA����
          tableSetter(l, pair.second, proxy);
        }
      }

      //Array����table�֕ϊ�����֐�
      template<typename T, typename Alloc, typename Table>
      void tableSetter(lua_State* l, const Array<T, Alloc>& arr, Table& table) {
        table = sol::table(l, sol::new_table{ static_cast<int>(arr.size()) });
        for (auto&& i : step(arr.size())) {
          //�����̌^���z��ł��ϊ��ł���悤�ɁA�w���p�֐���ʂ��ăe�[�u���ɒǉ�
          table.add(Lua::detail::makeHelper(arr[i]));
        }
      }

      template<typename T, typename Proxy>
      void tableSetter(lua_State* l, const T& value, Proxy& proxy) {
        proxy = value;
      }

      //Grid����table�֕ϊ�����֐�
      template<typename T, typename Alloc, typename Table>
      void tableSetter(lua_State* l, const Grid<T, Alloc>& arr, Table& table) {
        table = sol::table(l, sol::new_table{ static_cast<int>(arr.height()) });
        for (auto&& y : step(arr.height())) {
          table[y + 1] = sol::table(l, sol::new_table{ static_cast<int>(arr.width()) });
          for (auto&& x : step(arr.width())) {
            //�����̌^���z��ł��ϊ��ł���悤�ɍċA����
            tableSetter(l, arr[y][x], table[y + 1][x + 1]);
          }
        }
      }

      //lua�̃X�^�b�N��T�^���ǂ����m�F����֐�
      template<typename T, typename Handler>
      inline bool checkStack(lua_State* L, int index, Handler&& handler, sol::stack::record& tracking, int stackSize) {
        //���̒l�̃C���f�b�N�X�Ȃǂ��������邽�߁Blua�̊֐����Ăяo��
        int absolute_index = lua_absindex(L, index);
        //�X�^�b�N�̎w�肳�ꂽ�C���f�b�N�X��T�^���ǂ����m�F
        bool success = sol::stack::check<T>(L, absolute_index, handler);
        //�X�^�b�N����擾�����ʂ��L�^
        tracking.use(stackSize);
        return success;
      }

      //lua�̃X�^�b�N�փw���p�N���X�̒l��ςފ֐�
      template<typename T>
      inline int pushStack(lua_State* l, const SpecializationHelper<T>& helper) {
        //T����sol::table�ւ̕ϊ�
        const auto& value = helper.value;
        sol::table table;
        Lua::detail::tableSetter(l, value, table);
        //sol::table�Ƃ��ăX�^�b�N�Ƀv�b�V��
        return sol::stack::push(l, table);
      }

      //lua�̃X�^�b�N�֒l��ςފ֐�
      template<typename T>
      inline int pushStack(lua_State* l, const T& value) {
        return sol::stack::push(l, value);
      }

      //lua�̃X�^�b�N����l���擾����֐�
      template<typename T>
      inline auto getStack(lua_State* l, int index, sol::stack::record& tracking, int stackSize) {
        int absolute_index = lua_absindex(l, index);
        //�X�^�b�N�̎w�肳�ꂽ�ʒu��T�^�Ƃ��Ď擾
        auto ret = sol::stack::get<T>(l, absolute_index);
        //�X�^�b�N����擾�����ʂ��L�^
        tracking.use(stackSize);
        return ret;
      }
    }
  }
}