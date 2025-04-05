#pragma once

#include "position.h"
#include "tokens.h"
#include <optional>
#include <string>
#include <variant>

struct Token {
    TokenType type;
    std::optional<std::variant<std::string, int, double>> value;
    Position position;
};
