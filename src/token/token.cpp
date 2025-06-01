#include "token.h"

Token::Token(TokenType type) : type(type) {}

auto Token::get_value() const -> ValueType { return value; }

auto Token::get_type() const -> TokenType { return type; }

void Token::set_type(TokenType new_type) { type = new_type; }

auto Token::get_position() const -> Position { return position; }

void Token::set_position(Position pos) { position = pos; }

template <typename... Ts>
struct Overload : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
Overload(Ts...) -> Overload<Ts...>;
auto operator<<(std::ostream &os, const Token &token) -> std::ostream & {
    os << "[\033[1;32mTOKEN:\033[0m [Type: \033[1;36m" << token.get_type()
       << "\033[0m, Value: \033[1;36m";
    std::visit(Overload{[&os](std::monostate) { os << "\033[1;35mNone"; },
                        [&os](bool rhs) { os << "\033[1;32m" << (rhs ? "true" : "false"); },
                        [&os](auto rhs) { os << rhs; }},
               token.get_value());
    os << "\033[0m, ";
    os << "Position: \033[1;36m" << token.get_position() << "\033[0m]";
    return os;
}
