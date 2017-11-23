#include "IOManager.h"
#include <fstream>


bool IOManager::readFileToBuffer(std::string& filePath, std::vector<unsigned char>& buffer) {
	// Attempts to open the file.
	std::ifstream file(filePath, std::ios::binary);
	// Check if file failed to load and throw an error.
	if (file.fail()) {
		perror(filePath.c_str());
		return false;
	}

	// Seek to the end of the file.
	file.seekg(0, std::ios::end);
	// Create and set variable fileSize to the files size.
	int fileSize = file.tellg();
	// Seek to the beginning of the file.
	file.seekg(0, std::ios::beg);
	// reduce the file size by any header bytes that might be present.
	fileSize -= file.tellg();
	// Resize 'beffer' to fileSize.
	buffer.resize(fileSize);
	// Reads from the file
	file.read((char*)&(buffer[0]), fileSize);
	// Ensures the file closes.
	file.close();


	return true;
}