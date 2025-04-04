#pragma once

#include <cstdint>

class Position {
    private:
        uint32_t line;
        uint32_t column;
    public:
        uint32_t get_line();
        void increment_line();
        uint32_t get_column();
        void increment_column();
};
