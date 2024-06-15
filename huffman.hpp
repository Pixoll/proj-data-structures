#pragma once

#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <vector>
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

    min_heap_node *tree_root = nullptr;

public:
    huffman() = default;

    ~huffman() {
        delete this->tree_root;
    }

    string encode(const string &input) {
        unordered_map<char, uint64> frequencies;
        for (const char c: input)
            frequencies[c]++;

        this->tree_root = make_min_heap(frequencies);
        unordered_map<char, string> encode_map;
        stack<pair<min_heap_node *, string>> code_stack;
        code_stack.emplace(this->tree_root, "");

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

        string encoded_data;
        for (const char c: input)
            encoded_data += encode_map[c];

        return encoded_data;
    }

    string decode(const string &encoded) {
        if (this->tree_root == nullptr) {
            cerr << "huffman: encode something first" << endl;
            exit(1);
        }

        string decoded;
        min_heap_node *node = this->tree_root;

        for (const char c: encoded) {
            node = c == '0' ? node->left : node->right;

            if (node->left == nullptr && node->right == nullptr) {
                decoded += node->data;
                node = this->tree_root;
            }
        }

        return decoded;
    }

private:
    static min_heap_node *make_min_heap(const unordered_map<char, uint64> &frequencies) {
        min_heap_node *left, *right, *top;
        priority_queue<min_heap_node *, vector<min_heap_node *>, min_heap_node::compare> min_heap;

        for (auto [data, freq]: frequencies)
            min_heap.push(new min_heap_node(data, freq));

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
};
