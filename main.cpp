#include "huffman_greedy.hpp"
#include "lempel_ziv_gfg.hpp"
#include "tests.hpp"

#include <filesystem>

using namespace std;

int main(const int argc, const char *argv[]) {
    if (filesystem::exists("data"))
        filesystem::remove_all("data");

    if (filesystem::exists("output"))
        filesystem::remove_all("output");

    filesystem::create_directory("data");
    filesystem::create_directory("output");

    const string test_string = "tangananica-tanganana";
    huffman_greedy::decode(huffman_greedy::encode(test_string));
    lempel_ziv_gfg::decompress(lempel_ziv_gfg::compress(test_string));

    const int tests = argc > 1 ? strtol(argv[1], nullptr, 10) : 1;
    const int iterations = argc > 2 ? strtol(argv[2], nullptr, 10) : 20;

    run_tests("lorem-ipsum", "lorem_ipsum.txt", tests, iterations);
    run_tests("duplicate", "duplicate.txt", tests, iterations);
    run_tests("butterfly", "butterfly.jpg", tests, iterations);

    return 0;
}
