#pragma once
#include <string>

namespace filemanager
{
	// Returns a file's contents as a string
	// Expects filePath to be a path from the working directory
	std::string LoadFile(const std::string filePath);

	// Appends the specified content to the file
	// Creates a new file if there is no file at that path
	void AppendFile(const std::string filePath, const std::string content);
}