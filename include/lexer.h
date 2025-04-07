#pragma once

#include <memory>
#include "position_manager.h"
#include "input_stream.h"
#include "tokenizer.h"
#include "visitor.h"

class Lexer {
    private:
        bool verbose = false;
        bool eof = false;
        std::unique_ptr<TokenVisitor> token_visitor;
        std::shared_ptr<PositionManager> position;
        std::shared_ptr<InputManager> input;
        std::unique_ptr<Tokenizer> tokenizer;
    public:
        Lexer(std::shared_ptr<InputStream> input_stream);
        Lexer(std::shared_ptr<InputStream> input_stream, bool verbose);
        Token get_token();
        bool end();
};
