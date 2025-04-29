#include "lexer.h"
#include "exceptions.h"

Lexer::Lexer(std::shared_ptr<std::istream> input_stream) {
    position = std::make_shared<Position>();
    input = std::make_unique<InputManager>(position, std::move(input_stream));
    tokenizer = std::make_unique<Tokenizer>(input);
}

Lexer::Lexer(std::shared_ptr<std::istream> input_stream, bool verbose) : Lexer(input_stream){
    this->verbose = verbose;
}

Token Lexer::get_token() {
    Token result;
    try {
        result = tokenizer->get_token();
    } catch (CompilerException& e) {
        e.set_position(*position);
        throw;
    }
    eof = input->end();
    if (verbose) {
        std::cout << result << std::endl;
    }
    return result;
}

bool Lexer::end() const {
    return eof;
}
