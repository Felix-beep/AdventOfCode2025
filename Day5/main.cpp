#include "../includes.cpp"
#include "../global_functions.h"
#include "local_functions.h"

int main() {
    auto Clock = startClock();

    auto lines = readFromFile("input2");

    cout << "Test" << endl;

    auto [ranges, IDs] = ParseToInput(lines);

    std::sort(ranges.begin(), ranges.end(),
        [](const range& a, const range& b) {
            if (a.low != b.low) return a.low < b.low;
            return a.high < b.high;
        }
    );

    // Part 1
    vector<int> freshIDs;

    for (const auto& ID : IDs) {
        for (const auto& r : ranges) {
            if (ID >= r.low && ID <= r.high) {
                freshIDs.push_back(ID);
                break; 
            }
        }
    }

    // Part 2
    vector<range> CleanedRanges;

    for (range r : ranges) {
        if (CleanedRanges.empty()) {
            CleanedRanges.push_back(r);
            continue;
        }
        range& last = CleanedRanges.back();
        if (last.high >= r.low) {
            r.low = r.low + 1;
            r.high = std::max(last.high, r.high);
            last.high = r.low - 1;
        }
        CleanedRanges.push_back(r);
    }

    auto totalRangeArea = accumulate(CleanedRanges.begin(), CleanedRanges.end(), (int64_t)0, [](int64_t sum, auto& range) { return sum += range.high - range.low + 1; });

    printSolution(freshIDs.size(), totalRangeArea);

    endClock(Clock);
    return 0;
}