#pragma once

#include <string>

namespace diamond_engine {
class File {
public:
	static std::string Read(const std::string& path);
};
}
