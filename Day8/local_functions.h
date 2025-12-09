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

auto CalculatePart1 = [](map<int, vector<Position3D>>& Circuits) {
	// sort them 
	vector<pair<int, vector<Position3D>>> sortedCircuits;

	for (const auto& [id, members] : Circuits) {
		sortedCircuits.emplace_back(id, members);
	}

	sort(sortedCircuits.begin(), sortedCircuits.end(),
		[](const auto& a, const auto& b) {
			return a.second.size() > b.second.size();
		});

	// Keep only the top 3
	int numToKeep = 3;
	if (sortedCircuits.size() > numToKeep) {
		sortedCircuits.resize(numToKeep);
	}

	// calculate the result
	return accumulate(sortedCircuits.begin(), sortedCircuits.end(), (int64_t)1, [](int64_t sum, pair<int, vector<Position3D>> p) { return sum *= p.second.size(); });
};

auto CalculatePart2 = [](Position3D pos1, Position3D pos2) {
	return (int64_t)pos1.x * (int64_t)pos2.x;
};


#endif