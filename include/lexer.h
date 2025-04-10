#pragma once

#include <memory>
#include "tokenizer.h"
#include "visitor.h"

class Lexer {
    private:
        bool verbose = false;
        bool eof = false;
        std::unique_ptr<TokenVisitor> token_visitor;
        std::shared_ptr<Position> position;
        std::shared_ptr<InputManager> input;
        std::unique_ptr<Tokenizer> tokenizer;
    public:
        Lexer(std::shared_ptr<std::istream> input_stream);
        Lexer(std::shared_ptr<std::istream> input_stream, bool verbose);
        Token get_token();
        bool end() const;
};
