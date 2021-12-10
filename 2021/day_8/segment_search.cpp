#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <sstream>

namespace {
using LengthCharacters = std::unordered_map<std::size_t, std::vector<std::string>>;
using Mapping = std::pair<std::string, std::string>;
using Segments = std::vector<std::string>;
using SegmentsIterator = Segments::iterator;
using CharacterDigits = std::unordered_map<std::string, int>;

const LengthCharacters length_characters{
    {6, {"abcefg", "abdefg", "abcdfg"}},
    {2, {"cf"}},
    {5, {"acdeg", "acdfg", "abdfg"} },
    {4, {"bcdf"}},
    {3, {"acf"}},
    {7, {"abcdefg"}}
};

const CharacterDigits character_digits{
    {"abcefg", 0},
    {"cf", 1},
    {"acdeg", 2},
    {"acdfg", 3},
    {"bcdf", 4},
    {"abdfg", 5},
    {"abdefg", 6},
    {"acf", 7},
    {"abcdefg", 8},
    {"abcdfg", 9}
};

const std::string Splitter = " | ";

Mapping find_mapping(
    SegmentsIterator segments_iterator,
    SegmentsIterator segments_iterator_end,
    Mapping mapping) {
    if (segments_iterator == segments_iterator_end)
        return mapping;
    
    auto segment_character_variants = length_characters.at(segments_iterator->size());
    for (auto segment_character_variant : segment_character_variants) {
        bool variant_is_possible = true;
        std::string available_variant_characters;
        for (const auto variant_character : segment_character_variant) {
            const auto mapping_index = mapping.second.find(variant_character);
            if (mapping_index != std::string::npos) {
                if (segments_iterator->find(mapping.first.at(mapping_index)) == std::string::npos) {
                    variant_is_possible = false;
                    break;
                }
            } else {
                available_variant_characters += variant_character;
            }
        }

        if (!variant_is_possible) {
            continue;
        }

        std::string segment_chars_to_map;
        for (const auto segment_character : *segments_iterator) {
            const auto mapping_index = mapping.first.find(segment_character);
            if (mapping_index != std::string::npos) {
                if (segment_character_variant.find(mapping.second.at(mapping_index)) == std::string::npos) {
                    variant_is_possible = false;
                    break;
                }
            } else {
                segment_chars_to_map += segment_character;
            }
        }

        if (!variant_is_possible) {
            continue;
        }

        const auto first_permutation = available_variant_characters;
        do {
            auto new_mapping = mapping;
            new_mapping.first += segment_chars_to_map;
            new_mapping.second += available_variant_characters;
            const auto final_mapping = find_mapping(segments_iterator + 1, segments_iterator_end, new_mapping);
            if (!final_mapping.first.empty()) {
                return final_mapping;
            }

            std::next_permutation(begin(available_variant_characters), end(available_variant_characters));
        } while (available_variant_characters != first_permutation);
    }

    return {};
}

Segments parse_segments(const std::string& input_string) {
    std::istringstream segments_stream{input_string};
    Segments segments;
    std::string segment;
    while (std::getline(segments_stream, segment, ' '))
        segments.push_back(segment);
    return segments;
}

}

int main(int argc, char *argv[]) {
    const std::string input_path = argv[1];
    std::ifstream input_stream{input_path};
    std::string input_string;
    int total_sum = 0;
    while (std::getline(input_stream, input_string)) {
        const auto splitter_position = input_string.find(Splitter);
        const auto first_input_part = input_string.substr(0, splitter_position);
        const auto second_input_part = input_string.substr(splitter_position + Splitter.size());
        const auto first_segments = parse_segments(first_input_part);
        const auto second_segments = parse_segments(second_input_part);
        auto segments = first_segments;
        std::copy(begin(second_segments), end(second_segments), std::back_inserter(segments));
        std::sort(begin(segments), end(segments), [](const std::string& a, const std::string& b) { return a.size() < b.size(); });
        const auto mapping = find_mapping(begin(segments), end(segments), {});
        int number = 0;
        for (const auto& digit_segment : second_segments) {
            std::string mapped_segment;
            for (const auto segment_character : digit_segment) {
                const auto mapping_position = mapping.first.find(segment_character);
                mapped_segment += mapping.second.at(mapping_position);
            }

            std::sort(begin(mapped_segment), end(mapped_segment));
            number *= 10;
            number += character_digits.at(mapped_segment);
        }

        total_sum += number;
    }

    std::cout << "total sum " << total_sum << std::endl;
    return 0;
}
