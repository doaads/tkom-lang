#include "position.h"


Position::Position() : line(1), column(1) {}
Position::Position(uint32_t line, uint32_t column) : line(line), column(column) {}

uint32_t Position::get_line() const noexcept {
    return line;
}

uint32_t Position::get_column() const noexcept {
    return column;
}

uint32_t Position::increment_line() {
    column = 1;
    return line++;
}

uint32_t Position::increment_column() {
    return column++;
}

void Position::adjust_position(char ch) {
    if (ch == '\n') {
        increment_line();
    } else {
        increment_column();
    }
}

std::ostream& operator<<(std::ostream& os, Position pos) {
    os << pos.get_line() << ':' << pos.get_column();
    return os;
}
