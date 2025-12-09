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
        return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1 << 6) + (h1 >> 2));
    }
};

std::vector<AxisPair> twoLargestAxisAlignedPairs(const std::vector<Point>& points)
{
    // x -> list of ys; y -> list of xs
    std::map<int64_t, std::vector<int64_t>> ys_at_x;
    std::map<int64_t, std::vector<int64_t>> xs_at_y;

    for (const auto& [x, y] : points) {
        ys_at_x[x].push_back(y);  // vertical candidates
        xs_at_y[y].push_back(x);  // horizontal candidates
    }

    AxisPair best1{ {0,0},{0,0}, -1 };
    AxisPair best2{ {0,0},{0,0}, -1 };

    auto consider = [&](const Point& a, const Point& b, int64_t d) {
        if (d > best1.dist) {
            best2 = best1;
            best1 = AxisPair{ a, b, d };
        }
        else if (d > best2.dist) {
            best2 = AxisPair{ a, b, d };
        }
        };

    // vertical segments (same x)
    for (auto& [x, ys] : ys_at_x) {
        if (ys.size() < 2) continue;
        std::sort(ys.begin(), ys.end());
        int64_t d = ys.back() - ys.front();
        consider(Point{ x, ys.front() }, Point{ x, ys.back() }, d);
    }

    // horizontal segments (same y)
    for (auto& [y, xs] : xs_at_y) {
        if (xs.size() < 2) continue;
        std::sort(xs.begin(), xs.end());
        int64_t d = xs.back() - xs.front();
        consider(Point{ xs.front(), y }, Point{ xs.back(), y }, d);
    }

    std::vector<AxisPair> result;
    if (best1.dist >= 0) result.push_back(best1);
    if (best2.dist >= 0 && !(best2.a == best1.a && best2.b == best1.b) &&
        !(best2.a == best1.b && best2.b == best1.a)) {
        result.push_back(best2);
    }
    return result;
}

bool is_valid_rectangle(const vector<Point>& points, const Point& p1, const Point& p2)
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

    // Per-row: bit 0 = has vertical edge (left/right)
    //          bit 1 = has interior x (between xmin and xmax)
    std::unordered_map<int64_t, uint8_t> row_flags;
    // Per-column: bit 0 = has horizontal edge (bottom/top)
    //             bit 1 = has interior y (between ymin and ymax)
    std::unordered_map<int64_t, uint8_t> col_flags;

    row_flags.reserve(std::size(points));
    col_flags.reserve(std::size(points));

    constexpr uint8_t EDGE = 1;
    constexpr uint8_t INNER = 2;

    for (const auto& p : points) {
        auto [px, py] = p;

        bool in_x_open = (xmin < px && px < xmax);
        bool in_y_open = (ymin < py && py < ymax);
        bool in_x_closed = (xmin <= px && px <= xmax);
        bool in_y_closed = (ymin <= py && py <= ymax);

        // 1) Strict interior point → reject immediately
        if (in_x_open && in_y_open) {
            return false;
        }

        bool on_left = (px == xmin && in_y_closed);
        bool on_right = (px == xmax && in_y_closed);
        bool on_bottom = (py == ymin && in_x_closed);
        bool on_top = (py == ymax && in_x_closed);

        // --- Vertical edges vs interior along the same row (y) ---

        if (on_left || on_right) {
            auto& rf = row_flags[py];
            if (rf & INNER) {
                // There is already a point between xmin and xmax on this row
                // → some edge point on this row "sees" that point towards interior
                return false;
            }
            rf |= EDGE;
        }

        if (in_x_open && in_y_closed) {
            // This point lies between xmin and xmax on row py
            auto& rf = row_flags[py];
            if (rf & EDGE) {
                // There is already a left/right edge point on this row
                // → that edge point "sees" this point towards interior
                return false;
            }
            rf |= INNER;
        }

        // --- Horizontal edges vs interior along the same column (x) ---

        if (on_bottom || on_top) {
            auto& cf = col_flags[px];
            if (cf & INNER) {
                return false;
            }
            cf |= EDGE;
        }

        if (in_y_open && in_x_closed) {
            // This point lies between ymin and ymax on column px
            auto& cf = col_flags[px];
            if (cf & EDGE) {
                return false;
            }
            cf |= INNER;
        }
    }

    return true; // All conditions satisfied
}

std::pair<Point, Point> largestAxisAlignedPair(const std::vector<Point>& points)
{
    // Maps: x -> all y values; y -> all x values
    std::map<int64_t, std::vector<int64_t>> ys_at_x;
    std::map<int64_t, std::vector<int64_t>> xs_at_y;

    for (const auto& [x, y] : points) {
        ys_at_x[x].push_back(y);  // for vertical checks
        xs_at_y[y].push_back(x);  // for horizontal checks
    }

    int64_t max_distance = -1;
    Point bestA{ 0,0 }, bestB{ 0,0 };

    // --- Vertical segments (same x) ---
    for (auto& [x, ys] : ys_at_x) {
        if (ys.size() < 2) continue;

        std::sort(ys.begin(), ys.end());

        int64_t d = ys.back() - ys.front();
        if (d > max_distance) {
            max_distance = d;
            bestA = Point{ x, ys.front() };
            bestB = Point{ x, ys.back() };
        }
    }

    // --- Horizontal segments (same y) ---
    for (auto& [y, xs] : xs_at_y) {
        if (xs.size() < 2) continue;

        std::sort(xs.begin(), xs.end());

        int64_t d = xs.back() - xs.front();
        if (d > max_distance) {
            max_distance = d;
            bestA = Point{ xs.front(), y };
            bestB = Point{ xs.back(), y };
        }
    }

    return { bestA, bestB };
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

    int64_t highestArea = 0;
    int64_t constrainedArea = 0;

    auto topPairs = twoLargestAxisAlignedPairs(points);

    // Collect their 4 endpoints into a set
    std::unordered_set<Point, PointHash> important;
    for (const auto& seg : topPairs) {
        important.insert(seg.a);
        important.insert(seg.b);
    }

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

            if (area > highestArea) {
                highestArea = area;
            }

            // Only do the expensive check if there is a chance to improve
            if (area > constrainedArea){
                if (!important.contains(p1) && !important.contains(p2))
                    continue;

                if (is_valid_rectangle(points, p1, p2)) {
                    constrainedArea = area;
                }

            }
        }
    }

    printSolution(highestArea, constrainedArea);

    endClock(Clock);
    return 0;
}