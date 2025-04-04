#include "position.h"

uint32_t Position::get_line() {
    return line;
}

uint32_t Position::get_column() {
    return column;
}

void Position::increment_line() {
    line++;
}

void Position::increment_column() {
    column++;
}
