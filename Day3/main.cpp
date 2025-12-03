#include "../includes.cpp"
#include "../global_functions.h"
#include "local_functions.h"

int main() {
    auto Clock = startClock();

    auto input = readFromFile("input2");

    vector<int> Batteries;
    vector<int64_t> LongBatteries;

    // Part 1
    for (string& line : input) {
        char max = 0;
        char max2 = 0;
        for (int i = 0; i < line.length(); ++i) {
            if (i < line.length() - 1 && line[i] > max) {
                max = line[i];
                max2 = 0;
            }
            else if (line[i] > max2) {
                max2 = line[i];
            }
        }
        string newString = { max, max2 };
        Batteries.push_back(StringToNumber<int>(newString));
    }

    // Part 2
    const int stringLength = 12;

    for (string& line : input) {
        string Battery(stringLength, '0');
        int cutoff = 0;
        int len = static_cast<int>(line.length());

        for (int x = 0; x < stringLength; ++x) {
            int upper = len - stringLength + x; 
            //std::cout << "Looking at: " << line.substr(cutoff, upper - cutoff + 1) << '\n';
            for (int i = cutoff; i <= upper; ++i) {
                if (Battery[x] < line[i]) {
                    Battery[x] = line[i];
                    cutoff = i + 1;
                }
            }
        }
        LongBatteries.push_back(StringToNumber<int64_t>(Battery));
    }

    int totalP1 = accumulate(Batteries.begin(), Batteries.end(), 0);
    int64_t totalP2 = accumulate(LongBatteries.begin(), LongBatteries.end(), (int64_t)0);

    printSolution(totalP1, totalP2);

    endClock(Clock);
    return 0;
}