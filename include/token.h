#pragma once

#include <iostream>
#include <string>
#include <variant>

#include "position.h"
#include "tokens.h"

using ValueType = std::variant<std::monostate, std::string, int, double, bool>;

/**
 * @brief class representing a single token
 */
class Token {
   private:
    TokenType type;  //< the type of the token
    ValueType value;  //< the value of the token
    Position position;  //< the position of the token

   public:
    Token() = default;
    /**
     * @brief Construct the token with a given type
     */
    Token(TokenType type);

    /**
     * @brief get the value of a token
     */
    template <typename T>
    [[nodiscard]] auto get_value() const -> T {
        return std::get<T>(value);
    }

    /**
     * @brief get the value of the token as a `std::variant` (`ValueType`)
     */
    [[nodiscard]] auto get_value() const -> ValueType;
    template <typename T>
    void set_value(T new_value) {
        value = new_value;
    }

    /**
     * @brief get the type of the token
     */
    [[nodiscard]] auto get_type() const -> TokenType;

    /**
     * @brief set the type of the token
     * @param new_type the new type of the token
     */
    void set_type(TokenType new_type);

    /**
     * @brief get the token's position
     */
    [[nodiscard]] auto get_position() const -> Position;

    /**
     * @brief set the token's position
     */
    void set_position(Position pos);

    friend auto operator<<(std::ostream &os, const Token &token) -> std::ostream &;
};
