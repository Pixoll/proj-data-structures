#pragma once

#include "trie.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class lempel_ziv_fast {
public:
    typedef pair<int, int> compressed_pair_t;
    typedef vector<compressed_pair_t> compressed_t;

    static compressed_t compress(const string &input) {
        trie trie;
        compressed_t compressed;

        for (int i = 0; i < input.size();) {
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

    static void write_compressed_to_file(const compressed_t &compressed, const string &file_name) {
        ofstream file(file_name, ios::out | ios::binary);
        file.write(reinterpret_cast<const char *>(compressed.data()), compressed.size() * sizeof(compressed_pair_t));
        file.close();
    }
};