#include "DebugLog.h"
#include <time.h>
#include <sdl2/SDL_rwops.h>
#include <sstream>

namespace DebugLog
{

	/*
	* Taken from SDL_test_log.c
	*/
	static char* TimestampToString(const time_t timestamp)
	{
		time_t copy;
		static char buffer[64];
		struct tm local;
		SDL_memset(buffer, 0, sizeof(buffer));
		copy = timestamp;
#ifdef __linux__ 
	localtime_r(&copy, &local);
#elif defined(_WIN64)
	localtime_s(&local, &copy);

#endif
		strftime(buffer, sizeof(buffer), "%x %X", &local);

		return buffer;
	}

	static string TimestampedString(string msg)
	{
		std::stringstream ss;
		ss << TimestampToString(time(0)) << ": " << msg.c_str();
		return ss.str();
	}

	void Info(string infoMsg)
	{
		Info(SDL_LOG_CATEGORY_TEST, infoMsg);
	}

	void Info(int category, string infoMsg)
	{
		SDL_LogMessage(category, SDL_LOG_PRIORITY_INFO, TimestampedString(infoMsg).c_str());
	}

	void Warn(string warnMsg)
	{
		Warn(SDL_LOG_CATEGORY_TEST, warnMsg);
	}

	void Warn(int category, string warnMsg)
	{
		SDL_LogMessage(category, SDL_LOG_PRIORITY_WARN, TimestampedString(warnMsg).c_str());
	}

	void Error(string errorMsg)
	{
		Error(SDL_LOG_CATEGORY_TEST, errorMsg);
	}

	void Error(int category, string errorMsg)
	{
		std::string timedMsg = TimestampedString(errorMsg);

		SDL_RWops* rw = SDL_RWFromFile("error.log", "w");
		if (rw != NULL)
		{
			size_t len = SDL_strlen(timedMsg.c_str());
			if (SDL_RWwrite(rw, timedMsg.c_str(), 1, len) != len)
			{
				Warn("Couldn't write message to log file.");
			}
			SDL_RWclose(rw);
		}
		SDL_LogMessage(category, SDL_LOG_PRIORITY_ERROR, timedMsg.c_str());
	}
}
