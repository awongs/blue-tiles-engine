#pragma once
#include <string>

namespace filemanager
{
	// Returns a file's contents as a string
	// Expects filePath to be a path from the working directory
	std::string LoadFile(std::string filePath);
}