#pragma once

#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

typedef unsigned long long uint64;

class lempel_ziv {
public:
    typedef vector<pair<uint64, uint64>> compressed_t;

    static compressed_t compress(const string &input) {
        unordered_map<char, set<uint64>> chars;
        compressed_t compressed;

        for (uint64 i = 0; i < input.length();) {
            set<uint64> &positions = chars[input[i]];
            if (positions.empty()) {
                positions.insert(i);
                compressed.emplace_back(input[i], 0);
                i++;
                continue;
            }

            uint64 chosen_position;
            uint64 max_length = 0;

            for (const uint64 position: positions) {
                uint64 length = 1;
                for (uint64 j = 1; j < input.length() - i; j++) {
                    const set<uint64> &indices = chars[input[i + j]];
                    if (indices.empty() || indices.find(position + j) == indices.end())
                        break;

                    length++;
                }

                if (length > max_length) {
                    max_length = length;
                    chosen_position = position;
                }
            }

            positions.insert(i);
            for (int j = 1; j < max_length; ++j)
                chars[input[i + j]].insert(i + j);

            compressed.emplace_back(chosen_position, max_length);
            i += max_length;
        }

        return compressed;
    }

    static string decompress(const compressed_t &compressed) {
        string decompressed;

        for (const auto &[first, length]: compressed) {
            if (length == 0) {
                decompressed += char(first);
                continue;
            }

            decompressed += decompressed.substr(first, length);
        }

        return decompressed;
    }

    static string compressed_to_string(const compressed_t &compressed) {
        stringstream output;
        for (const auto &[first, length]: compressed) {
            output << "(";
            if (length == 0)
                output << char(first);
            else
                output << first;
            output << "," << length << ") ";
        }
        return output.str();
    }
};
