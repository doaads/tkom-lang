#pragma once

#include <cstdint>
#include <ostream>

class Position {
   private:
    uint32_t line;
    uint32_t column;

   public:
    Position();
    Position(uint32_t line, uint32_t column);
    uint32_t get_line() const noexcept;
    uint32_t increment_line();
    uint32_t get_column() const noexcept;
    uint32_t increment_column();
    void adjust_position(char ch);
    friend std::ostream &operator<<(std::ostream &os, Position pos);
};
