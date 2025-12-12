#ifndef LOCAL_HEADER_H
#define LOCAL_HEADER_H

#include "../includes.cpp"

using Shape = bool[3][3];

struct ShapeObject {
	int id;
	Shape cells{};
};

struct Region {
    int width;
    int height;
    vector<int> Ids;
};

auto roundDownToMultipleOf3 = [](int n) {
    return n - (n % 3);
};

auto parseInput(const std::vector<std::string>& input) {
    std::vector<ShapeObject> shapes;
    std::vector<Region> regions;

    ShapeObject current{};   // zero-initialized
    int row = 0;
    int emptyLines = 0;

    auto finish = [&]() {
        if (row == 3) shapes.push_back(current);
        current = ShapeObject{};
        row = 0;
        };

    for (const std::string& line : input) {
        if (line.empty()) {            // separator between shapes
            finish();
            emptyLines++;
            if (emptyLines == 6) break;
            continue;
        }
        if (line.size() == 2 && line[1] == ':') { // skip "0:", "1:", ...
            current.id = line[0] - 48;
            continue;
        }
        if (line.size() != 3 || row >= 3) {
            continue; // or throw / handle error
        }

        for (int col = 0; col < 3; ++col) {
            current.cells[row][col] = (line[col] == '#');
        }
        ++row;
    }

    finish(); // finalize last shape if no trailing blank line

    cout << "input length: " << input.size() << endl;
    for (int i = 30; i < input.size(); ++i) {
        cout << input[i] << endl;
        Region newRegion = Region();
        auto split = splitAndClean(input[i], ':');

        auto split2 = splitAndClean(split[0], 'x');
        newRegion.height = StringToNumber<int>(split2[0]);
        newRegion.width = StringToNumber<int>(split2[1]);

        auto split3 = splitAndClean(split[1], ' ');
        for (string s : split3) {
            newRegion.Ids.push_back(StringToNumber<int>(s));
        }

        regions.push_back(newRegion);
    }

    return make_pair(shapes, regions);
}

auto rotateShape = [](const Shape& in, int rot, bool(&out)[3][3]) {
    rot &= 3;
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            if (rot == 0) out[r][c] = in[r][c];
            else if (rot == 1) out[r][c] = in[2 - c][r];
            else if (rot == 2) out[r][c] = in[2 - r][2 - c];
            else out[r][c] = in[c][2 - r];
        }
    }
};

auto seeIfSimpleSolution = [](Region& region) {
    int worstspace = roundDownToMultipleOf3(region.width) * roundDownToMultipleOf3(region.height);
    int bestspace = region.width * region.height;
    int numOfShapes = accumulate(region.Ids.begin(), region.Ids.end(), 0);
    if (numOfShapes * 7 > bestspace) { // not enough space to fit everything even with perfect placement
        return -1;
    }

    if (numOfShapes * 9 < worstspace) { // enough space even with worst placement
        return 1;
    }

    return 0;
};

static vector<pair<int, int>> filledOffsets(const bool s[3][3]) {
    vector<pair<int, int>> pts;
    pts.reserve(9);
    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c)
            if (s[r][c]) pts.emplace_back(r, c);
    return pts;
}

static bool canPlace(const vector<vector<int>>& grid,
    int top, int left,
    const vector<pair<int, int>>& pts) {
    const int H = (int)grid.size();
    const int W = (int)grid[0].size();

    for (auto [dr, dc] : pts) {
        int r = top + dr;
        int c = left + dc;
        if (r < 0 || r >= H || c < 0 || c >= W) return false;
        if (grid[r][c] != -1) return false;
    }
    return true;
}

static void undoPlace(vector<vector<int>>& grid,
    int top, int left,
    const vector<std::pair<int, int>>& pts) {
    for (auto [dr, dc] : pts) {
        grid[top + dr][left + dc] = -1;
    }
}

static void doPlace(vector<vector<int>>& grid,
    int top, int left,
    const vector<pair<int, int>>& pts,
    int shapeId) {
    for (auto [dr, dc] : pts) {
        grid[top + dr][left + dc] = shapeId;
    }
}

auto seeIfHardSolution = [](Region& region, vector<ShapeObject>& shapes) -> pair<bool, int> {
    vector<vector<int>> grid(region.height, vector<int>(region.width, -1));

    vector<int> remaining = region.Ids;

    auto anyRemaining = [&]() -> bool {
        for (int x : remaining) if (x > 0) return true;
        return false;
    };

    bool rotBuf[3][3];

    int placeAttempts = 0;

    function<bool()> dfs = [&]() -> bool {
        placeAttempts++;
        if (!anyRemaining()) return true;

        int shapeIdx = -1;
        for (int i = 0; i < (int)remaining.size(); ++i) {
            if (remaining[i] > 0) { shapeIdx = i; break; }
        }
        if (shapeIdx == -1) return true;

        int shapeId = shapes[shapeIdx].id;

        for (int rot = 0; rot < 4; ++rot) {
            rotateShape(shapes[shapeIdx].cells, rot, rotBuf);
            auto pts = filledOffsets(rotBuf);
            if (pts.empty()) continue;

            for (int r = 0; r < region.height; ++r) {
                for (int c = 0; c < region.width; ++c) {
                    if (!canPlace(grid, r, c, pts)) continue;

                    doPlace(grid, r, c, pts, shapeId);
                    --remaining[shapeIdx];

                    if (dfs()) return true;

                    ++remaining[shapeIdx];
                    undoPlace(grid, r, c, pts);
                }
            }
        }
        
        return false;
    };

    bool isPossible = dfs();

    return make_pair(isPossible, placeAttempts);
};

#endif