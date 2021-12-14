#include <string>
#include <fstream>
#include <unordered_map>
#include <sstream>
#include <iostream>

namespace {
constexpr std::size_t part_one_iterations = 10;
constexpr std::size_t part_two_iterations = 40;

using InsertionRules = std::unordered_map<std::string, char>;
using CharacterCount = std::unordered_map<char, std::size_t>;

std::string read_polymer_template(std::ifstream& input_stream) {
    std::string input_string;
    std::getline(input_stream, input_string);
    const auto polymer_template = input_string;
    std::getline(input_stream, input_string);
    return polymer_template;
}

InsertionRules read_insertion_rules(std::ifstream& input_stream) {
    InsertionRules insertion_rules;
    std::string input_string;
    while (std::getline(input_stream, input_string)) {
        const auto key = input_string.substr(0, 2);
        const auto value = input_string.back();
        insertion_rules[key] = value;
    }

    return insertion_rules;
}

std::string apply_insertion_rules(
        const std::string& polymer_template,
        const InsertionRules& insertion_rules,
        std::size_t count) {
    auto next_template = polymer_template;
    for (std::size_t i = 0; i < count; ++i) {
        std::stringstream iteration_template;
        iteration_template << next_template.front();
        for (std::size_t character_position = 1; character_position < next_template.size(); ++character_position) {
            const auto prev_character_position = character_position - 1;
            const auto rule_key = next_template.substr(prev_character_position, 2);
            iteration_template << insertion_rules.at(rule_key);
            iteration_template << next_template.at(character_position);
        }

        next_template = iteration_template.str();
    }

    return next_template;
}

std::size_t get_min_max_difference(const std::string& polymer) {
    CharacterCount character_count;
    for (const auto character : polymer) {
        ++character_count[character];
    }

    std::size_t min_count = std::numeric_limits<std::size_t>::max();
    std::size_t max_count = 0;
    for (const auto& count : character_count) {
        min_count = std::min(min_count, count.second);
        max_count = std::max(max_count, count.second);
    }

    return max_count - min_count;
}

}

int main(int argc, char *argv[]) {
    const std::string input_path = argv[1];
    std::ifstream input_stream{input_path};
    const auto polymer_template = read_polymer_template(input_stream);
    const auto insertion_rules = read_insertion_rules(input_stream);

    const auto adapted_polymer = apply_insertion_rules(polymer_template, insertion_rules, part_one_iterations);
    std::cout << "part 1 answer " << get_min_max_difference(adapted_polymer) << std::endl;
    return 0;
}
