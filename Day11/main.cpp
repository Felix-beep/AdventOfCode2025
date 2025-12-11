#include "../includes.cpp"
#include "../global_functions.h"
#include "local_functions.h"

int main() {
    auto Clock = startClock();

    global_function();
    local_function();

    bool TestInput = 0;

    auto lines = readFromFile(TestInput ? "input1" : "input2");

    auto input = parseInput(lines);

    for (Device d : input) {
        cout << d.name << ": ";
        for (string s : d.Outputs) {
            cout << s << " ";
        }
        cout << endl;
    }

    map<string, Device> DeviceMap;

    for (Device d : input) {
        DeviceMap[d.name] = d;
    }

    auto P1 = FindAllPaths("you", "out", DeviceMap);

    map<tuple<string, bool, bool>, int64_t> seen;
    auto P2 = FindAllPathsP2("svr", "out", DeviceMap, seen);

    printSolution(P1, P2);

    endClock(Clock);
    return 0;
}