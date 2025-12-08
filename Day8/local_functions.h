#ifndef LOCAL_HEADER_H
#define LOCAL_HEADER_H

#include "../includes.cpp"

void local_function() {
	std::cout << "Hello from local function!" << std::endl;
};

auto ParseInput(vector<string>& lines) {
	vector<Position3D> pos;
	for (string& line : lines) {
		auto split = splitAndClean(line, ',');
		pos.push_back(Position3D(StringToNumber<int64_t>(split[0]), StringToNumber<int64_t>(split[1]), StringToNumber<int64_t>(split[2])));
	}
	return pos;
}

#endif