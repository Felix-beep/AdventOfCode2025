#include "../includes.cpp"
#include "../global_functions.h"
#include "local_functions.h"

int main() {
    auto Clock = startClock();
    auto input = readFromFile("input2");

    vector<SafeInput> commands;

    ranges::transform(input.begin(), input.end(), back_inserter(commands), StringToInput);
    
    int start = 50;
    int counterPart1 = 0;
    int counterPart2 = 0;

    for (const SafeInput & command : commands) {

        int hits = 0;
        if (command.right) {
            hits = (start + command.num) / 100;
            start += command.num;
        }
        else{
            if (command.num >= start) {
                if (start != 0) hits++;
                hits += (command.num - start) / 100;
            }
            start -= command.num;
        }

        counterPart2 += hits;

        start %= 100;
        if (start < 0) start += 100;

        if (start == 0) {
            counterPart1++;
        }
    }

    printSolution(counterPart1, counterPart2);

    endClock(Clock);
    return 0;
}