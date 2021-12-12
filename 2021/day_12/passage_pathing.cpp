#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <unordered_set>
#include <iostream>

namespace {
constexpr auto cave_connector = '-';
constexpr std::size_t multi_visit_max_cnt = 2;
const std::string start_cave = "start";
const std::string end_cave = "end";

using Cave = std::string;
using CaveConnections = std::vector<Cave>;
using CaveGraph = std::unordered_map<Cave, CaveConnections>;
using CavesConnection = std::pair<Cave, Cave>;
using VisitedCaves = std::unordered_map<Cave, std::size_t>;

bool is_small_cave(const Cave& cave) {
    return cave.front() >= 'a' && cave.front() <= 'z';
}

CavesConnection read_caves_connection(const std::string& input_string) {
    const auto connector_position = input_string.find(cave_connector);
    const Cave first_cave = input_string.substr(0, connector_position);
    const Cave second_cave = input_string.substr(connector_position + 1);
    return CavesConnection{first_cave, second_cave};
}

CaveGraph read_cave_graph(const std::string& input_path) {
    CaveGraph cave_graph;
    std::ifstream input_stream{input_path};
    std::string input_string;
    while (std::getline(input_stream, input_string)) {
        const auto caves_connection = read_caves_connection(input_string);
        cave_graph[caves_connection.first].push_back(caves_connection.second);
        cave_graph[caves_connection.second].push_back(caves_connection.first);
    }
    
    return cave_graph;
}

std::size_t get_path_count(
        const CaveGraph& cave_graph,
        const Cave& from,
        VisitedCaves& visited_caves, 
        const Cave& multi_visit_cave) {
    if (from == end_cave) {
        if (multi_visit_cave.empty() || visited_caves[multi_visit_cave] > 1)
            return 1;
        return 0;
    }

    std::size_t path_count = 0;
    const auto small_cave = is_small_cave(from);
    if (small_cave) {
        ++visited_caves[from];
    }

    for (const auto& connection : cave_graph.at(from)) {
        if (visited_caves[connection] == 0) {
            path_count += get_path_count(cave_graph, connection, visited_caves, multi_visit_cave);
        } else if (connection == multi_visit_cave && visited_caves[connection] < multi_visit_max_cnt) {
            path_count += get_path_count(cave_graph, connection, visited_caves, multi_visit_cave);
        }
    }

    if (small_cave) {
        --visited_caves[from];
    }

    return path_count;
}

}

int main(int argc, char *argv[]) {
    const std::string input_path = argv[1];
    const auto caves_graph = read_cave_graph(input_path);
    
    VisitedCaves visited_caves;
    const auto path_count = get_path_count(caves_graph, start_cave, visited_caves, "");
    std::cout << "path count " << path_count << std::endl;        

    std::size_t multi_visit_path_count = path_count;
    for (const auto& cave : caves_graph) {
        if (cave.first == start_cave || cave.first == end_cave) {
            continue;
        }

        visited_caves.clear();
        multi_visit_path_count += get_path_count(caves_graph, start_cave, visited_caves, cave.first);
    }

    std::cout << "multi visit path count " << multi_visit_path_count << std::endl;
}
