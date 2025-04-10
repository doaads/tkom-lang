#pragma once

#include "position.h"
#include <memory>

class InputManager {
    private:
        std::shared_ptr<Position> position;
        std::shared_ptr<std::istream> input_stream;
        char last_char;
        bool handed_back = false;
        bool eof = false;
    public:
        InputManager(std::shared_ptr<Position> position, std::shared_ptr<std::istream> input);
        char get_next_char();
        Position save_position() const;
        void skip_line();
        void unget();
        bool end() const;
};
