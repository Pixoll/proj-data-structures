#pragma once

#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class lempel_ziv_gfg {
public:
    typedef vector<int> compressed_t;

    static compressed_t compress(const string &input) {
        unordered_map<string, int> table;
        for (int i = 0; i <= 255; i++)
            table[string(1, char(i))] = i;

        string previous(1, input[0]);
        int code = 256;
        compressed_t compressed;

        for (int i = 0; i < input.length(); i++) {
            string current;
            if (i != input.length() - 1)
                current += input[i + 1];

            if (table.find(previous + current) != table.end()) {
                previous += current;
                continue;
            }

            compressed.push_back(table[previous]);
            table[previous + current] = code++;
            previous = current;
        }

        compressed.push_back(table[previous]);

        return compressed;
    }

    static string decompress(const compressed_t &compressed) {
        unordered_map<int, string> table;
        for (int i = 0; i <= 255; i++)
            table[i] += char(i);

        string decompressed;
        int previous = compressed[0];
        string substring = table[previous];
        char c = substring[0];

        decompressed += substring;
        int code = 256;

        for (int i = 0; i < compressed.size() - 1; i++) {
            const int current = compressed[i + 1];

            if (table.find(current) == table.end()) {
                substring = table[previous];
                substring += c;
            } else {
                substring = table[current];
            }

            decompressed += substring;
            c = substring[0];

            table[code] = table[previous] + c;
            code++;

            previous = current;
        }

        return decompressed;
    }

    static void write_compressed_to_file(const compressed_t &compressed, const string &file_name) {
        ofstream file(file_name, ios::out | ios::binary);
        file.write(reinterpret_cast<const char *>(compressed.data()), compressed.size() * sizeof(compressed[0]));
        file.close();
    }

    static void write_decompressed_to_file(const string &decompressed, const string &file_name) {
        ofstream file(file_name, ios::out | ios::binary);
        file.write(decompressed.data(), decompressed.length());
        file.close();
    }
};
