#include "../includes.cpp"
#include "../global_functions.h"
#include "local_functions.h"

int main() {
    auto Clock = startClock();

    auto lines = readFromFile("input2");

    vector<pair<string, string>> input = ParseStringToInput(lines);

    vector<int64_t> invalidCodesP1;
    vector<int64_t> invalidCodesP2;


    auto checkIfValidP2 = [](const std::string value) {
        const int n = static_cast<int>(value.size());

        for (int characters = 1; characters <= n / 2; ++characters) {
            if (n % characters != 0) continue; // perfect division

            std::string pattern = value.substr(0, characters);

            auto block_indices = std::views::iota(1, n / characters);

            bool all_match = std::ranges::all_of(
                block_indices,
                [&](int k) {
                    int pos = k * characters;
                    return value.compare(pos, characters, pattern) == 0;
                }
            );

            if (all_match) {
                return false; // a pattern
            }
        }

        return true; // no pattern
    };


    auto checkIfValidP1 = [](const std::string& value) {
        if (value.length() % 2 != 0) return true;     
        int midpoint = value.length() / 2;
        return value.substr(0, midpoint) != value.substr(midpoint, midpoint);
    };

    auto getValidRanges = [&](pair<string, string> range) {
        int64_t firstNum = StringToNumber<int64_t>(range.first);
        int64_t secondNum = StringToNumber<int64_t>(range.second);
        for (int64_t start = firstNum; start <= secondNum; start++) {
            if (!checkIfValidP1(to_string(start))) invalidCodesP1.push_back(start);
            if (!checkIfValidP2(to_string(start))) invalidCodesP2.push_back(start);
        }
    };

    for (pair<string, string> line : input) {
        getValidRanges(line);
    }

    auto resultP1 = accumulate(invalidCodesP1.begin(), invalidCodesP1.end(), (int64_t)0);
    auto resultP2 = accumulate(invalidCodesP2.begin(), invalidCodesP2.end(), (int64_t)0);

    printSolution(resultP1, resultP2);

    endClock(Clock);
    Clock = startClock();



    // Hilfsfunktion: Anzahl der Dezimalziffern einer Zahl
    auto numDigits = [](long long n) -> int {
        n = std::llabs(n);
        if (n == 0) return 1;
        int d = 0;
        while (n > 0) {
            ++d;
            n /= 10;
        }
        return d;
        };

    // Hilfsfunktion: 10^e für kleine e
    auto pow10_int = [](int e) -> long long {
        long long r = 1;
        while (e-- > 0) {
            r *= 10;
        }
        return r;
        };

    // Hauptfunktion:
    // Liefert alle Zahlen im Bereich [from, to], die aus einem wiederholten Digit-Pattern bestehen,
    // z.B. 99 (9+9), 111 (1+1+1), 1111 (11+11 oder 1+1+1+1), 7777, 1212, 123123, ...
    auto findRepeatedSubstringNumbers = [&](long long from, long long to) -> std::vector<long long> {
        if (from > to) std::swap(from, to);

        std::set<long long> results;

        int minLen = numDigits(from);
        int maxLen = numDigits(to);

        for (int len = minLen; len <= maxLen; ++len) {
            long long minNumForLen = pow10_int(len - 1);       // kleinste len-stellige Zahl
            long long maxNumForLen = pow10_int(len) - 1;       // größte len-stellige Zahl

            // Falls der Bereich [from, to] keine len-stelligen Zahlen enthält -> überspringen
            if (to < minNumForLen || from > maxNumForLen) {
                continue;
            }

            // Patternlänge patLen (muss ein echter Teiler von len sein)
            for (int patLen = 1; patLen < len; ++patLen) {
                if (len % patLen != 0) continue;

                int repeats = len / patLen;

                // Pattern darf nicht mit 0 beginnen -> mindestens 10^(patLen-1)
                long long patternStart = pow10_int(patLen - 1);
                long long patternEnd = pow10_int(patLen) - 1;

                // Faktor F für candidate = pattern * F
                long long pow10_pat = pow10_int(patLen);
                long long factor = 0;
                for (int r = 0; r < repeats; ++r) {
                    factor = factor * pow10_pat + 1;
                }

                // anhand von from/to einschränken
                long long minPatternByRange = (from + factor - 1) / factor;  // ceil(from / factor)
                long long maxPatternByRange = to / factor;                   // floor(to / factor)

                long long patternLo = std::max(patternStart, minPatternByRange);
                long long patternHi = std::min(patternEnd, maxPatternByRange);

                if (patternLo > patternHi) {
                    continue;  // in diesem (len, patLen)-Fall keine Kandidaten im Bereich
                }

                for (long long pattern = patternLo; pattern <= patternHi; ++pattern) {
                    long long candidate = pattern * factor;

                    // Hier könnte man theoretisch noch mal from/to checken,
                    // ist aber durch die Range-Berechnung schon garantiert im Bereich.
                    results.insert(candidate);
                }
            }
        }

        return std::vector<long long>(results.begin(), results.end());
        };

    vector<vector<long long>> CodesP3;

    for (auto range : input) {
        int64_t firstNum = StringToNumber<int64_t>(range.first);
        int64_t secondNum = StringToNumber<int64_t>(range.second);
        CodesP3.push_back(findRepeatedSubstringNumbers(firstNum, secondNum));
    }

    /*for (auto Codes : CodesP3) {
        for (auto Code : Codes) {
            cout << Code << ",";
        }
        cout << endl;
    }*/

    long long total =
        std::accumulate(CodesP3.begin(), CodesP3.end(), (long long)0,
            [](long long sum, const std::vector<long long>& inner) {
                return sum + std::accumulate(inner.begin(), inner.end(), (long long)0);
            }
    );
    cout << endl;
     
    printSolution(resultP1, total);

    endClock(Clock);

    return 0;
}