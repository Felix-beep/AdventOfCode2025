#include "../includes.cpp"
#include "../global_functions.h"
#include "local_functions.h"

int main() {
    auto Clock = startClock();

    auto lines = readFromFile("input2");

    auto [numbers, characters] = SplitNumbersAndOperators(ParseToInput(lines));

    auto SolveProblem = [](vector<vector<string>> input, vector<char> characters) {
        vector<int64_t> results;

        for (int i = 0; i < input.size(); ++i) {
            results.push_back(accumulate(input[i].begin(), input[i].end(), (int64_t)0,
                [&](int64_t sum, string value)
                {
                    int64_t num = StringToNumber<int64_t>(value);
                    switch (characters[i]) {
                    case '*': return (sum) ? sum *= num : num;
                    case '+': return sum += num;
                    }
                }
            ));
        }

        return accumulate(results.begin(), results.end(), (int64_t)0);
    };


    printSolution(SolveProblem(numbers, characters), SolveProblem(Cephalopod(numbers), characters));

    endClock(Clock);
    return 0;
}