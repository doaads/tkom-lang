#pragma once
#include <iostream>

enum class TokenType {
    // keywords
    T_INT_TYPE,
    T_FLT_TYPE,
    T_STRING_TYPE,
    T_VOID_TYPE,
    T_RET,
    T_WHILE,
    T_FOR,
    T_IF,
    T_ELIF,
    T_ELSE,
    T_MUT,

    // operators
    T_BINDFRT,
    T_DECORATE,
    T_CALL,
    T_ASSIGN,
    T_NOT,
    T_PLUS,
    T_MINUS,
    T_MULT,
    T_DIV,
    T_AND,
    T_OR,
    T_LTE,
    T_GTE,
    T_EQ,
    T_NEQ,
    T_LT,
    T_GT,
    T_FUNC_SIGN,

    // identifier
    T_IDENTIFIER,

    // literals
    T_INT,
    T_FLT,
    T_STRING,

    // special characters
    T_LPAREN,
    T_RPAREN,
    T_LBLOCK,
    T_RBLOCK,
    T_LFTYPE,
    T_RFTYPE,
    T_SEMICOLON,
    T_COMMA,
    T_WILDCARD,
    T_COMMENT,

    T_ERROR,
    T_EOF
};

std::ostream& operator<<(std::ostream& os, const TokenType type);
