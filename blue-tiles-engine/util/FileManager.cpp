#include <fstream>
#include <sstream>
#include <sdl2/SDL_image.h>

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
		// Image surface
		SDL_Surface* image;
		image = IMG_Load("../Assets/crate.jpg");

		// Check if the image data was sucessfully loaded
		if (image != nullptr)
		{
			// Create the texture
			std::shared_ptr<Texture> texture = std::make_shared<Texture>(image->pixels, image->w, image->h, image->format->BytesPerPixel);

			// Return the texture as a shared pointer after freeing the surface
			SDL_FreeSurface(image);
			return texture;
		}
		else
		{
			DebugLog::Error("Failed to load " + filePath);
			DebugLog::Error(IMG_GetError());
			return nullptr;
		}
	}
}
