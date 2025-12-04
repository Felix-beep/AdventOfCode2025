#include "../includes.cpp"
#include "../global_functions.h"
#include "local_functions.h"

int main() {
    auto Clock = startClock();

    auto input = readFromFile("input2");

    int numOfPaperP1 = 0;
    int numOfPaperP2 = 0;

    // Part 1
    for (int x = 0; x < input.size(); ++x) {
        for (int y = 0; y < input[x].length(); ++y) {
            Position Pos = { x, y };
            if (input[x][y] != '@') continue;

            int neighbours = 0;
            for (Position dir : neighboursDiagonal) {
                Position newPos = Pos + dir;
                if (isInBounds(newPos, input.size(), input[x].length())) {
                    if (input[newPos.x][newPos.y] == '@') {
                        neighbours++;
                    }
                }
            }
            if (neighbours < 4) {
                numOfPaperP1++;
            }
        }
    }

    // Part 2
    auto removePaper = [](vector<string>& map) {
        int numRemoved = 0;
        for (int x = 0; x < map.size(); ++x) {
            for (int y = 0; y < map[x].length(); ++y) {
                Position Pos = { x, y };
                if (map[x][y] != '@') continue;

                int neighbours = 0;
                for (Position dir : neighboursDiagonal) {
                    Position newPos = Pos + dir;
                    if (isInBounds(newPos, map.size(), map[x].length())) {
                        if (map[newPos.x][newPos.y] == '@') {
                            neighbours++;
                        }
                    }
                }
                if (neighbours < 4) {
                    numRemoved++;
                    map[Pos.x][Pos.y] = '.';
                }
            }
        }
        return numRemoved;
    };

    while (true) {
        int paperRemoved = removePaper(input);
        numOfPaperP2 += paperRemoved;
        if (paperRemoved == 0) break;
    }

    printSolution(numOfPaperP1, numOfPaperP2);

    endClock(Clock);
    return 0;
}