#pragma once

#include "EventHandler.h"
#include "TimeUtil.h"

namespace JimmyGod::Core
{
	using DebugLogEvent = Event<const std::string&>;
	extern DebugLogEvent OnDebugLog;
}

#define BEGIN_MACRO do {
#define END_MACRO } while (false)

#if defined(_DEBUG)
#define LOG(format, ...)\
	BEGIN_MACRO\
		char _buffer[4096];\
		int _res = _snprintf_s(_buffer, std::size(_buffer), _TRUNCATE, "[%.3f]: "##format##"\n", JimmyGod::Core::TimeUtil::GetTime(), __VA_ARGS__);\
		OutputDebugStringA(_buffer);\
		JimmyGod::Core::OnDebugLog(_buffer);\
	END_MACRO

#define ASSERT(condition, format, ...)\
	BEGIN_MACRO\
		if (!(condition))\
		{\
			LOG("%s(%d) "##format, __FILE__, __LINE__, __VA_ARGS__);\
			DebugBreak();\
		}\
	END_MACRO
#else
#define LOG(format, ...)
#define ASSERT(condition, format, ...) do { (void)(sizeof(condition)); } while (false)
#endif
