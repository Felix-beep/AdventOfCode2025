#ifndef LOCAL_HEADER_H
#define LOCAL_HEADER_H

#include "../includes.cpp"
#include "../global_functions.h"

struct range {
	int64_t low;
	int64_t high;
};

void local_function() {
	std::cout << "Hello from local function!" << std::endl;
};

auto ParseToInput(vector<string> lines){
	auto [ranges, IDs] = splitText(lines, "");

	vector<range> Vranges;
	vector<int64_t> VIDs;

	for(string line : ranges) {
		auto res = splitAndClean(line, '-');
		Vranges.push_back(range{ StringToNumber<int64_t>(res[0]), StringToNumber<int64_t>(res[1]) });
	}

	for (string line : IDs) {
		VIDs.push_back(StringToNumber<int64_t>(line));
	}

	return make_pair( Vranges, VIDs );
}

#endif