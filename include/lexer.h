#pragma once

#include <memory>

#include "tokenizer.h"

/**
 * @brief A tokenizer interface producing tokens from an input stream
 */
class Lexer {
   private:
    bool verbose = false;  //< whether we want verbose logging
    bool eof = false;  //< whether we have reached end of file
    std::shared_ptr<Position> position;  //< the current position
    std::shared_ptr<InputManager> input;  //< the input manager, for managing the input file
    std::unique_ptr<Tokenizer> tokenizer;  //< the main tokenizer

   public:

    /**
     * @brief Lexer's constructor, verbosity set to false by default
     */
    Lexer(std::shared_ptr<std::istream> input_stream);

    /**
     * @brief Lexer's constructor, verbosity set by a parameter
     * @param verbose Whether we want verbose logging
     */
    Lexer(std::shared_ptr<std::istream> input_stream, bool verbose);

    /**
     * @brief get the next token from the tokenizer
     */
    Token get_token();

    /**
     * @brief function returning true if the entire file is read
     */
    bool end() const;
};
