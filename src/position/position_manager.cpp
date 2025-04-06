#include "position_manager.h"

Position PositionManager::save_position() {
    return position;
}

void PositionManager::adjust_position(char ch) {
    if (ch == '\n') {
        position.increment_line();
    } else {
        position++;
    }
}

