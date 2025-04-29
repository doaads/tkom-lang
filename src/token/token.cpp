#include "token.h"

Token::Token(TokenType type) : type(type) {}

ValueType Token::get_value() const {
    return value;
}

TokenType Token::get_type() const {
    return type;
}

void Token::set_type(TokenType new_type) {
    type = new_type;
}

Position Token::get_position() const {
    return position;
}

void Token::set_position(Position pos) {
    position = pos;
}

std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << "[\033[1;32mTOKEN:\033[0m [Type: \033[1;36m" << token.get_type() << "\033[0m, Value: \033[1;36m";
        std::visit([&os](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::monostate>) {
            os << "\033[1;35mNone";
        } else if constexpr (std::is_same_v<T, bool>) {
           os << "\033[1;32m" << (arg ? "true" : "false");
        } else {
            os << arg;
        }
    }, token.get_value());
    os << "\033[0m, ";
    os << "Position: \033[1;36m" << token.get_position() << "\033[0m]";
    return os;
}
