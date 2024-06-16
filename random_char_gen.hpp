#pragma once

#include <random>

using namespace std;

class random_char_gen {
private:
    mt19937_64 engine = mt19937_64(random_device{}());
    uniform_int_distribution<char> generator;

public:
    explicit random_char_gen(char min = 0, char max = 127) : generator(min, max) {
    }

    ~random_char_gen() = default;

    char operator()() {
        return this->generator(engine);
    }
};
