#include "../includes.cpp"
#include "../global_functions.h"
#include "local_functions.h"

using Point = std::pair<int64_t, int64_t>;

struct AxisPair {
    Point a;
    Point b;
    int64_t dist;
};

struct PointHash {
    std::size_t operator()(const Point& p) const noexcept {
        std::size_t h1 = std::hash<int64_t>{}(p.first);
        std::size_t h2 = std::hash<int64_t>{}(p.second);

        // Standard hash combine
        return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1 << 6) + (h1 >> 2));
    }
};

bool is_valid_rectangle_edges_ok(const std::vector<Point>& points,
    const Point& p1,
    const Point& p2)
{
    auto [x1, y1] = p1;
    auto [x2, y2] = p2;

    int64_t xmin = std::min(x1, x2);
    int64_t xmax = std::max(x1, x2);
    int64_t ymin = std::min(y1, y2);
    int64_t ymax = std::max(y1, y2);

    // Degenerate rectangles: no area
    if (xmin == xmax || ymin == ymax)
        return false;

    // Reject if any point lies strictly inside the rectangle
    for (const auto& p : points) {
        auto [px, py] = p;

        if (xmin < px && px < xmax &&
            ymin < py && py < ymax) {
            return false;  // strictly interior point → invalid
        }
    }

    // Edges and corners are allowed
    return true;
}

bool is_valid_rectangle_corners_only(const std::vector<Point>& points,
    const Point& p1,
    const Point& p2)
{
    auto [x1, y1] = p1;
    auto [x2, y2] = p2;

    int64_t xmin = std::min(x1, x2);
    int64_t xmax = std::max(x1, x2);
    int64_t ymin = std::min(y1, y2);
    int64_t ymax = std::max(y1, y2);

    // Degenerate rectangles: no area
    if (xmin == xmax || ymin == ymax)
        return false;

    bool hasExtraCorner = false;

    for (const auto& p : points) {
        auto [px, py] = p;

        // 1) Strictly inside → always invalid
        bool inside_strict =
            (xmin < px && px < xmax &&
                ymin < py && py < ymax);

        if (inside_strict) {
            return false;
        }

        // 2) On boundary?
        bool on_vertical_edge = (px == xmin || px == xmax) && (py >= ymin && py <= ymax);
        bool on_horizontal_edge = (py == ymin || py == ymax) && (px >= xmin && px <= xmax);
        bool on_edge = on_vertical_edge || on_horizontal_edge;

        // 3) Is this point exactly a corner?
        bool is_corner =
            ((px == xmin || px == xmax) &&
                (py == ymin || py == ymax));

        // Edge but not at corner → forbidden
        if (on_edge && !is_corner) {
            return false;
        }

        // 4) Corner point that is not one of the defining corners → count it
        if (is_corner && !(p == p1 || p == p2)) {
            hasExtraCorner = true;
        }
    }

    // Valid only if there is at least one extra corner point
    return hasExtraCorner;
}

