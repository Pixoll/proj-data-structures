#pragma once

#include "trie.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class lempel_ziv {
public:
    typedef vector<pair<uint64, uint64>> compressed_t;

    // TODO: fails on repeated characters
    static compressed_t compress(const string &input) {
        trie trie;
        compressed_t compressed;

        for (uint64 i = 0; i < input.size();) {
            auto [position, length] = trie.search(input, i);

            if (length == 0) {
                compressed.emplace_back(input[i], 0);
                trie.insert(input, i, 1);
                i++;
                continue;
            }

            compressed.emplace_back(position, length);
            trie.insert(input, i, length + 1);
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
