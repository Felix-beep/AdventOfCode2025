#include "../includes.cpp"
#include "../global_functions.h"
#include "local_functions.h"

int main() {
    auto Clock = startClock();

    bool TestInput = false;
    auto text = readFromFile((TestInput) ? "input1" : "input2");
    auto input = ParseInput(text);

    using Edge = std::tuple<double, int, int>;

    auto cmp = [&input](const Edge& a, const Edge& b) {
        return get<0>(a) > get<0>(b);
        };
    std::priority_queue<Edge, std::vector<Edge>, decltype(cmp)> DistanceQueue(cmp);

    // go through each Position Pairing and put them in the queue
    for (int i = 0; i < (int)input.size(); ++i) {
        const Position3D& pos1 = input[i];
        for (int j = i + 1; j < (int)input.size(); ++j) {
            const Position3D& pos2 = input[j];
            double d = (pos1 - pos2).length();
            DistanceQueue.emplace(d, i, j);
        }
    }
    map<int, vector<Position3D>> MapOfCircuits;
    map<Position3D, int> MapOfPostions;

    for (int i = 0; i < input.size(); ++i) {
        MapOfCircuits[i] = vector<Position3D>{ input[i] };
        MapOfPostions[input[i]] = i;
    }

    int64_t resultP1 = 0;
    int64_t resultP2 = 0;
    int  ConnectedPoints = 0;

    while (!DistanceQueue.empty() && MapOfCircuits.size() > 1) {
        auto &[dist, i, j] = DistanceQueue.top();
        DistanceQueue.pop();
        int Uid1 = MapOfPostions[input[i]];
        int Uid2 = MapOfPostions[input[j]];
        ConnectedPoints++;

        if (Uid1 != Uid2) {
            if (MapOfCircuits[Uid1].size() < MapOfCircuits[Uid2].size()) {
                std::swap(Uid1, Uid2);
            }
            for (const Position3D& pos : MapOfCircuits[Uid2]) {
                MapOfPostions[pos] = Uid1;
                MapOfCircuits[Uid1].push_back(pos);
            }
            MapOfCircuits.erase(Uid2);
        }

        if (ConnectedPoints == (TestInput ? 10 : 1000)) {
            resultP1 = CalculatePart1(MapOfCircuits);
        }

        if (MapOfCircuits.size() == 1) {
            resultP2 = CalculatePart2(input[i], input[j]);
        }
    }

    printSolution(resultP1, resultP2);

    endClock(Clock);
    return 0;
}