#pragma once

#include <ostream>
#include <sstream>

namespace diamond_engine {
struct Size {
	int width, height;
};

static std::string SizeToString(const Size& size) {
	std::stringstream stream;
	stream << "Size -- Width: " << size.width << " - Height: " << size.height;
	return stream.str();
}

static std::ostream& operator<<(std::ostream& stream, const Size& size) {
	return stream << SizeToString(size);
}

inline bool operator==(const Size& lhs, const Size& rhs) {
	return (lhs.width == rhs.width) && (lhs.height == rhs.height);
}
}
