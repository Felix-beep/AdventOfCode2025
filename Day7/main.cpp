#include "../includes.cpp"
#include "../global_functions.h"
#include "local_functions.h"

int main() {
    auto Clock = startClock();

    auto input = readFromFile("input2");
    map<int64_t, int64_t> beams;

    auto it = find(input[0].begin(), input[0].end(), 'S');
    if (it != input[0].end()) {
        beams[distance(input[0].begin(), it)] = 1;
    }

    int splits = 0;
    
    for (const string& s : input) {
        map<int64_t, int64_t> next_beams;
        for (auto i : beams) {
            if (i.first < 0 || i.first >= (int)s.size()) continue; 

            if (s[i.first] == '^') {
                if (i.first - 1 >= 0)
                    next_beams[i.first - 1] += i.second;
                if (i.first + 1 < (int)s.size())
                    next_beams[i.first + 1] += i.second;
                splits++;
            }
            else {
                next_beams[i.first] += i.second;
            }
        }

        beams.swap(next_beams);
    }

    auto totalBeams = accumulate(beams.begin(), beams.end(), (int64_t)0, [](int64_t sum, auto value) { return sum += value.second; });

    printSolution(splits, totalBeams);

    endClock(Clock);
    return 0;
}