int main() {
    auto Clock = startClock();

    bool TestInput = 0;

    std::vector<std::string> lines = readFromFile(TestInput ? "input1" : "input2");

    // Materialize the points once
    std::vector<Point> points;
    points.reserve(lines.size());
    for (const auto& line : lines) {
        auto split = splitAndClean(line, ',');
        points.emplace_back(
            StringToNumber<int64_t>(split[0]),
            StringToNumber<int64_t>(split[1])
        );
    }

    for (int y = 20; y > 0; --y) {
        for (int x = 0; x < 10; x++) {
            if (any_of(points.begin(), points.end(), [&](Point cmp) { return cmp.first == y && cmp.second == x; })) {
                cout << "X";
            }
            else {
                cout << "O";
            }
        }
        cout << endl;
    }

    int64_t highestArea = 0;       // largest area of ANY rectangle (even invalid)
    int64_t bestValidArea = 0;     // largest area of rectangles that pass is_valid_rectangle
    int64_t longestRecordedWidth = 0;

    Point baseP1{ 0,0 }, baseP2{ 0,0 };   // corners of the best valid "base" rectangle

    // --------------------------------------------------------
    // 1) Find the largest-area valid rectangle (the base one)
    // --------------------------------------------------------
    for (auto it1 = points.begin(); it1 != points.end(); ++it1) {
        for (auto it2 = std::next(it1); it2 != points.end(); ++it2) {
            const auto& p1 = *it1;
            const auto& p2 = *it2;

            auto [small, large] =
                (p1 < p2)
                ? std::pair{ p1, p2 }
            : std::pair{ p2, p1 };

            int64_t width = large.first + 1 - small.first;
            int64_t height = large.second + 1 - small.second;
            int64_t area = std::llabs(width * height);

            // track unconstrained max area if you still want it
            if (area > highestArea) {
                highestArea = area;
            }

            // only check validity if it can beat current best valid area
            if (width > longestRecordedWidth) {
                if (is_valid_rectangle_corners_only(points, p1, p2)) {
                    longestRecordedWidth = width;
                    baseP1 = p1;
                    baseP2 = p2;
                }
            }
        }
    }

    cout << baseP1.first << "/" << baseP1.second << " to " << baseP2.first << "/" << baseP2.second << endl;

    // no valid rectangles;
    if (longestRecordedWidth == 0) {
        printSolution(highestArea, 0);
        endClock(Clock);
        return 0;
    }

    
    // --------------------------------------------------------
// 2) Extend from base rectangle using only corners that
//    are present in the input point list.
//    Rectangles must have both corners from `points` and
//    no interior points (edges/corners allowed).
// --------------------------------------------------------
    int64_t constrainedArea = 0;

    auto [bx1, by1] = baseP1;
    auto [bx2, by2] = baseP2;

    int64_t xmin = std::min(bx1, bx2);
    int64_t xmax = std::max(bx1, bx2);
    int64_t ymin = std::min(by1, by2);
    int64_t ymax = std::max(by1, by2);

    Point topLeft{ xmin, ymax };
    Point topRight{ xmax, ymax };
    Point bottomLeft{ xmin, ymin };
    Point bottomRight{ xmax, ymin };

    auto rect_area = [](const Point& a, const Point& b) -> int64_t {
        int64_t w = std::llabs(a.first - b.first) + 1;
        int64_t h = std::llabs(a.second - b.second) + 1;
        return w * h;
        };

    // Build a set for quick membership checks
    std::unordered_set<Point, PointHash> pointSet(points.begin(), points.end());

    // Only corners that actually exist in the input can be used
    std::vector<Point> topCorners;
    std::vector<Point> bottomCorners;

    if (pointSet.contains(topLeft))     topCorners.push_back(topLeft);
    if (pointSet.contains(topRight))    topCorners.push_back(topRight);
    if (pointSet.contains(bottomLeft))  bottomCorners.push_back(bottomLeft);
    if (pointSet.contains(bottomRight)) bottomCorners.push_back(bottomRight);

    // Optional tracking of the best extension
    Point bestExtP1 = baseP1;
    Point bestExtP2 = baseP2;

    for (const auto& p : points) {
        auto [px, py] = p;

        // Only consider points horizontally aligned with the base rectangle
        if (px < xmin || px > xmax) {
            continue;
        }

        // ===================== CASE 1: p ABOVE the top edge =====================
        if (py > ymax && !topCorners.empty()) {
            // For each *existing* top corner, try rectangle (corner, p)
            for (const auto& corner : topCorners) {
                // Both `corner` and `p` are guaranteed to be in the input

                if (!is_valid_rectangle_edges_ok(points, corner, p)) {
                    continue;
                }

                int64_t area = rect_area(corner, p);
                if (area > constrainedArea) {
                    constrainedArea = area;
                    bestExtP1 = corner;
                    bestExtP2 = p;
                }
            }
        }

        // ===================== CASE 2: p BELOW the bottom edge =====================
        else if (py < ymin && !bottomCorners.empty()) {
            for (const auto& corner : bottomCorners) {
                if (!is_valid_rectangle_edges_ok(points, corner, p)) {
                    continue;
                }

                int64_t area = rect_area(corner, p);
                if (area > constrainedArea) {
                    constrainedArea = area;
                    bestExtP1 = corner;
                    bestExtP2 = p;
                }
            }
        }
    }

    // `constrainedArea` is now the largest area of a rectangle such that:
    //  - both corners are in the original point list
    //  - one corner is on the top edge (if p is above) or bottom edge (if p is below)
    //  - there are no interior points (edges and corners are allowed)

    printSolution(highestArea, constrainedArea);

    endClock(Clock);
    return 0;
}
