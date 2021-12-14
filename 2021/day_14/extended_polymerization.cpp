#include <string>
#include <fstream>
#include <unordered_map>
#include <iostream>

namespace {
using InsertionRules = std::unordered_map<std::string, char>;
using CharacterCount = std::unordered_map<char, unsigned long long>;
using PairsCount = std::unordered_map<std::string, unsigned long long>;

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

PairsCount count_pairs(const std::string& polymer) {
    PairsCount pairs_count;
    for (std::size_t i = 1; i < polymer.size(); ++i) {
        const auto key = polymer.substr(i - 1, 2);
        ++pairs_count[key];
    }

    return pairs_count;
}

unsigned long long grow_polymer_and_get_min_max( 
        const std::string& polymer,
        std::size_t steps,
        const InsertionRules& insertion_rules) {
    auto pairs_count = count_pairs(polymer);
    for (std::size_t i = 0; i < steps; ++i) {
        PairsCount step_pairs_count;
        for (const auto& pair : pairs_count) {
            std::string new_first_pair{pair.first.front()};
            new_first_pair.push_back(insertion_rules.at(pair.first));
            step_pairs_count[new_first_pair] += pair.second;

            std::string new_second_pair{new_first_pair.back()};
            new_second_pair.push_back(pair.first.back());
            step_pairs_count[new_second_pair] += pair.second;
        }
        pairs_count = std::move(step_pairs_count);
    }

    CharacterCount character_count;
    for (const auto& pair_count : pairs_count) {
        character_count[pair_count.first.front()] += pair_count.second;
        character_count[pair_count.first.back()] += pair_count.second;
    }

    ++character_count[polymer.front()];
    ++character_count[polymer.back()];

    unsigned long long min_count = std::numeric_limits<unsigned long long>::max();
    unsigned long long max_count = 0;
    for (const auto& count : character_count) {
        min_count = std::min(min_count, count.second);
        max_count = std::max(max_count, count.second);
    }

    return (max_count - min_count) / 2;
}

}

int main(int argc, char *argv[]) {
    const std::string input_path = argv[1];
    std::ifstream input_stream{input_path};
    const auto polymer_template = read_polymer_template(input_stream);
    const auto insertion_rules = read_insertion_rules(input_stream);
    auto pairs_count = count_pairs(polymer_template);

    std::cout << "after 10 " << grow_polymer_and_get_min_max(polymer_template, 10, insertion_rules) << std::endl;
    std::cout << "after 40 " << grow_polymer_and_get_min_max(polymer_template, 40, insertion_rules) << std::endl;
    return 0;
}
