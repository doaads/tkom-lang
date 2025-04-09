#pragma once

#include "position.h"
#include "tokens.h"
#include "visitor.h"
#include <string>
#include <variant>
#include <iostream>

class Visitor;

struct Token {
    TokenType type;
    std::variant<std::monostate, std::string, int, double, bool> value;
    Position position;

    void accept(Visitor& visitor);
    std::string get_value();
};

std::ostream& operator<<(std::ostream& os, const Token& token);
