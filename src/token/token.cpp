#include "token.h"

void Token::accept(Visitor& visitor) {
    visitor.visit(*this);
}

std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << "[\033[1;32mTOKEN:\033[0m [Type: \033[1;36m" << token.type << "\033[0m, Value: \033[1;36m";
    if (std::holds_alternative<std::string>(token.value)) {
        os << std::get<std::string>(token.value);
    } else if (std::holds_alternative<int>(token.value)) {
        os << std::get<int>(token.value);
    } else if (std::holds_alternative<double>(token.value)){
        os << std::get<double>(token.value);
    } else {
        os << "\033[1;35mNone";
    }
    os << "\033[0m, ";
    os << "Position: \033[1;36m" << token.position << "\033[0m]";
    return os;
}
