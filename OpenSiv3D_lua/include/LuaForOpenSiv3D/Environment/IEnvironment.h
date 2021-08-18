#pragma once
#include <LuaForOpenSiv3D/InternalStrings.h>
#include <string>

namespace s3d::LuaScript {
	class IEnvironment {
		private:
		public:
			//virtual void executeFromFile(const Internal::String& file_name) = 0;

		protected:
			template<typename ScriptType>
			void ExecuteFromFile(const Internal::String& file_name, ScriptType& script);
	};
}


#include "detail/IEnvironment.ipp"