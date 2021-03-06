#ifndef ENJON_CVARS_SYSTEM_H
#define ENJON_CVARS_SYSTEM_H
#pragma once

#include <unordered_map>
#include <vector>
#include <string>

#define CVAR_REGISTER(key, val, type) 		CVarsSystem::Register(key, val, type)
#define CVAR_SET(key, val) 					CVarsSystem::Set(key, val)
#define CVAR_GET_REGISTERED()				CVarsSystem::GetRegisteredCommands()	

namespace Enjon
{
	enum CVarType
	{
		TYPE_BOOL,
		TYPE_FLOAT,
		TYPE_INT,
		TYPE_UINT
	};

	struct CVarBase
	{
		virtual void Init() = 0;
		CVarType mType;
	};

	template <typename T>
	struct CVar : public CVarBase
	{
		CVar(T* val, CVarType type = CVarType::TYPE_FLOAT) : value(val) 
		{
			mType = type;
		}

		void Init(){}
		T* value;
	};

	class CVarsSystem
	{
		public: 
			template <typename T>
			static bool Register(std::string key, T* val, CVarType type);

			template <typename T>
			static bool Set(std::string key, T val);

			static std::vector<std::string> GetRegisteredCommands();

		public:
			static std::unordered_map<std::string, CVarBase*> mMap;
	};
}

#endif