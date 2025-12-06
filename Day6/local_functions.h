#ifndef LOCAL_HEADER_H
#define LOCAL_HEADER_H

#include "../includes.cpp"

void local_function() {
	std::cout << "Hello from local function!" << std::endl;
};

auto ParseToInput(const vector<string>& lines) {
	vector<vector<string>> result;

	int offset = 0;
	int width = lines[0].size();

	for (int i = 0; i < width; ++i) {
		bool delimiter = true;
		for (const string& line : lines) {
			if (i >= line.size() || line[i] != ' ') {
				delimiter = false;
				break;
			}
		}

		if (delimiter) {
			vector<string> chunk;
			for (const string& l : lines) {
				chunk.push_back(l.substr(offset, i - offset));
			}
			result.push_back(chunk);
			offset = i + 1;
		}
	}

	return result;
}

auto SplitNumbersAndOperators(vector<vector<string>> columns) {
	vector<char> characters;

	for (auto& col : columns) {
		characters.push_back(col.back()[0]);
		col.pop_back();                       
	}

	return std::make_pair(columns, characters);
}

auto Cephalopod(vector<vector<string>> input) {
	vector<vector<string>> result;

	for (auto line : input) {
		map<int, string> dictionary;
		for (string s : line) {
			for (int i = 0; i < s.length(); ++i) {
				dictionary[i] = dictionary[i] + s[i];
			}
		}
		vector<string> str;
		for (auto i : dictionary) {
			str.push_back(i.second);
		}
		result.push_back(str);
	}
	return result;
}

#endif