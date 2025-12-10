#ifndef LOCAL_HEADER_H
#define LOCAL_HEADER_H

#include "../includes.cpp"

using Lamp = bool;
using Button = vector<int>;
using Joltage = int;

struct InputLine {
	vector<Lamp> Lamps;
	vector<Button> Buttons;
	vector<Joltage> Joltages;
};

void local_function() {
	std::cout << "Hello from local function!" << std::endl;
};

std::vector<InputLine> parseInput(std::vector<std::string>& input)
{
    std::vector<InputLine> result;

    // Regexes:
    // 1) Lamps in [ .#... ]
    std::regex lamps_re(R"(\[([.#]+)\])");

    // 2) Each button group in ( 1,2,3 )
    std::regex buttons_re(R"(\(([^()]*)\))");

    // 3) Joltages in { 1,2,3 }
    std::regex joltages_re(R"(\{([^{}]*)\})");

    // 4) Integers (for buttons and joltages)
    std::regex int_re(R"(-?\d+)");

    for (const std::string& line : input) {
        // Skip completely empty / whitespace-only lines
        if (std::all_of(line.begin(), line.end(),
            [](unsigned char c) { return std::isspace(c); }))
            continue;

        InputLine item;

        // ---- Lamps ----
        {
            std::smatch m;
            if (std::regex_search(line, m, lamps_re)) {
                const std::string lamps_str = m[1].str(); // inside [...]
                for (char c : lamps_str) {
                    if (c == '.')      item.Lamps.push_back(false);
                    else if (c == '#') item.Lamps.push_back(true);
                    // anything else is ignored
                }
            }
        }

        // ---- Buttons (zero or more groups) ----
        {
            auto it = std::sregex_iterator(line.begin(), line.end(), buttons_re);
            auto end = std::sregex_iterator();

            for (; it != end; ++it) {
                const std::smatch& m = *it;
                std::string group = m[1].str(); // inside ( ... )

                Button b;

                auto iit = std::sregex_iterator(group.begin(), group.end(), int_re);
                auto iend = std::sregex_iterator();

                for (; iit != iend; ++iit) {
                    b.push_back(std::stoi((*iit).str()));
                }

                item.Buttons.push_back(std::move(b));
            }
        }

        // ---- Joltages (one { ... } group per line in your format) ----
        {
            std::smatch m;
            if (std::regex_search(line, m, joltages_re)) {
                std::string group = m[1].str(); // inside { ... }

                auto iit = std::sregex_iterator(group.begin(), group.end(), int_re);
                auto iend = std::sregex_iterator();

                for (; iit != iend; ++iit) {
                    item.Joltages.push_back(std::stoi((*iit).str()));
                }
            }
        }

        result.push_back(std::move(item));
    }

    return result;
}

#endif