#pragma once

#include "token.h"

class Token;

class Visitor {
    public:
        virtual ~Visitor() {}
        virtual void visit(Token& token) = 0;
};

class TokenVisitor : public Visitor {
    public:
        void visit(Token& token) override;
};
