#ifndef GLOBAL_HEADER_H
#define GLOBAL_HEADER_H

#include "includes.cpp"

auto readFromFile(std::string filename) {
    std::ifstream file(filename + ".txt");

    std::vector<std::string> lines;
    std::string line;

    if (!file) {
        std::cerr << "Failed to open file.\n";
    }

    // Step 1: Read lines from the file
    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    return lines;
}

void writeToFile(const std::string& filename, const std::string& content) {
    // Create an output file stream
    std::ofstream outFile(filename);

    // Check if the file is open
    if (!outFile) {
        std::cerr << "Error: Could not open file " << filename << " for writing." << std::endl;
        return;
    }

    // Write the content to the file
    outFile << content;

    // Close the file
    outFile.close();

    std::cout << "Content written to " << filename << std::endl;
}

template<typename T>
auto ViewToVector() {
    return [](const auto& view) {
        std::vector<T> result;
        for (const auto& item : view) {
            result.push_back(item);
        }
        return result;
        };
}

template <typename T>
void printVector(const std::vector<T>& input) {
    for (const auto& item : input) {
        std::cout << item << '\n';
    }
}

void printSolution(int64_t part1, int64_t part2) {
    cout << "Solution Part 1: " << part1 << endl;
    cout << "Solution Part 2: " << part2 << endl;
}

auto startClock() {
    return chrono::high_resolution_clock::now();
}

auto endClock(chrono::time_point<chrono::high_resolution_clock> startTime) {
    auto endTime = chrono::high_resolution_clock::now(); // End the timer
    chrono::duration<double> elapsed = endTime - startTime; // Calculate elapsed time
    cout << "Execution time: " << elapsed.count() << " seconds" << endl;
}

void global_function() {
    std::cout << "Hello from global function!" << std::endl;
};


// Trims leading and trailing whitespace from a string
std::string trim(const std::string& str) {
    auto start = std::find_if_not(str.begin(), str.end(), ::isspace);
    auto end = std::find_if_not(str.rbegin(), str.rend(), ::isspace).base();
    return (start < end) ? std::string(start, end) : "";
}


// Splits a string by a delimiter and removes empty/whitespace strings
std::vector<std::string> splitAndClean(const std::string& input, char delimiter) {
    std::stringstream ss(input);
    std::string item;

    // Split the string and process each item
    std::vector<std::string> tokens;
    while (std::getline(ss, item, delimiter)) {
        auto trimmed = trim(item);
        if (!trimmed.empty()) {
            tokens.push_back(trimmed);
        }
    }

    return tokens;
}

pair<vector<std::string>, vector<std::string>> splitText(const vector<string>& input, string delimiter = "") {
    vector<std::string> firstPart;
    vector<std::string> secondPart;

    bool delimiterReached = false;

    for (const auto& line : input) {
        if (line == delimiter) {
            delimiterReached = true;
            continue;
        }
        (delimiterReached ? secondPart : firstPart).push_back(line);
    }

    return { firstPart, secondPart };
}

template <typename T>
T StringToNumber(const std::string& input) {
    static_assert(std::is_arithmetic<T>::value,
        "Template parameter T must be a numeric type");

    if constexpr (std::is_integral_v<T>) {
        return static_cast<T>(std::stoll(input));   // for integer types
    }
    else if constexpr (std::is_floating_point_v<T>) {
        return static_cast<T>(std::stod(input));    // for floating-point types
    }
}

template <typename T>
std::vector<T> VectorStringToNumber(const std::vector<std::string>& input) {
    static_assert(std::is_arithmetic_v<T>,
        "Template parameter T must be a numeric type");

    std::vector<T> output;
    output.reserve(input.size());  // optional but efficient

    for (const std::string& line : input) {
        output.push_back(StringToNumber<T>(line));
    }

    return output;
}


template <typename T>
T StringToNumberCast(const std::string& input) {
    static_assert(std::is_arithmetic<T>::value, "Template parameter T must be a numeric type");

    if constexpr (std::is_integral<T>::value) {
        return static_cast<T>(std::stoll(input)); // Convert to long long for integers
    }
    else if constexpr (std::is_floating_point<T>::value) {
        return static_cast<T>(std::stod(input)); // Convert to double for floating-point types
    }
}



auto getDistance = [](int a, int b) {
    return std::abs(a - b);
    };

struct Position {
    int64_t x;
    int64_t y;

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }

    bool operator<(const Position& other) const {
        if (x != other.x) {
            return x < other.x;
        }
        return y < other.y;
    }

    Position operator+(const Position& other) const {
        return { x + other.x, y + other.y };
    }

    Position operator-(const Position& other) const {
        return { x - other.x, y - other.y };
    }

    Position operator*(int scalar) const {
        return { x * scalar, y * scalar };
    }

    Position& operator=(const Position& other) {
        if (this != &other) { // Avoid self-assignment
            x = other.x;
            y = other.y;
        }
        return *this; // Return reference to the current object
    }
};

struct PositionsHash {
    std::size_t operator()(const Position& pos) const {
        // Combine the hash of x and y
        return std::hash<int64_t>()(pos.x) ^ (std::hash<int64_t>()(pos.y) << 1);
    }
};

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    UNKNOWN
};

vector<Position> neighbours = {
    {0, 1},
    {0, -1},
    {1, 0},
    {-1, 0}
};

vector<Position> neighboursDiagonal = {
    {1, 1},
    {1, -1},
    {-1, 1},
    {-1, -1},
    {0, 1},
    {0, -1},
    {1, 0},
    {-1, 0}
};

vector<Position> getNormals(Position pos) {
    vector<Position> normals;
    if (pos.x == 0) {
        normals.push_back({ 1, 0 });
        normals.push_back({ -1, 0 });
    }
    else if (pos.y == 0) {
        normals.push_back({ 0, 1 });
        normals.push_back({ 0, -1 });
    }
    return normals;
}

bool isInBounds(Position pos, int rows, int cols) {
    return pos.x >= 0 && pos.x < rows && pos.y >= 0 && pos.y < cols;
}

struct Position3D {
    int64_t x;
    int64_t y;
    int64_t z;

    bool operator==(const Position3D& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator<(const Position3D& other) const {
        if (x != other.x) {
            return x < other.x;
        }
        if (y != other.y) {
            return y < other.y;
        }
        return z < other.z;
    }

    Position3D operator+(const Position3D& other) const {
        return { x + other.x, y + other.y, z + other.z };
    }

    Position3D operator-(const Position3D& other) const {
        return { x - other.x, y - other.y, z - other.z };
    }

    Position3D operator*(int scalar) const {
        return { x * scalar, y * scalar, z * scalar };
    }

    Position3D& operator=(const Position3D& other) {
        if (this != &other) {
            x = other.x;
            y = other.y;
            z = other.z;
        }
        return *this;
    }

    double length() const {
        return std::sqrt(
            static_cast<double>(x) * x +
            static_cast<double>(y) * y +
            static_cast<double>(z) * z
        );
    }
};

struct Position3DHash {
    std::size_t operator()(const Position3D& pos) const {
        // Combine the hash of x, y, and z
        std::size_t hx = std::hash<int64_t>()(pos.x);
        std::size_t hy = std::hash<int64_t>()(pos.y);
        std::size_t hz = std::hash<int64_t>()(pos.z);

        return hx ^ (hy << 1) ^ (hz << 2);
    }
};



#endif