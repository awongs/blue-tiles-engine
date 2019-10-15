#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <map>

class Texture;

namespace filemanager
{
	// Returns a file's contents as a string.
	// Expects filePath to be a path from the working directory.
	std::string LoadFile(const std::string filePath);

	// Appends the specified content to the file.
	// Creates a new file if there is no file at that path.
	void AppendFile(const std::string filePath, const std::string content);

	// Returns an image's contents as an unsigned char pointer.
	// Expects filePath to be a path from the working directory.
	std::shared_ptr<Texture> LoadTexture(const std::string filePath);

	// Cache of textures
	extern std::unordered_map<std::string, std::shared_ptr<Texture>> cachedTextures;
}
