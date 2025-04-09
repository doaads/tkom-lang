#include "position.h"


Position::Position() : wrap(false), line(1), column(0) {}

uint32_t Position::get_line() const noexcept {
    return line;
}

uint32_t Position::get_column() const noexcept {
    return column;
}

uint32_t Position::increment_line() {
    wrap = true;
    return line;
}

uint32_t Position::increment_column() {
    if (wrap) {
        wrap = false;
        column = 1;
        line++;
        return column;
    }
    return column++;
}

int Position::operator++(int) {
    return increment_column();
}

std::ostream& operator<<(std::ostream& os, Position pos) {
    os << pos.get_line() << ':' << pos.get_column();
    return os;
}
