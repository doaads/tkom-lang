#pragma once

#include <cstdint>
#include <ostream>

/**
 * @brief a class representing a position in the source document
 */
class Position {
   private:
    uint32_t line;
    uint32_t column;

   public:
    Position();
    Position(uint32_t line, uint32_t column);
    
    /**
     * @brief get the current line
     */
    [[nodiscard]] auto get_line() const noexcept -> uint32_t;

    /**
     * @brief increment the current line, resetting the column
     */
    auto increment_line() -> uint32_t;

    /**
     * @brief get the current column
     */
    [[nodiscard]] auto get_column() const noexcept -> uint32_t;

    /**
     * @brief increment the current column
     */
    auto increment_column() -> uint32_t;

    /**
     * @brief adjust the position for a given character
     *
     * Increment the current column for most characters, and line for newlines
     *
     * @param ch the character to be checked
     */
    void adjust_position(char ch);
    friend auto operator<<(std::ostream &os, Position pos) -> std::ostream &;
};
