#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

class lorem_ipsum_supplier {
private:
    string lorem_ipsum;
    int index = 0;

public:
    lorem_ipsum_supplier() {
        ifstream file("input/lorem_ipsum.txt");
        if (!file.is_open()) {
            cerr << "could not open lorem_ipsum.txt, cannot proceed" << endl;
            exit(1);
        }

        stringstream buffer;
        buffer << file.rdbuf();
        lorem_ipsum = buffer.str();
    }

    ~lorem_ipsum_supplier() = default;

    char operator()() {
        return this->lorem_ipsum[this->index++];
    }
};
