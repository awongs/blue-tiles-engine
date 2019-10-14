#include <fstream>
#include <sstream>
#include <sdl2/SDL_image.h>

#include "FileManager.h"
#include "../engine/debugbt/DebugLog.h"
#include "../engine/graphics/Texture.h"

namespace filemanager
{
	std::unordered_map<std::string, std::shared_ptr<Texture>> cachedTextures;

	std::string LoadFile(const std::string filePath)
	{
		// Setup streams for reading the file
		std::ifstream inputStream;
		std::stringstream stringBuffer;

		// Try to open the file
		inputStream.open(filePath);
		if (inputStream.is_open())
		{
			//DebugLog::Info("Successfully opened " + filePath);

			// Read all file contents into the string buffer
			stringBuffer << inputStream.rdbuf();
			inputStream.close();

			return stringBuffer.str();
		}
		else
		{
			DebugLog::Error("Failed to open " + filePath);

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
			//DebugLog::Info("Successfully opened " + filePath);

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
		// Check if the texture is cached
		if (cachedTextures[filePath] != nullptr)
		{
			return cachedTextures[filePath];
		}

		// Image surface
		SDL_Surface* image;
		image = IMG_Load(filePath.c_str());

		// Check if the image data was sucessfully loaded
		if (image != nullptr)
		{
			// Create the texture
			std::shared_ptr<Texture> texture = std::make_shared<Texture>(image->pixels, image->w, image->h, image->format->BytesPerPixel);

			// Free the surface
			SDL_FreeSurface(image);

			// Return the texture as a shared pointer after caching it
			cachedTextures[filePath] = texture;
			return texture;
		}
		else
		{
			DebugLog::Error("Failed to open " + filePath);
			DebugLog::Error(IMG_GetError());
			return nullptr;
		}
	}
}
