#include "position.h"


Position::Position() : line(0), column(0), wrap(false) {}

uint32_t Position::get_line() {
    return line;
}

uint32_t Position::get_column() {
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

