#include "token.h"

void Token::accept(Visitor& visitor) {
    visitor.visit(*this);
}

std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << "[\033[1;32mTOKEN:\033[0m [Type: \033[1;36m" << token.type << "\033[0m, Value: \033[1;36m";
        std::visit([&os](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::monostate>) {
            os << "\033[1;35mNone";
        } else if constexpr (std::is_same_v<T, bool>) {
           os << "\033[1;32m" << (arg ? "true" : "false");
        } else {
            os << arg;
        }
    }, token.value);
    os << "\033[0m, ";
    os << "Position: \033[1;36m" << token.position << "\033[0m]";
    return os;
}
