#include "huffman.hpp"
#include "lempel_ziv.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int main() {
    ifstream file("test.txt");
    if (!file.is_open()) {
        cerr << "could not open test.txt, cannot proceed" << endl;
        return 1;
    }

    stringstream buffer;
    buffer << file.rdbuf();
    const string text = buffer.str();

    if (text.empty()) {
        cerr << "test.txt file is empty, cannot proceed" << endl;
        return 1;
    }

    cout << "testing text: " << text.length() * 8 << " bits\n"
         << text << "\n" << endl;

    huffman h;
    const string encoded = h.encode(text);
    const string decoded = h.decode(encoded);

    cout << "huffman encoded: " << encoded.length() << " bits\n"
         << encoded << "\n\n"
         << "decoded matches: " << ((decoded == text) ? "true" : "false") << "\n\n"
         << "===============================================================\n" << endl;

    lempel_ziv lz;
    const string compressed = lz.compress(text);
    const string decompressed = lz.decompress(compressed);

    cout << "lempel-ziv compressed: " << compressed.length() << " bits\n"
         << compressed << "\n\n"
         << "decompressed matches: " << ((decoded == text) ? "true" : "false") << endl;

    return 0;
}
