#include <string>
#include <fstream>
#include <set>
#include <iostream>
#include <vector>

namespace {
constexpr auto separator = ',';

enum class FoldAxe {
    x,
    y
};

using Point = std::pair<int, int>;
using PointSet = std::set<Point>;
using Fold = std::pair<FoldAxe, int>;

Point read_point(const std::string& input_string) {
    const auto separator_position = input_string.find(separator);
    Point point;
    point.first = std::stoi(input_string.substr(0, separator_position));
    point.second = std::stoi(input_string.substr(separator_position + 1));
    return point;
}

PointSet read_points(std::ifstream& input_stream) {
    PointSet point_set;
    std::string input_string;
    std::getline(input_stream, input_string);
    while (!input_string.empty()) {
        point_set.insert(read_point(input_string));
        std::getline(input_stream, input_string);
    }

    return point_set;
}

Fold read_fold(const std::string& input_string) {
    Fold fold;
    fold.first = FoldAxe::x;
    if (input_string.at(11) == 'y') {
        fold.first = FoldAxe::y;
    }

    fold.second = std::stoi(input_string.substr(13));
    return fold;
}

void fold_vertically(PointSet& points, int x) {
    PointSet new_points;
    for (auto point : points) {
        if (point.first > x) {
            point.first = 2 * x - point.first;
        }

        new_points.insert(point);
    }

    points = new_points;
}

void fold_horizontally(PointSet& points, int y) {
    PointSet new_points;
    for (auto point : points) {
        if (point.second > y) {
            point.second = 2 * y - point.second;
        }

        new_points.insert(point);
    }

    points = new_points;
}

void fold_paper(PointSet& points, const Fold& fold) {
    if (fold.first == FoldAxe::x) {
        fold_vertically(points, fold.second);
    } else {
        fold_horizontally(points, fold.second);
    }
}

void draw_points(const PointSet& points) {
    int max_x = 0, max_y = 0;
    for (const auto& point : points) {
        max_x = std::max(max_x, point.first);
        max_y = std::max(max_y, point.second);
    }

    std::vector<std::vector<char>> drawing(max_y + 1, std::vector<char>(max_x + 1, '.'));
    for (const auto& point : points) {
        drawing.at(point.second).at(point.first) = '#';
    }

    for (const auto& row : drawing) {
        for (const auto cell : row) {
            std::cout << cell;
        }

        std::cout << std::endl;
    }
}
}

int main(int argc, char *argv[]) {
    const std::string input_path = argv[1];
    std::ifstream input_stream{input_path};
    auto points = read_points(input_stream);

    std::string input_string;
    std::getline(input_stream, input_string);
    auto fold = read_fold(input_string);
    fold_paper(points, fold);
    std::cout << "points count after one fold " << points.size() << std::endl << std::endl;

    while (std::getline(input_stream, input_string)) {
        auto fold = read_fold(input_string);
        fold_paper(points, fold);
    }

    draw_points(points);
    return 0;
}
