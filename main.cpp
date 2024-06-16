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
        stringstream out_hm, out_lz;
        string input;
        performance p;

        out_hm << "length,encode_t,decode_t,bits\n";
        out_lz << "length,compress_t,decompress_t,bits\n";

        for (int length = 1; length <= 1000; length++) {
            input += generator();

            for (int _ = 0; _ < 50; ++_) {
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

                out_hm << length << ","
                       << encode_time << ","
                       << decode_time << ","
                       << encoded.length() << "\n";

                out_lz << length << ","
                       << compress_time << ","
                       << decompress_time << ","
                       << compressed.size() * 8 << "\n";
            }
        }

        ofstream file_hm("data/huffman_" + name + ".csv");
        ofstream file_lz("data/lempel_ziv_" + name + ".csv");

        file_hm << out_hm.rdbuf();
        file_lz << out_lz.rdbuf();

        file_hm.close();
        file_lz.close();
    }

    return 0;
}
