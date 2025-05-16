#include "input_manager.h"

#include <istream>

InputManager::InputManager(std::shared_ptr<Position> position, std::shared_ptr<std::istream> input)
    : position(position), input_stream(input), handed_back(false), eof(false) {}

char InputManager::get_next_char() {
    if (handed_back) {
        handed_back = false;
        return last_char;
    }

    last_char = input_stream->get();

    if (last_char == '\r' and input_stream->peek() == '\n') {
        last_char = input_stream->get();
    } else if (last_char == EOF) {
        eof = true;
    }

    position->adjust_position(last_char);
    return last_char;
}

Position InputManager::save_position() const { return *position; }

void InputManager::unget() { handed_back = true; }

bool InputManager::end() const { return eof; }

void InputManager::skip_line() {
    while (last_char != '\n' && last_char != EOF) {
        get_next_char();
    }
}
