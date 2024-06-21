#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

class char_supplier {
private:
    string data;
    unsigned long long index = 0;

public:
    explicit char_supplier(const string &file_name) {
        ifstream file("input/" + file_name, ios::in | ios::binary);
        if (!file.is_open()) {
            cerr << "could not open " << file_name << ", cannot proceed" << endl;
            exit(1);
        }

        ostringstream buffer;
        buffer << file.rdbuf();
        data = buffer.str();
    }

    ~char_supplier() = default;

    unsigned long long size() {
        return this->data.length();
    }

    char operator()() {
        return this->data[this->index++];
    }
};
