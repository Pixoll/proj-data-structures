#include "huffman.hpp"
#include "lempel_ziv_fast.hpp"
#include "lempel_ziv_gfg.hpp"
#include "lorem_ipsum_supplier.hpp"
#include "tests.hpp"

#include <filesystem>
#include <functional>

using namespace std;

int main(const int argc, const char *argv[]) {
    if (filesystem::exists("data"))
        filesystem::remove_all("data");

    if (filesystem::exists("output"))
        filesystem::remove_all("output");

    filesystem::create_directory("data");
    filesystem::create_directory("output");

    const string test_string = "tangananica-tanganana";
    huffman::decode(huffman::encode(test_string));
    lempel_ziv_fast::decompress(lempel_ziv_fast::compress(test_string));
    lempel_ziv_gfg::decompress(lempel_ziv_gfg::compress(test_string));

    const int tests = argc > 1 ? strtol(argv[1], nullptr, 10) : 100;
    const int length_step = argc > 2 ? strtol(argv[2], nullptr, 10) : 1000;
    const int max_iterations = argc > 3 ? strtol(argv[3], nullptr, 10) : 50;

    run_tests("lorem-ipsum", lorem_ipsum_supplier(), tests, length_step, max_iterations);

    return 0;
}
