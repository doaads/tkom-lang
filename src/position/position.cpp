#include "position.h"

Position::Position() : line(1), column(1) {}
Position::Position(uint32_t line, uint32_t column) : line(line), column(column) {}

auto Position::get_line() const noexcept -> uint32_t { return line; }

auto Position::get_column() const noexcept -> uint32_t { return column; }

auto Position::increment_line() -> uint32_t {
    column = 1;
    return line++;
}

auto Position::increment_column() -> uint32_t { return column++; }

void Position::adjust_position(char ch) {
    if (ch == '\n') {
        increment_line();
    } else {
        increment_column();
    }
}

auto operator<<(std::ostream &os, Position pos) -> std::ostream & {
    os << pos.get_line() << ':' << pos.get_column();
    return os;
}
