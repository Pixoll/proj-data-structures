#pragma once

#include <cmath>
#include <iostream>
#include <fstream>
#include <queue>
#include <stack>
#include <string>
#include <vector>
#include <tuple>
#include <unordered_map>

using namespace std;

class huffman_greedy {
private:
    class tree_node {
    public:
        static const char special_value = -1;

        const char data;
        unsigned int freq;
        tree_node *left = nullptr;
        tree_node *right = nullptr;

        tree_node(char data, unsigned int freq) : data(data), freq(freq) {}

        ~tree_node() {
            delete this->left;
            delete this->right;
        }

        struct compare {
            bool operator()(tree_node *l, tree_node *r) {
                return l->freq > r->freq;
            }
        };
    };

public:
    huffman_greedy() = default;

    ~huffman_greedy() = default;

    static const int frequencies_offset = sizeof(int) * 2 + sizeof(char);

    typedef pair<char, int> frequency_pair_t;
    typedef tuple<int, int, char, vector<frequency_pair_t>, vector<unsigned char>> encoded_t;

    static encoded_t encode(const string &input) {
        unordered_map<char, int> frequency_map;
        for (const char c: input)
            frequency_map[c]++;

        vector<frequency_pair_t> frequency_table(frequency_map.begin(), frequency_map.end());
        tree_node *tree = make_tree(frequency_table);

        unordered_map<char, string> encode_map;
        stack<pair<tree_node *, string>> code_stack;
        code_stack.emplace(tree, "");

        while (!code_stack.empty()) {
            auto [node, code] = code_stack.top();
            code_stack.pop();

            if (node->left == nullptr && node->right == nullptr)
                encode_map[node->data] = code;

            if (node->right != nullptr)
                code_stack.emplace(node->right, code + "1");

            if (node->left != nullptr)
                code_stack.emplace(node->left, code + "0");
        }

        unsigned char byte = 0;
        int bits = 0;
        vector<unsigned char> encoded_data;

        for (const char c: input) {
            for (const char b: encode_map[c]) {
                byte = (byte << 1) | (b - '0');

                if (++bits == 8) {
                    encoded_data.push_back(byte);
                    byte = 0;
                    bits = 0;
                }
            }
        }

        if (bits > 0) {
            byte <<= (8 - bits);
            encoded_data.push_back(byte);
        }

        delete tree;

        return {frequency_table.size(), encoded_data.size(), bits, frequency_table, encoded_data};
    }

    static string decode(const encoded_t &encoded) {
        const auto &[
                _,
                max_bytes,
                last_bit,
                frequency_table,
                encoded_data
        ] = encoded;

        tree_node *tree = make_tree(frequency_table);

        string decoded;
        tree_node *node = tree;
        int bytes = 0;

        for (const unsigned char byte: encoded_data) {
            for (int i = 7; i >= 0; i--) {
                const int bit = (byte >> i) & 1;
                node = bit == 0 ? node->left : node->right;

                if (node->left == nullptr && node->right == nullptr) {
                    decoded += node->data;
                    node = tree;
                }

                if (bytes == max_bytes && 7 - i == last_bit - 1)
                    break;
            }

            bytes++;
        }

        delete tree;

        return decoded;

        read_encoded_from_file("");
    }

    static void write_encoded_to_file(const encoded_t &encoded, const string &file_name) {
        ofstream file(file_name, ios::out | ios::binary);

        file.write(reinterpret_cast<const char *>(&get<0>(encoded)), sizeof(get<0>(encoded)));
        file.write(reinterpret_cast<const char *>(&get<1>(encoded)), sizeof(get<1>(encoded)));
        file.write(reinterpret_cast<const char *>(&get<2>(encoded)), sizeof(get<2>(encoded)));

        for (const auto &[c, f]: get<3>(encoded)) {
            file.write(&c, sizeof(c));
            file.write(reinterpret_cast<const char *>(&f), sizeof(f));
        }

        for (const auto &b: get<4>(encoded))
            file.write(reinterpret_cast<const char *>(&b), sizeof(b));

        file.close();
    }

    static encoded_t read_encoded_from_file(const string &file_name) {
        huffman_greedy::encoded_t encoded;

        ifstream file(file_name, ios::in | ios::binary);

        file.read(reinterpret_cast<char *>(&get<0>(encoded)), sizeof(get<0>(encoded)));
        file.read(reinterpret_cast<char *>(&get<1>(encoded)), sizeof(get<1>(encoded)));
        file.read(reinterpret_cast<char *>(&get<2>(encoded)), sizeof(get<2>(encoded)));

        const int unique_chars = get<0>(encoded);
        get<3>(encoded).reserve(unique_chars);

        for (int _ = 0; _ < unique_chars; _++) {
            huffman_greedy::frequency_pair_t frequency_pair;
            file.read(&frequency_pair.first, sizeof(frequency_pair.first));
            file.read(reinterpret_cast<char *>(&frequency_pair.second), sizeof(frequency_pair.second));
            get<3>(encoded).emplace_back(frequency_pair);
        }

        const int bytes = ceil(float(get<1>(encoded) * 8 + get<2>(encoded)) / 8);
        get<4>(encoded).reserve(bytes);

        for (int _ = 0; _ < bytes; _++) {
            unsigned char byte;
            file.read(reinterpret_cast<char *>(&byte), sizeof(byte));
            get<4>(encoded).push_back(byte);
        }

        file.close();

        return encoded;
    }

    static void write_decoded_to_file(const string &decoded, const string &file_name) {
        ofstream file(file_name, ios::out | ios::binary);
        file.write(decoded.data(), decoded.length());
        file.close();
    }

private:
    static tree_node *make_tree(const vector<frequency_pair_t> &frequency_table) {
        tree_node *left, *right, *top;
        priority_queue<tree_node *, vector<tree_node *>, tree_node::compare> tree;

        for (auto [data, freq]: frequency_table)
            tree.push(new tree_node(data, freq));

        if (tree.size() == 1) {
            left = tree.top();
            tree.pop();

            top = new tree_node(tree_node::special_value, left->freq);
            top->left = left;

            return top;
        }

        while (tree.size() != 1) {
            left = tree.top();
            tree.pop();

            right = tree.top();
            tree.pop();

            top = new tree_node(tree_node::special_value, left->freq + right->freq);
            top->left = left;
            top->right = right;

            tree.push(top);
        }

        return tree.top();
    }

//    static void print_tree(const string &prefix, const tree_node *node, bool is_left) {
//        if (node == nullptr)
//            return;
//
//        cout << prefix
//             << (is_left ? "├──" : "└──")
//             << "(" << int(node->data) << "," << node->freq << ")" << endl;
//
//        print_tree(prefix + (is_left ? "│   " : "    "), node->left, true);
//        print_tree(prefix + (is_left ? "│   " : "    "), node->right, false);
//    }
//
//    static void print_tree(const tree_node *node) {
//        print_tree("", node, false);
//        cout << endl;
//    }
};
