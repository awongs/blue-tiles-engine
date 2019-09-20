#include <fstream>
#include <sstream>

#include "FileManager.h"
#include "../engine/debugbt/DebugLog.h"

namespace filemanager
{
	std::string LoadFile(std::string filePath)
	{
		// Setup streams for reading the file
		std::ifstream fileStream;
		std::stringstream stringBuffer;

		// Try to open the file
		fileStream.open(filePath);

		if (fileStream.is_open())
		{
			DebugLog::Info("Successfully loaded " + filePath);

			// Read all file contents into the string buffer
			stringBuffer << fileStream.rdbuf();
			return stringBuffer.str();
		}
		else
		{
			DebugLog::Error("Failed to load " + filePath);

			// Return empty string if not successful
			return std::string();
		}
	}
}


