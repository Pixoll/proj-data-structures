#include "huffman.hpp"
#include "lempel_ziv.hpp"
#include "lorem_ipsum_supplier.hpp"
#include "performance.hpp"

#include <iostream>
#include <filesystem>
#include <functional>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

int main() {
    if (filesystem::exists("data"))
        filesystem::remove_all("data");

    filesystem::create_directory("data");

    huffman warmup;
    warmup.decode(warmup.encode("warmup"));

    lempel_ziv::decompress(lempel_ziv::compress("warmup"));

    const int tests = 100;
    const int length_step = 100;
    const int max_length = length_step * tests;
    const int max_iterations = 20;

    lorem_ipsum_supplier generator;

    stringstream out_hm_times, out_lz_times, out_bits;
    string input;
    performance p;

    out_hm_times << "length,encode,decode\n";
    out_lz_times << "length,compress,decompress\n";
    out_bits << "length,huffman,lempel-ziv\n";

    int last_length = 0;
    for (int length = length_step; length <= max_length; length += length_step) {
        cout << "test " << length / 100 << endl;

        while (last_length < length) {
            input += generator();
            last_length++;
        }

        for (int i = 0; i < max_iterations; i++) {
            huffman h;

            p.start();
            const string &encoded = h.encode(input);
            const uint64 encode_time = p.end();

            p.start();
            h.decode(encoded);
            const uint64 decode_time = p.end();

            p.start();
            const lempel_ziv::compressed_t &compressed = lempel_ziv::compress(input);
            const uint64 compress_time = p.end();

            p.start();
            lempel_ziv::decompress(compressed);
            const uint64 decompress_time = p.end();

            out_hm_times << length << ","
                         << encode_time << ","
                         << decode_time << "\n";

            out_lz_times << length << ","
                         << compress_time << ","
                         << decompress_time << "\n";

            if (i == 0) {
                out_bits << length << ","
                         << encoded.length() << ","
                         << compressed.size() * 8 << "\n";
            }
        }
    }

    ofstream file_hm_times("data/huffman.csv");
    ofstream file_lz_times("data/lempel_ziv.csv");
    ofstream file_bits("data/bits.csv");

    file_hm_times << out_hm_times.rdbuf();
    file_lz_times << out_lz_times.rdbuf();
    file_bits << out_bits.rdbuf();

    file_hm_times.close();
    file_lz_times.close();
    file_bits.close();

    return 0;
}
