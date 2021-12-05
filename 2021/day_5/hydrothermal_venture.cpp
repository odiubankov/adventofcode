#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <stdlib.h> 

struct Point {
    int x = 0;
    int y = 0;
};

struct Line {
    Point a;
    Point b;
};

using Lines = std::vector<Line>;
using Points = std::unordered_map<int, std::unordered_map<int, int>>;

Line read_line(const std::string& input_string) {
    Line line;
    std::size_t start_position = 0;
    std::size_t separator_position = input_string.find(',');
    line.a.x = std::stoi(
        input_string.substr(start_position, separator_position - start_position));
    
    start_position = separator_position + 1;
    separator_position = input_string.find(' ', start_position);
    line.a.y = std::stoi(
        input_string.substr(start_position, separator_position - start_position));

    start_position = separator_position + 4;
    separator_position = input_string.find(',', start_position);
    line.b.x = std::stoi(
        input_string.substr(start_position, separator_position - start_position));

    start_position = separator_position + 1;
    line.b.y = std::stoi(
        input_string.substr(start_position));

    return line;
}

int main(int argc, char *argv[]) {
    const std::string input_path = argv[1];
    std::ifstream input_stream{input_path};

    Lines lines;
    std::string input_string;
    while (std::getline(input_stream, input_string)) {
        lines.push_back(read_line(input_string));
    }

    Points points;
    for (const auto& line : lines) {
        if (line.a.x == line.b.x) {
            for (int y = std::min(line.a.y, line.b.y); y <= std::max(line.a.y, line.b.y); ++y) {
                ++points[line.a.x][y];
            }
        } else if (line.a.y == line.b.y) {
            for (int x = std::min(line.a.x, line.b.x); x <= std::max(line.a.x, line.b.x); ++x) {
                ++points[x][line.a.y];
            }
        } else if (abs(line.a.x - line.b.x) == abs(line.a.y - line.b.y)) {
            const int delta_x = line.a.x < line.b.x ? 1 : -1;
            const int delta_y = line.a.y < line.b.y ? 1 : -1;
            for (int x = line.a.x, y = line.a.y;
                 x != line.b.x && y != line.b.y;
                 x += delta_x, y += delta_y) {
                ++points[x][y];
            }
            ++points[line.b.x][line.b.y];
        }
    }

    std::size_t intersections_count = 0;
    for (const auto& x : points) {
        for (const auto& y : x.second) {
            if (y.second > 1) {
                ++intersections_count;
            }
        }
    }

    std::cout << "intersections count " << intersections_count << std::endl;
    return 0;
}
