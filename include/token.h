#pragma once

#include "position.h"
#include "tokens.h"
#include "visitor.h"
#include <optional>
#include <string>
#include <variant>
#include <iostream>

class Visitor;

struct Token {
    TokenType type;
    std::optional<std::variant<std::string, int, double>> value;
    Position position;

    void accept(Visitor& visitor);
};

std::ostream& operator<<(std::ostream& os, const Token& token);
