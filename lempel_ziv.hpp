#pragma once

#include "trie.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class lempel_ziv {
public:
    typedef pair<int, int> lz_pair;

    // TODO: fails on repeated characters
    static vector<lz_pair> compress(const string &input) {
        trie trie;
        vector<lz_pair> compressed;

        for (int i = 0; i < input.size();) {
            auto [position, length] = trie.search(input, i);

            if (length > 0) {
                compressed.emplace_back(position, length);
                trie.insert(input, i, length + 1);
                i += length;
                continue;
            }

            compressed.emplace_back(input[i], 0);
            trie.insert(input, i, 1);
            i++;
        }

        return compressed;
    }

    static string decompress(const vector<lz_pair> &compressed) {
        string decompressed;

        for (const auto &[first, length]: compressed) {
            if (length == 0) {
                decompressed += char(first);
                continue;
            }

            const int position = first;

            if (position >= 0 && position + length <= decompressed.size()) {
                decompressed += decompressed.substr(position, length);
                continue;
            }

            cerr << "Error: Invalid position or length in compressed data\n"
                 << "length: " << length << "\n"
                 << "position: " << position << "\n"
                 << "decompressed.size(): " << decompressed.size() << "\n";
            return "";
        }

        return decompressed;
    }

    static string compressed_to_string(const vector<lz_pair> &compressed) {
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
