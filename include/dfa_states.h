#pragma once

enum class DFAState {
    IN_WHITESPACE,
    IN_IDENTIFIER,
    IN_NUMBER,
    IN_FLT,
    IN_STRING,
    IN_ESCAPE,
    IN_LT,    // less than
    IN_GT,    // greater than
    IN_NOT,   // !,  can extend to !=
    IN_EQ,    // =,  can extend to =>
    IN_CALL,  // ->, can extend to ->>
    IN_MINUS, // -,  can extend to ->
    IN_DIV, // /,     can extend to //
    END
};
