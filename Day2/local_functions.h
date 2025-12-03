#ifndef LOCAL_HEADER_H
#define LOCAL_HEADER_H

#include "../includes.cpp"
#include "../global_functions.h"

void local_function() {
	std::cout << "Hello from local function!" << std::endl;
};

vector<pair<string, string>> ParseStringToInput(vector<string> in) {
	vector<pair<string, string>> ranges;
	for(string s : in) {
		vector<string> lines = splitAndClean(s, ',');
		for (string li : lines) {
			vector<string> range = splitAndClean(li, '-');
			ranges.push_back(make_pair(range[0], range[1]));
		}
	}
	return ranges;
}

#endif