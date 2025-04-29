#include "tokens.h"

std::ostream& operator<<(std::ostream& os, const TokenType type) {
    switch (type) {
        case TokenType::T_INT_TYPE: os << "T_INT_TYPE"; break;
        case TokenType::T_FLT_TYPE: os << "T_FLT_TYPE"; break;
        case TokenType::T_STRING_TYPE: os << "T_STRING_TYPE"; break;
        case TokenType::T_VOID_TYPE: os << "T_VOID_TYPE"; break;
        case TokenType::T_RET: os << "T_RET"; break;
        case TokenType::T_WHILE: os << "T_WHILE"; break;
        case TokenType::T_FOR: os << "T_FOR"; break;
        case TokenType::T_IF: os << "T_IF"; break;
        case TokenType::T_ELIF: os << "T_ELIF"; break;
        case TokenType::T_ELSE: os << "T_ELSE"; break;
        case TokenType::T_MUT: os << "T_MUT"; break;
        case TokenType::T_BINDFRT: os << "T_BINDFRT"; break;
        case TokenType::T_DECORATE: os << "T_DECORATE"; break;
        case TokenType::T_CALL: os << "T_CALL"; break;
        case TokenType::T_ASSIGN: os << "T_ASSIGN"; break;
        case TokenType::T_NOT: os << "T_NOT"; break;
        case TokenType::T_PLUS: os << "T_PLUS"; break;
        case TokenType::T_MINUS: os << "T_MINUS"; break;
        case TokenType::T_MULT: os << "T_MULT"; break;
        case TokenType::T_DIV: os << "T_DIV"; break;
        case TokenType::T_AND: os << "T_AND"; break;
        case TokenType::T_OR: os << "T_OR"; break;
        case TokenType::T_LTE: os << "T_LTE"; break;
        case TokenType::T_GTE: os << "T_GTE"; break;
        case TokenType::T_EQ: os << "T_EQ"; break;
        case TokenType::T_NEQ: os << "T_NEQ"; break;
        case TokenType::T_LT: os << "T_LT"; break;
        case TokenType::T_GT: os << "T_GT"; break;
        case TokenType::T_IDENTIFIER: os << "T_IDENTIFIER"; break;
        case TokenType::T_INT: os << "T_INT"; break;
        case TokenType::T_FLT: os << "T_FLT"; break;
        case TokenType::T_STRING: os << "T_STRING"; break;
        case TokenType::T_LPAREN: os << "T_LPAREN"; break;
        case TokenType::T_RPAREN: os << "T_RPAREN"; break;
        case TokenType::T_LBLOCK: os << "T_LBLOCK"; break;
        case TokenType::T_RBLOCK: os << "T_RBLOCK"; break;
        case TokenType::T_SEMICOLON: os << "T_SEMICOLON"; break;
        case TokenType::T_COMMA: os << "T_COMMA"; break;
        case TokenType::T_WILDCARD: os << "T_WILDCARD"; break;
        case TokenType::T_COMMENT: os << "T_COMMENT"; break;
        case TokenType::T_ERROR: os << "\033[1;31mT_ERROR\033[0m"; break;
        case TokenType::T_EOF: os << "T_EOF"; break;
        case TokenType::T_FUNC_SIGN: os << "T_FUNC_SIGN"; break;
        case TokenType::T_LFTYPE: os << "T_LFTYPE"; break;
        case TokenType::T_RFTYPE: os << "T_RFTYPE"; break;
        case TokenType::T_BOOL_TYPE: os << "T_BOOL_TYPE"; break;
        case TokenType::T_BOOL: os << "T_BOOL"; break;
    }
    return os;
}
