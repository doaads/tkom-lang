#pragma once

enum class Type {
    INT,
    FLT,
    STRING,
    BOOL,
    VOID
};

struct VarType {
    Type type;
    bool mut = false;
};
