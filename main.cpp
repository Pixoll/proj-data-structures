#include "huffman.hpp"
#include "lempel_ziv.hpp"
#include "performance.hpp"
#include "random_char_gen.hpp"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

int main() {
    if (!filesystem::exists("data"))
        filesystem::create_directory("data");

    random_char_gen random_char(1);
    stringstream out_hm, out_lz;
    string input;
    performance p;

    out_hm << "length,encode_t,decode_t,bits,match\n";
    out_lz << "length,compress_t,decompress_t,bits,match\n";

    for (int length = 1; length <= 1000000; length++) {
        input += random_char();

        for (int _ = 0; _ < 100; ++_) {
            huffman h;

            p.start();
            const string &encoded = h.encode(input);
            const uint64 encode_time = p.end();

            p.start();
            const string &decoded = h.decode(encoded);
            const uint64 decode_time = p.end();

            p.start();
            const lempel_ziv::compressed_t &compressed = lempel_ziv::compress(input);
            const uint64 compress_time = p.end();

            p.start();
            const string &decompressed = lempel_ziv::decompress(compressed);
            const uint64 decompress_time = p.end();

            out_hm << length << ","
                   << encode_time << ","
                   << decode_time << ","
                   << encoded.length() << ","
                   << (decoded == input) << "\n";

            out_lz << length << ","
                   << compress_time << ","
                   << decompress_time << ","
                   << compressed.size() * 8 << ","
                   << (decompressed == input) << "\n";
        }
    }

    ofstream file_hm("data/huffman.csv"), file_lz("data/lempel_ziv.csv");

    file_hm << out_hm.rdbuf();
    file_lz << out_lz.rdbuf();

    file_hm.close();
    file_lz.close();

    return 0;
}
