#include "huffman.hpp"
#include "lempel_ziv.hpp"
#include "lorem_ipsum_supplier.hpp"
#include "performance.hpp"
#include "random_char_gen.hpp"

#include <iostream>
#include <filesystem>
#include <functional>
#include <fstream>
#include <sstream>
#include <string>
#include <utility>

using namespace std;

int main() {
    if (!filesystem::exists("data"))
        filesystem::create_directory("data");

    huffman warmup;
    warmup.decode(warmup.encode("warmup"));

    lempel_ziv::decompress(lempel_ziv::compress("warmup"));

    const pair<string, function<char()>> test_params[] = {
            {"randomized",  random_char_gen(1)},
            {"lorem_ipsum", lorem_ipsum_supplier()},
    };

    for (const auto &[name, generator]: test_params) {
        stringstream out_hm_times, out_lz_times, out_bits;
        string input;
        performance p;

        out_hm_times << "length,encode_t,decode_t\n";
        out_lz_times << "length,compress_t,decompress_t\n";
        out_bits << "length,huffman,lempel-ziv\n";

        for (int length = 1; length <= 1000; length++) {
            input += generator();

            for (int i = 0; i < 50; i++) {
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

        ofstream file_hm_times("data/huffman_" + name + ".csv");
        ofstream file_lz_times("data/lempel_ziv_" + name + ".csv");
        ofstream file_bits("data/bits_" + name + ".csv");

        file_hm_times << out_hm_times.rdbuf();
        file_lz_times << out_lz_times.rdbuf();
        file_bits << out_bits.rdbuf();

        file_hm_times.close();
        file_lz_times.close();
        file_bits.close();
    }

    return 0;
}
