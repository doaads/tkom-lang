#pragma once

enum class DFAState {
    IN_WHITESPACE,
    IN_IDENTIFIER,
    IN_NUMBER,
    IN_FLT,
    IN_STRING,
    IN_ESCAPE,
    IN_OPERATOR,
    IN_LONG_OPERATOR,
    IN_FIRST_CHAR_LONG_OP,
    IN_SPECIAL_CHAR,
    END
};
