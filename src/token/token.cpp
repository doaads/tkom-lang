#include "token.h"

void Token::accept(Visitor& visitor) {
    visitor.visit(*this);
}

std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << "[\e[1;32mTOKEN:\e[0m [Type: \e[1;36m" << token.type << "\e[0m, Value: \e[1;36m";
    if (std::holds_alternative<std::string>(*token.value)) {
        os << std::get<std::string>(*token.value);
    } else if (std::holds_alternative<int>(*token.value)) {
        os << std::get<int>(*token.value);
    } else {
        os << std::get<double>(*token.value);
    }
    os << "\e[0m, ";
    os << "Position: \e[1;36m" << token.position << "\e[0m]" << std::endl;
    return os;
}
