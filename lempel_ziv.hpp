#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

typedef unsigned long long uint64;

class lempel_ziv {
public:
    typedef vector<pair<uint64, uint64>> compressed_t;

    // TODO: compressed size is not quite there yet
    static compressed_t compress(const string &input) {
        unordered_map<char, uint64> chars;
        compressed_t compressed;

        for (uint64 i = 0; i < input.length();) {
            const char c1 = input[i];
            if (chars.count(c1) == 0) {
                chars[c1] = i;
                compressed.emplace_back(c1, 0);
                i++;
                continue;
            }

            const uint64 position = chars[c1];
            uint64 length = 1;
            for (uint64 j = 1; j < input.length() - i; j++) {
                const char c2 = input[i + j];
                if (chars.count(c2) == 0 || chars[c2] != position + j)
                    break;

                length++;
            }

            compressed.emplace_back(position, length);
            i += length;
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

            const uint64 position = first;

            if (position + length <= decompressed.size()) {
                decompressed += decompressed.substr(position, length);
                continue;
            }

            cerr << "Error: Invalid position or length in compressed data\n"
                 << "length: " << length << "\n"
                 << "position: " << position << "\n"
                 << "decompressed.size(): " << decompressed.size() << endl;
            return "";
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
