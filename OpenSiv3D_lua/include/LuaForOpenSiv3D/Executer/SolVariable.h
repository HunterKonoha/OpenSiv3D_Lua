#pragma once

namespace s3d::LuaScript {
	template<typename valType>
	class SolVariable {
		//private:
		public: // for debug
			valType m_variable;

		public:
			SolVariable() = delete;
			SolVariable(const valType& variable) :m_variable(variable) {}
	};
}