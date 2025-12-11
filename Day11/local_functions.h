#ifndef LOCAL_HEADER_H
#define LOCAL_HEADER_H

#include "../includes.cpp"

void local_function() {
	std::cout << "Hello from local function!" << std::endl;
};

struct Device {
	string name;
	vector<string> Outputs;
};

auto parseInput(vector<string>& lines) {
	vector<Device> Devices;
	for (string line : lines) {
		auto split = splitAndClean(line, ':');
		Device newDevice;
		newDevice.name = split[0];
		auto split2 = splitAndClean(split[1], ' ');
		for (string s : split2) {
			newDevice.Outputs.push_back(s);
		}
		Devices.push_back(newDevice);
	}
	return Devices;
}

auto FindAllPaths(string in, string out, map<string, Device>& Devices) {
	if (in == out) return 1;

	int AllPaths = 0;
	for (string d : Devices[in].Outputs) {
		AllPaths += FindAllPaths(d, out, Devices);
	}

	return AllPaths;
}

auto FindAllPathsP2(string in, string out, map<string, Device>& Devices, map<tuple<string, bool, bool>, int64_t>& seen, bool visitsfft = false, bool visitsdac = false) {
	if (in == out) return (visitsfft && visitsdac) ? int64_t(1) : int64_t(0);

	if (in == "fft") {
		visitsfft = true;
	}

	if (in == "dac") {
		visitsdac = true;
	}

	if (seen.contains(tuple(in, visitsfft, visitsdac))) {
		return seen[tuple(in, visitsfft, visitsdac)];
	}

	int64_t SpecificPaths = 0;
	for (string d : Devices[in].Outputs) {
		SpecificPaths += FindAllPathsP2(d, out, Devices, seen, visitsfft, visitsdac);
	}

	seen[tuple(in, visitsfft, visitsdac)] = SpecificPaths;

	return SpecificPaths;
}

#endif