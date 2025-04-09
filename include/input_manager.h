#pragma once

#include "position_manager.h"
#include "input_stream.h"
#include <memory>

class InputManager {
    private:
        std::shared_ptr<PositionManager> position;
        std::shared_ptr<InputStream> input_stream;
        char last_char;
        bool handed_back = false;
        bool eof = false;
    public:
        InputManager(std::shared_ptr<PositionManager> position, std::shared_ptr<InputStream> input);
        char get_next_char();
        Position save_position() const;
        void skip_line();
        void unget();
        bool end() const;
        std::weak_ptr<PositionManager> get_position() const;
};
