#pragma once

#include <istream>
#include <memory>
#include <variant>
#include "position.h"
#include "tokens.h"
#include "input_stream.h"


struct Token {
    TokenType type;
    std::variant<std::string, int, double> value;
    Position position;
};


class Lexer {
    private:
        std::unique_ptr<InputStream> input_stream;
        Position position;
        void get_next_char();
    public:
        explicit Lexer(std::istream& input);
};
