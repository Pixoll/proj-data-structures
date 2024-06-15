#pragma once

#include <string>
#include <utility>

using namespace std;

class trie {
private:
    struct trie_node {
        trie_node *children[128]{};
        int start = 0;
        int length = 0;

        ~trie_node() {
            for (auto &child: children) {
                delete child;
            }
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

    void insert(const string &string, int start, int length) {
        trie_node *current = this->root;

        for (int i = start; i < start + length; ++i) {
            char c = string[i];
            if (current->children[c] == nullptr)
                current->children[c] = new trie_node();

            current = current->children[c];
        }

        current->start = start;
        current->length = length;
    }

    pair<int, int> search(const string &string, int start) {
        trie_node *current = this->root;
        int max_length = 0;
        int position = -1;

        for (int i = start; i < string.size(); ++i) {
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
