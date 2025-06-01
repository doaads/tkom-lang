#include "input_manager.h"

#include <istream>
#include <utility>

InputManager::InputManager(std::shared_ptr<Position> position, std::shared_ptr<std::istream> input)
    : position(std::move(position)), input_stream(std::move(input)) {}

auto InputManager::get_next_char() -> char {
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

auto InputManager::save_position() const -> Position { return *position; }

void InputManager::unget() { handed_back = true; }

auto InputManager::end() const -> bool { return eof; }

void InputManager::skip_line() {
    while (last_char != '\n' && last_char != EOF) {
        get_next_char();
    }
}
