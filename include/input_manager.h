#pragma once

#include <memory>

#include "position.h"

/**
 * @brief class representing the input document and tracking the position in it
 */
class InputManager {
   private:
    std::shared_ptr<Position> position;  //< the current position in the document
    std::shared_ptr<std::istream> input_stream;  //< the input stream
    char last_char;  //< the last character received from the input
    bool handed_back = false;  //< whether the current character has been returned back
    bool eof = false;  //< whether we have reached the end of the file

   public:
    InputManager(std::shared_ptr<Position> position, std::shared_ptr<std::istream> input);
    
    /**
     * @brief get the next character from the input and adjust position
     */
    auto get_next_char() -> char;

    /**
     * return a copy of the current position
     */ 
    [[nodiscard]] auto save_position() const -> Position;

    /**
     * skip until the end of the current line
     */
    void skip_line();

    /**
     * @brief flip the handed_back field, on next character request, return `current_char` instead
     */
    void unget();

    /**
     * @brief returns true if the source document has ended
     */
    [[nodiscard]] auto end() const -> bool;
};
