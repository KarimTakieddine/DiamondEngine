#include <fstream>
#include <stdexcept>
#include <vector>

#include "File.h"

namespace diamond_engine {
	std::string File::Read(const std::string& path) {
		std::ifstream inputFileStream(path, std::ifstream::binary);

		if (!inputFileStream.is_open()) {
			throw std::runtime_error("Failed to open file: " + path);
		}

		inputFileStream.seekg(0, inputFileStream.end);
		auto fileSize = inputFileStream.tellg();
		inputFileStream.seekg(0, inputFileStream.beg);

		std::vector<char> contentsBuffer(
			static_cast<size_t>(fileSize) + 1,
			'\0');
		
		inputFileStream.read(contentsBuffer.data(), fileSize);
		inputFileStream.close();

		return std::string(contentsBuffer.data());
	}
}