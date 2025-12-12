#include "../includes.cpp"
#include "../global_functions.h"
#include "local_functions.h"

int main() {
    auto Clock = startClock();

    bool TestInput = 0;

    auto input = readFromFile(TestInput ? "input1" : "input2");

    auto [shapes, regions] = parseInput(input);
    /*
    for (const auto& s : shapes) {
        std::cout << s.id << ":\n";
        for (int r = 0; r < 3; ++r) {
            for (int c = 0; c < 3; ++c) {
                std::cout << (s.cells[r][c] ? '#' : '.');
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    }

    for (const auto& s : regions) {
        std::cout << s.height << "/" << s.width << ":\n";
        for (int i : s.Ids) {
            cout << i << " ";
        }
        cout << endl;
    }
    */
    int Solvables = 0;
    int64_t PlaceAttempts = 0;

    for (auto s : regions) {
        // -1 = mathematically impossible, 1 = mathematically trivial, 0 = unknown
        int isPossible = seeIfSimpleSolution(s);
        if (isPossible == -1) continue;
        if (isPossible == 1) { 
            Solvables++;
            continue;
        }

        // apparently all problems that are potentionally possible are also solvable... 
        // so we can skip this part and just assume that all where isPossible == 0 are Solvables.

        if (!isPossible) {
            auto [possible, attempts] = seeIfHardSolution(s, shapes);
            PlaceAttempts += attempts;
            if (possible) {
                Solvables++;
            }
        }
    }

    cout << "Solution: " << Solvables << endl;
    cout << "Blocks attempted to be placed: " << PlaceAttempts << endl;

    endClock(Clock);
    return 0;
}