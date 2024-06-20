#pragma once

#include <string>
#include <utility>

using namespace std;

typedef unsigned long long uint64;

class trie {
private:
    struct trie_node {
        trie_node *children[128]{};
        uint64 start = 0;
        uint64 length = 0;

        ~trie_node() {
            for (auto &child: children)
                delete child;
        }
    };

    trie_node *root;

public:
    trie() {
        this->root = new trie_node();
    }

    ~trie() {
        delete this->root;
    }

    void insert(const string &string, uint64 start, uint64 length) {
        trie_node *current = this->root;

        for (uint64 i = start; i < start + length; i++) {
            char c = string[i];
            if (current->children[c] == nullptr)
                current->children[c] = new trie_node();

            current = current->children[c];
        }

        current->start = start;
        current->length = length;
    }

    pair<uint64, uint64> search(const string &string, uint64 start) {
        trie_node *current = this->root;
        uint64 max_length = 0;
        uint64 position = -1;

        for (uint64 i = start; i < string.size(); i++) {
            char c = string[i];
            if (current->children[c] == nullptr)
                break;

            current = current->children[c];
            if (current->length > max_length) {
                max_length = current->length;
                position = current->start;
            }
        }

        return {position, max_length};
    }
};
