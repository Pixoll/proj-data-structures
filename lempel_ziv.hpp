#pragma once

#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class lempel_ziv {
public:
    typedef vector<pair<int, int>> compressed_t;

    static compressed_t compress(const string &input) {
        unordered_map<char, set<int>> chars;
        compressed_t compressed;

        for (int i = 0; i < input.length();) {
            set<int> &positions = chars[input[i]];
            if (positions.empty()) {
                positions.insert(i);
                compressed.emplace_back(input[i], 0);
                i++;
                continue;
            }

            int chosen_position;
            int max_length = 0;

            for (const int position: positions) {
                int length = 1;
                for (int j = 1; j < input.length() - i; j++) {
                    const set<int> &indices = chars[input[i + j]];
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
            for (int j = 1; j < max_length; j++)
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
