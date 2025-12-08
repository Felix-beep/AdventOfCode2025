#include "../includes.cpp"
#include "../global_functions.h"
#include "local_functions.h"

auto Position3DPairHash = [](Position3D pos1, Position3D pos2) {
    Position3DHash singleHash;

    auto [a, b] = (pos1 < pos2) ? make_pair(pos1, pos2) : make_pair(pos2, pos1);

    std::size_t ha = singleHash(a);
    std::size_t hb = singleHash(b);

    return make_pair(ha, hb);
    };

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

    set<pair<size_t, size_t>> VisitedPositionPairs;
    int numOfCircuits = 0;

    // go through each Position Pairing
    for (int i = 0; i < (int)input.size(); ++i) {
        const Position3D& pos1 = input[i];
        for (int j = i + 1; j < (int)input.size(); ++j) {
            const Position3D& pos2 = input[j];
            double d = (pos1 - pos2).length();
            DistanceQueue.emplace(d, i, j);
        }
    }

    auto CalculatePart1 = [](map<int, vector<Position3D>>& Circuits) {
        // sort them 
        vector<pair<int, vector<Position3D>>> sortedCircuits;

        for (const auto& [id, members] : Circuits) {
            sortedCircuits.emplace_back(id, members);
        }

        sort(sortedCircuits.begin(), sortedCircuits.end(),
            [](const auto& a, const auto& b) {
                return a.second.size() > b.second.size();
            });

        // Keep only the top 3
        int numToKeep = 3;
        if (sortedCircuits.size() > numToKeep) {
            sortedCircuits.resize(numToKeep);
        }

        // calculate the result
        return accumulate(sortedCircuits.begin(), sortedCircuits.end(), (int64_t)1, [](int64_t sum, pair<int, vector<Position3D>> p) { return sum *= p.second.size(); });
    };

    auto CalculatePart2 = [](Position3D pos1, Position3D pos2) {
        return (int64_t)pos1.x * (int64_t)pos2.x;
    };

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
        auto [dist, i, j] = DistanceQueue.top();
        DistanceQueue.pop();
        Position3D& pos1 = input[i];
        Position3D& pos2 = input[j];
        int Uid1 = MapOfPostions[pos1];
        int Uid2 = MapOfPostions[pos2];
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
            resultP2 = CalculatePart2(pos1, pos2);
        }
    }



    printSolution(resultP1, resultP2);


    endClock(Clock);
    return 0;
}