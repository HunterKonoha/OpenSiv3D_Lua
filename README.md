# OpenSiv3D_Lua
OpenSiv3Dのlua実装

# 使い方
1. クローンする
2. [SourceForgeのLuaBinaries](https://sourceforge.net/projects/luabinaries/files/?source=navbar)から、Luaの5.3.xのWindowsのstatic libraryをDLする
3. [Sol2](https://github.com/ThePhD/sol2)から、sol2のヘッダをDLする
4. sol.hpp内部のconstexpr_move関数の呼び出し部分を、sol::constexpr_move()のように名前空間を追加する
5. プロジェクトのインクルードディレクトリを設定する
6. ビルドする
