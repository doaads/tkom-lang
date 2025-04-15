```cpp
class Factor { //- base
    Factor(FunctionCall call);
    Factor(Token token);
    Factor(Expression expr);
};

class UnaryExpression : public Factor {
    private:
        TokenType op_type;
        Factor right;
    public:
        UnaryExpression(unary_op, right);
};

class Expression : public UnaryExpression {
    private:
        Factor left;
        Factor right;
    public:
        Expression(Factor left, Factor right);
};

class Term : public Expression {
    public:
        Term(Factor left, Factor right);
};

class AdditiveExpression : public Expression {
    public:
        AdditiveExpression(Factor left, Factor right);
};

class CompExpression : public Expression {
    public:
        CompExpression(Factor left, Factor right);
};

class AndExpression : public Expression {
    public:
        AndExpression(Factor left, Factor right);
};

class OrExpression : public Expression {
    public:
        OrExpression(Factor left, Factor right);
};
```
