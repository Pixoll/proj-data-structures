#pragma once

#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <vector>
#include <tuple>
#include <unordered_map>

using namespace std;

typedef unsigned long long uint64;

class huffman {
private:
    class min_heap_node {
    public:
        static const char special_value = -1;

        const char data;
        uint64 freq;
        min_heap_node *left = nullptr;
        min_heap_node *right = nullptr;

        min_heap_node(char data, uint64 freq) : data(data), freq(freq) {}

        ~min_heap_node() {
            delete this->left;
            delete this->right;
        }

        struct compare {
            bool operator()(min_heap_node *l, min_heap_node *r) {
                return l->freq > r->freq;
            }
        };
    };

public:
    huffman() = default;

    ~huffman() = default;

    static const int frequencies_offset = sizeof(uint64) * 2 + sizeof(char);
    typedef tuple<uint64, uint64, char, vector<pair<char, uint64>>, vector<unsigned char>> encoded_t;

    static encoded_t encode(const string &input) {
        unordered_map<char, uint64> frequency_map;
        for (const char c: input)
            frequency_map[c]++;

        const uint64 unique_chars = frequency_map.size();
        vector<pair<char, uint64>> frequency_table;
        frequency_table.reserve(unique_chars);
        for (const auto &[c, f]: frequency_map)
            frequency_table.emplace_back(c, f);

        min_heap_node *tree = make_min_heap(frequency_table);
        unordered_map<char, string> encode_map;
        stack<pair<min_heap_node *, string>> code_stack;
        code_stack.emplace(tree, "");

        while (!code_stack.empty()) {
            auto [node, code] = code_stack.top();
            code_stack.pop();

            if (node->data != min_heap_node::special_value)
                encode_map[node->data] = code;

            if (node->right != nullptr)
                code_stack.emplace(node->right, code + "1");

            if (node->left != nullptr)
                code_stack.emplace(node->left, code + "0");
        }

        uint64 bytes = 0;
        int bits = 0;
        vector<unsigned char> encoded_data(1, 0);

        for (const char c: input) {
            for (const char b: encode_map[c]) {
                const int bit = b == '1' ? 1 : 0;
                encoded_data[bytes] |= bit << 7;

                if (++bits == 8) {
                    bits = 0;
                    bytes++;
                    encoded_data.push_back(0);
                } else {
                    encoded_data[bytes] >>= 1;
                }
            }
        }

        if (bits == 0)
            encoded_data.resize(bytes);

        delete tree;

        return {unique_chars, bytes, bits, frequency_table, encoded_data};
    }

    static string decode(const encoded_t &encoded) {
        const auto &[_, max_bytes, last_bit, frequency_table, encoded_data] = encoded;

        min_heap_node *tree = make_min_heap(frequency_table);

        string decoded;
        min_heap_node *node = tree;

        uint64 bytes = 0;

        for (const unsigned char c: encoded_data) {
            for (int i = 0; i < 8; i++) {
                if (bytes == max_bytes && i > last_bit)
                    break;

                const int bit = c & (1 << i);
                node = bit == 0 ? node->left : node->right;

                if (node->left == nullptr && node->right == nullptr) {
                    decoded += node->data;
                    node = tree;
                }
            }

            bytes++;
        }

        delete tree;

        return decoded;
    }

private:
    static min_heap_node *make_min_heap(const vector<pair<char, uint64>> &frequency_table) {
        min_heap_node *left, *right, *top;
        priority_queue<min_heap_node *, vector<min_heap_node *>, min_heap_node::compare> min_heap;

        for (auto [data, freq]: frequency_table)
            min_heap.push(new min_heap_node(data, freq));

        if (min_heap.size() == 1) {
            left = min_heap.top();
            min_heap.pop();

            top = new min_heap_node(min_heap_node::special_value, left->freq);
            top->left = left;

            return top;
        }

        while (min_heap.size() != 1) {
            left = min_heap.top();
            min_heap.pop();

            right = min_heap.top();
            min_heap.pop();

            top = new min_heap_node(min_heap_node::special_value, left->freq + right->freq);
            top->left = left;
            top->right = right;

            min_heap.push(top);
        }

        return min_heap.top();
    }

//    static void print_tree(const string &prefix, const min_heap_node *node, bool isLeft) {
//        if (node == nullptr)
//            return;
//
//        cout << prefix
//             << (isLeft ? "├──" : "└──")
//             << "(" << int(node->data) << "," << node->freq << ")" << endl;
//
//        print_tree(prefix + (isLeft ? "│   " : "    "), node->left, true);
//        print_tree(prefix + (isLeft ? "│   " : "    "), node->right, false);
//    }
//
//    static void print_tree(const min_heap_node *node) {
//        print_tree("", node, false);
//        cout << endl;
//    }
};
