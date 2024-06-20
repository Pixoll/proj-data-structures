#include "huffman.hpp"
#include "lempel_ziv_fast.hpp"
#include "lempel_ziv_gfg.hpp"
#include "lorem_ipsum_supplier.hpp"
#include "performance.hpp"

#include <iostream>
#include <filesystem>
#include <functional>
#include <sstream>
#include <string>

using namespace std;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"

int main(const int argc, const char *argv[]) {
    if (filesystem::exists("data"))
        filesystem::remove_all("data");

    if (filesystem::exists("output"))
        filesystem::remove_all("output");

    filesystem::create_directory("data");
    filesystem::create_directory("output");

    huffman::decode(huffman::encode("tangananica-tanganana"));
    lempel_ziv_fast::decompress(lempel_ziv_fast::compress("tangananica-tanganana"));
    lempel_ziv_gfg::decompress(lempel_ziv_gfg::compress("tangananica-tanganana"));

    const int tests = argc > 1 ? strtol(argv[1], nullptr, 10) : 100;
    const int length_step = argc > 2 ? strtol(argv[2], nullptr, 10) : 100;
    const int max_iterations = argc > 3 ? strtol(argv[3], nullptr, 10) : 1;

    const int max_length = length_step * tests;

    lorem_ipsum_supplier lorem_ipsum;
    stringstream out_e_vs_c, out_de_vs_dc, out_bits;
    string input;
    performance p;

    out_e_vs_c << "length,huffman,lempel-ziv fast,lempel-ziv gfg\n";
    out_de_vs_dc << "length,huffman,lempel-ziv fast,lempel-ziv gfg\n";
    out_bits << "length,huffman,lempel-ziv fast,lempel-ziv gfg\n";

    int last_length = 0;
    for (int length = length_step; length <= max_length; length += length_step) {
        const int test = length / length_step;
        cout << "test " << test << endl;

        while (last_length < length) {
            input += lorem_ipsum();
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
                cout << "huffman: " << (decoded == input ? "true" : "false") << "\n"
                     << "lz fast: " << (decompressed_fast == input ? "true" : "false") << "\n"
                     << "lz gfg: " << (decompressed_gfg == input ? "true" : "false") << "\n";

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

    ofstream file_e_vs_c("data/encode_vs_compress.csv");
    ofstream file_de_vs_dc("data/decode_vs_decompress.csv");
    ofstream file_bits("data/bytes.csv");

    file_e_vs_c << out_e_vs_c.rdbuf();
    file_de_vs_dc << out_de_vs_dc.rdbuf();
    file_bits << out_bits.rdbuf();

    file_e_vs_c.close();
    file_de_vs_dc.close();
    file_bits.close();

    return 0;
}

#pragma clang diagnostic pop
