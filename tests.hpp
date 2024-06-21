#pragma

#include "char_supplier.hpp"
#include "huffman_greedy.hpp"
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

void run_tests(const string &name, const string &input_file_name, int tests, int length_step, int max_iterations) {
    cout << "running " << name << " tests" << endl;

    performance p, total;
    char_supplier supplier(input_file_name);
    stringstream out_e_vs_c, out_de_vs_dc, out_bits;
    string input;

    out_e_vs_c << "length,huffman greedy,lempel-ziv gfg\n";
    out_de_vs_dc << "length,huffman greedy,lempel-ziv gfg\n";
    out_bits << "length,huffman greedy,lempel-ziv gfg\n";

    total.start();

    if (length_step == -1)
        length_step = supplier.size();

    const int max_length = length_step * tests;
    int last_length = 0;

    for (int length = length_step; length <= max_length; length += length_step) {
        const int test = length / length_step;
        while (last_length < length) {
            input += supplier();
            last_length++;
        }

        for (int i = 0; i < max_iterations; i++) {
            p.start();
            const huffman_greedy::encoded_t encoded = huffman_greedy::encode(input);
            const uint64_t encode_time = p.end();

            const string &decoded = huffman_greedy::decode(encoded);
            const uint64_t decode_time = p.end();

            p.start();
            const lempel_ziv_gfg::compressed_t &compressed_gfg = lempel_ziv_gfg::compress(input);
            const uint64_t compress_gfg_time = p.end();

            p.start();
            const string &decompressed_gfg = lempel_ziv_gfg::decompress(compressed_gfg);
            const uint64_t decompress_gfg_time = p.end();

            out_e_vs_c << length << ","
                       << encode_time << ","
                       << compress_gfg_time << "\n";

            out_de_vs_dc << length << ","
                         << decode_time << ","
                         << decompress_gfg_time << "\n";

            if (i == 0) {
                cout << "huffman greedy: " << (decoded == input ? "true" : "false") << "\n"
                     << "lz gfg: " << (decompressed_gfg == input ? "true" : "false") << "\n";

                const long long encoded_size = huffman_greedy::frequencies_offset
                                               + get<3>(encoded).size() * sizeof(huffman_greedy::frequency_pair_t)
                                               + get<4>(encoded).size();
                const long long compressed_gfg_size = compressed_gfg.size() * sizeof(compressed_gfg[0]);

                out_bits << length << ","
                         << encoded_size << ","
                         << compressed_gfg_size << "\n";

                huffman_greedy::write_encoded_to_file(
                        encoded,
                        "output/" + name + "_encoded_" + to_string(test) + ".bin"
                );
                lempel_ziv_gfg::write_compressed_to_file(
                        compressed_gfg,

                        "output/" + name + "_compressed_gfg_" + to_string(test) + ".bin"
                );

                ofstream file_1("output/decoded_" + input_file_name, ios::out | ios::binary);
                file_1.write(decoded.data(), decoded.length());
                file_1.close();

                ofstream file_2("output/decompressed_" + input_file_name, ios::out | ios::binary);
                file_2.write(decompressed_gfg.data(), decompressed_gfg.length());
                file_2.close();
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

void run_tests(const string &name, const string &input_file_name, int iterations) {
    run_tests(name, input_file_name, 1, -1, iterations);
}

#pragma clang diagnostic pop
