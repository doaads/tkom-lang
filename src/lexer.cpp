#include "lexer.h"


Lexer::Lexer(std::shared_ptr<InputStream> input_stream) {
    position = std::make_unique<PositionManager>();
    input = std::make_unique<InputManager>(position, input_stream);
    tokenizer = std::make_unique<Tokenizer>(input);
    token_visitor = std::make_unique<TokenVisitor>();
}

Lexer::Lexer(std::shared_ptr<InputStream> input_stream, bool verbose) : Lexer(input_stream){
    this->verbose = verbose;
}

Token Lexer::get_token() {
    Token result = tokenizer->get_token();
    eof = input->end();
    if (verbose) {
        result.accept(*token_visitor);
    }
    return result;
}

bool Lexer::end() {
    return eof;
}
