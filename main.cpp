#include "huffman.hpp"
#include "lempel_ziv.hpp"
#include "lempel_ziv_fast.hpp"
#include "lorem_ipsum_supplier.hpp"
#include "performance.hpp"

#include <iostream>
#include <filesystem>
#include <functional>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

int main(const int argc, const char *argv[]) {
    if (filesystem::exists("data"))
        filesystem::remove_all("data");

    filesystem::create_directory("data");

    huffman warmup;
    warmup.decode(warmup.encode("tangananica-tanganana"));
    lempel_ziv::decompress(lempel_ziv::compress("tangananica-tanganana"));
    lempel_ziv_fast::decompress(lempel_ziv_fast::compress("tangananica-tanganana"));

    const int tests = argc > 1 ? strtol(argv[1], nullptr, 10) : 100;
    const int length_step = argc > 2 ? strtol(argv[2], nullptr, 10) : 100;
    const int max_iterations = argc > 3 ? strtol(argv[3], nullptr, 10) : 20;

    const int max_length = length_step * tests;

    lorem_ipsum_supplier lorem_ipsum;
    stringstream out_e_vs_c, out_de_vs_dc, out_bits;
    string input;
    performance p;

    out_e_vs_c << "length,huffman,lempel-ziv greedy,lempel-ziv fast\n";
    out_de_vs_dc << "length,huffman,lempel-ziv greedy,lempel-ziv fast\n";
    out_bits << "length,huffman,lempel-ziv greedy,lempel-ziv fast\n";

    int last_length = 0;
    for (int length = length_step; length <= max_length; length += length_step) {
        cout << "test " << length / length_step << endl;

        while (last_length < length) {
            input += lorem_ipsum();
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

            p.start();
            const lempel_ziv_fast::compressed_t &compressed_fast = lempel_ziv_fast::compress(input);
            const uint64 compress_fast_time = p.end();

            p.start();
            lempel_ziv_fast::decompress(compressed_fast);
            const uint64 decompress_fast_time = p.end();

            out_e_vs_c << length << ","
                       << encode_time << ","
                       << compress_time << ","
                       << compress_fast_time << "\n";

            out_de_vs_dc << length << ","
                         << decode_time << ","
                         << decompress_time << ","
                         << decompress_fast_time << "\n";

            if (i == 0) {
                out_bits << length << ","
                         << encoded.length() << ","
                         << compressed.size() * 8 << ","
                         << compressed_fast.size() * 8 << "\n";
            }
        }
    }

    ofstream file_e_vs_c("data/encode_vs_compress.csv");
    ofstream file_de_vs_dc("data/decode_vs_decompress.csv");
    ofstream file_bits("data/bits.csv");

    file_e_vs_c << out_e_vs_c.rdbuf();
    file_de_vs_dc << out_de_vs_dc.rdbuf();
    file_bits << out_bits.rdbuf();

    file_e_vs_c.close();
    file_de_vs_dc.close();
    file_bits.close();

    return 0;
}
