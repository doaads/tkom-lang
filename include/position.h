#pragma once

#include <cstdint>
#include <ostream>

class Position {
    private:
        bool wrap;
        uint32_t line;
        uint32_t column;
    public:
        Position();
        uint32_t get_line();
        uint32_t increment_line();
        uint32_t get_column();
        uint32_t increment_column();
        int operator++(int);
        friend std::ostream& operator<<(std::ostream& os, Position pos);
};
