#include <fstream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "FileManager.h"
#include "../engine/debugbt/DebugLog.h"
#include "../engine/graphics/Texture.h"

namespace filemanager
{
	std::string LoadFile(const std::string filePath)
	{
		// Setup streams for reading the file
		std::ifstream inputStream;
		std::stringstream stringBuffer;

		// Try to open the file
		inputStream.open(filePath);
		if (inputStream.is_open())
		{
			DebugLog::Info("Successfully loaded " + filePath);

			// Read all file contents into the string buffer
			stringBuffer << inputStream.rdbuf();
			inputStream.close();

			return stringBuffer.str();
		}
		else
		{
			DebugLog::Error("Failed to load " + filePath);

			// Return empty string if not successful
			return std::string();
		}
	}
	
	void AppendFile(const std::string filePath, const std::string content)
	{
		// Setup stream for opening the file
		std::ofstream outputStream;

		// Try to open the file
		outputStream.open(filePath, std::ios_base::app);
		if (outputStream.is_open())
		{
			DebugLog::Info("Successfully opened " + filePath);

			// Append content to the file
			outputStream << content << std::endl;
			outputStream.close();
		}
		else
		{
			DebugLog::Error("Failed to open " + filePath);
		}
	}

	std::shared_ptr<Texture> LoadTexture(const std::string filePath)
	{
		// Image definitions
		int width, height, numberOfChannels;
		
		// Load the image data
		unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &numberOfChannels, 0);

		// Check if the image data was sucessfully loaded
		if (data == nullptr)
		{
			DebugLog::Error("Failed to load " + filePath);
			return nullptr;
		}

		// Return the texture as a shared pointer
		return std::make_shared<Texture>(data, width, height, numberOfChannels);
	}
}
