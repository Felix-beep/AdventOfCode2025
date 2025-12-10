#include "../includes.cpp"
#include "../global_functions.h"
#include "local_functions.h"
#include <z3++.h>

auto solveBoth(const InputLine& in) {
    const std::vector<Lamp>& desiredLamps = in.Lamps;
    const std::vector<Button>& buttons = in.Buttons;
    const std::vector<Joltage>& targetJ = in.Joltages;

    const std::size_t n = desiredLamps.size();

    using State = std::vector<int>;  // counters for each lamp

    // Start state: all counters are zero
    State start(n, 0);

    auto satisfiesParity = [&](const State& counters) -> bool {
        for (std::size_t i = 0; i < n; ++i) {
            int parity = counters[i] % 2;
            int desired = desiredLamps[i] ? 1 : 0;
            if (parity != desired) return false;
        }
        return true;
        };

    auto satisfiesJoltages = [&](const State& counters) -> bool {
        if (counters.size() != targetJ.size()) return false;
        for (std::size_t i = 0; i < n; ++i) {
            if (counters[i] != targetJ[i]) return false;
        }
        return true;
        };

    auto applyButton = [&](State& counters, const Button& b) {
        for (int idx : b) {
            counters[idx] += 1;
        }
        };

    // BFS setup
    std::queue<State> q;
    std::map<State, int> dist;  // state -> number of button presses to reach it

    int paritySteps = -1;  // first time parity condition is satisfied
    int joltagesSteps = -1;  // first time exact joltages are reached

    // Initial state
    dist[start] = 0;
    q.push(start);

    // Check if initial state already satisfies either condition
    if (satisfiesParity(start)) {
        paritySteps = 0;
    }
    if (satisfiesJoltages(start)) {
        joltagesSteps = 0;
    }
    if (paritySteps != -1 && joltagesSteps != -1) {
        return make_pair(paritySteps, joltagesSteps);
    }

    bool done = false;

    while (!q.empty() && !done) {
        State cur = q.front();
        q.pop();

        int curDist = dist[cur];

        // Expand neighbors: press each button once
        for (const Button& b : buttons) {
            State next = cur;
            applyButton(next, b);

            // If we've already seen this state, skip
            if (dist.find(next) != dist.end()) {
                continue;
            }

            int nextDist = curDist + 1;
            dist[next] = nextDist;

            // Check parity solution if not yet found
            if (paritySteps == -1 && satisfiesParity(next)) {
                paritySteps = nextDist;
            }

            // Check joltage solution if not yet found
            if (joltagesSteps == -1 && satisfiesJoltages(next)) {
                joltagesSteps = nextDist;
            }

            // If we have both, we can stop BFS
            // THIS ACTUALLY TAKES WAYYYY TOO LONG TO FIND THE JOLTAGES; SO WE TRY AND CALCULATE THIS SOME OTHER WAY
            if (paritySteps != -1 /* && joltagesSteps != -1*/) {
                done = true;
                break;
            }

            q.push(std::move(next));
        }
    }

    return make_pair( paritySteps, joltagesSteps );
}

long long computePressesForJoltages_Z3(const InputLine& line) {
    const auto& buttons = line.Buttons;
    const auto& joltages = line.Joltages;

    std::size_t n = joltages.size(); // number of lamps / equations
    std::size_t m = buttons.size();  // number of buttons / unknowns

    if (n == 0 || m == 0) {
        // Only possible if all joltages are 0
        for (auto j : joltages) {
            if (j != 0) return -1;
        }
        return 0;
    }

    // Z3 context and optimizer
    z3::context ctx;
    z3::optimize opt(ctx);

    // Create integer variables x_0, x_1, ..., x_{m-1}, with x_k >= 0
    std::vector<z3::expr> x;
    x.reserve(m);
    for (std::size_t k = 0; k < m; ++k) {
        std::string name = "x_" + std::to_string(k);
        z3::expr var = ctx.int_const(name.c_str());
        opt.add(var >= 0);  // non-negative presses
        x.push_back(var);
    }

    // For each lamp i, add constraint:
    //    sum_k (A[i][k] * x_k) == J[i]
    // where A[i][k] = 1 if button k affects lamp i, else 0.
    for (std::size_t i = 0; i < n; ++i) {
        z3::expr sum = ctx.int_val(0);
        for (std::size_t k = 0; k < m; ++k) {
            const Button& b = buttons[k];
            bool affects = false;
            for (int idx : b) {
                if (idx == static_cast<int>(i)) {
                    affects = true;
                    break;
                }
            }
            if (affects) {
                sum = sum + x[k];  // coefficient is 1
            }
        }
        opt.add(sum == ctx.int_val(joltages[i]));
    }

    // Objective: minimize total number of presses sum_k x_k
    z3::expr total = ctx.int_val(0);
    for (std::size_t k = 0; k < m; ++k) {
        total = total + x[k];
    }
    opt.minimize(total);

    // Solve
    z3::check_result res = opt.check();
    if (res != z3::sat) {
        return -1; // unsat or unknown
    }

    // Get the optimal model
    z3::model mmodel = opt.get_model();

    // Extract the minimal total presses
    long long totalPresses = 0;
    for (std::size_t k = 0; k < x.size(); ++k) {
        z3::expr val = mmodel.eval(x[k], true);
        // convert to integer; adjust to get_numeral_int64() or via string if needed
        long long v = val.get_numeral_int64();
        totalPresses += v;
    }

    return totalPresses;
}


int main() {
    auto Clock = startClock();

    global_function();
    local_function();

    bool TestInput = 0;

    auto lines = readFromFile(TestInput ? "input1" : "input2");

    auto input = parseInput(lines);

    for (InputLine i : input) {
        cout << "Lamps: ";
        for (Lamp l : i.Lamps) {
            cout << (l ? '#' : '.');
        }

        cout << " Buttons: ";
        for (Button b : i.Buttons) {
            cout << "(";
            for (int i : b) {
                cout << i << " ";
            }
            cout << ") ";
        }


        cout << " Joltages: ";
        for (Joltage j : i.Joltages) {
            cout << j << " ";
        }
        cout << endl;
    }

    vector<int> ShortestSolutionsP1;
    vector<int> ShortestSolutionsP2;

    int counter = 1;

    for (InputLine i : input) {
        auto[LampSolution, _] = solveBoth(i);
        auto JoltageSolution = computePressesForJoltages_Z3(i);
        ShortestSolutionsP1.push_back(LampSolution);
        ShortestSolutionsP2.push_back(JoltageSolution);
        cout << counter++ << "/" << input.size() << " is " << LampSolution << " and " << JoltageSolution << endl;
    }

    auto P1 = accumulate(ShortestSolutionsP1.begin(), ShortestSolutionsP1.end(), 0);
    auto P2 = accumulate(ShortestSolutionsP2.begin(), ShortestSolutionsP2.end(), 0);

    printSolution(P1, P2);

    endClock(Clock);
    return 0;
}