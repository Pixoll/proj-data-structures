#pragma

#include "huffman.hpp"
#include "lempel_ziv_fast.hpp"
#include "lempel_ziv_gfg.hpp"
#include "performance.hpp"

#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"

void run_tests(const string &name, const function<char()> &supplier, int tests, int length_step, int max_iterations) {
    cout << "running " << name << " tests" << endl;

    const int max_length = length_step * tests;
    performance p, total;
    stringstream out_e_vs_c, out_de_vs_dc, out_bits;
    string input;

    out_e_vs_c << "length,huffman,lempel-ziv fast,lempel-ziv gfg\n";
    out_de_vs_dc << "length,huffman,lempel-ziv fast,lempel-ziv gfg\n";
    out_bits << "length,huffman,lempel-ziv fast,lempel-ziv gfg\n";

    total.start();

    int last_length = 0;
    for (int length = length_step; length <= max_length; length += length_step) {
        while (last_length < length) {
            input += supplier();
            last_length++;
        }

        for (int i = 0; i < max_iterations; i++) {
            p.start();
            const huffman::encoded_t encoded = huffman::encode(input);
            const uint64_t encode_time = p.end();

            const string &decoded = huffman::decode(encoded);
            const uint64_t decode_time = p.end();

            p.start();
            const lempel_ziv_fast::compressed_t &compressed_fast = lempel_ziv_fast::compress(input);
            const uint64_t compress_fast_time = p.end();

            p.start();
            const string &decompressed_fast = lempel_ziv_fast::decompress(compressed_fast);
            const uint64_t decompress_fast_time = p.end();

            p.start();
            const lempel_ziv_gfg::compressed_t &compressed_gfg = lempel_ziv_gfg::compress(input);
            const uint64_t compress_gfg_time = p.end();

            p.start();
            const string &decompressed_gfg = lempel_ziv_gfg::decompress(compressed_gfg);
            const uint64_t decompress_gfg_time = p.end();

            out_e_vs_c << length << ","
                       << encode_time << ","
                       << compress_fast_time << ","
                       << compress_gfg_time << "\n";

            out_de_vs_dc << length << ","
                         << decode_time << ","
                         << decompress_fast_time << ","
                         << decompress_gfg_time << "\n";

            if (i == 0) {
                const long long encoded_size = huffman::frequencies_offset
                                               + get<3>(encoded).size() * sizeof(huffman::frequency_pair_t)
                                               + get<4>(encoded).size();
                const long long compressed_fast_size = compressed_fast.size() * sizeof(compressed_fast[0]);
                const long long compressed_gfg_size = compressed_gfg.size() * sizeof(compressed_gfg[0]);

                out_bits << length << ","
                         << encoded_size << ","
                         << compressed_fast_size << ","
                         << compressed_gfg_size << "\n";

//                huffman::write_encoded_to_file(
//                        encoded,
//                        "output/encoded_" + to_string(test) + ".bin"
//                );
//                lempel_ziv_fast::write_compressed_to_file(
//                        compressed_fast,
//
//                        "output/compressed_fast_" + to_string(test) + ".bin"
//                );
            }
        }
    }

    ofstream file_e_vs_c("data/" + name + "_encode_vs_compress.csv");
    ofstream file_de_vs_dc("data/" + name + "_decode_vs_decompress.csv");
    ofstream file_bits("data/" + name + "_bytes.csv");

    file_e_vs_c << out_e_vs_c.rdbuf();
    file_de_vs_dc << out_de_vs_dc.rdbuf();
    file_bits << out_bits.rdbuf();

    file_e_vs_c.close();
    file_de_vs_dc.close();
    file_bits.close();

    cout << "finished, took " << total.end<performance::milliseconds>() / 1e3 << " s\n" << endl;
}

#pragma clang diagnostic pop
