#include "parser.h"

BlockPtr Parser::parse_block() {
    if (current_token.get_type() != TokenType::T_LBLOCK)
        throw std::runtime_error("Expected '{'");

    next_token();
    TokenType type = current_token.get_type();
    std::vector<StatementPtr> statements;

    while (type != TokenType::T_RBLOCK) {
        StatementPtr statement = parse_statement();
        if (!statement)
            throw std::runtime_error("Expected statement or }");
        statements.push_back(statement);
    }
    return std::make_unique<Block>(statements);
}

StatementPtr Parser::parse_statement() {
    StatementPtr statement = parse_conditional_statement(TokenType::T_IF);
    if (statement) return statement;

    statement = parse_while_loop();
    if (statement) return statement;

    statement = parse_for_loop();
    if (statement) return statement;

    statement = parse_ret_statement();
    if (statement) return statement;

    statement = parse_call_statement();
    if (statement) return statement;

    return nullptr;
}

StatementPtr Parser::parse_assign() {
    ExprPtr expr = parse_or_expression();
    if (!expr)
        return nullptr;

    next_token();
    if (current_token.get_type() != TokenType::T_ASSIGN)
        //throw std::runtime_error("Expected '=>'");
        return nullptr;

    bool mut = false;
    next_token();
    if (current_token.get_type() == TokenType::T_MUT) {
        mut = true;
        next_token();
    }

    std::optional<Type> type = translate_token_to_type(current_token.get_type());
    if (type && *type == Type::VOID)
        throw std::runtime_error("void type is not compatible with assign statement");

    std::optional<VarType> variable_type;
    if (type) {
        variable_type = VarType{*type, mut};
    }

    next_token();
    if (current_token.get_type() != TokenType::T_IDENTIFIER)
        throw std::runtime_error("Expected identifier");

    std::string identifier = current_token.get_value<std::string>();

    next_token();
    if (current_token.get_type() != TokenType::T_SEMICOLON)
        throw std::runtime_error("Expected ;");

    return std::make_unique<AssignStatement>(std::move(expr), variable_type, identifier);
}

StatementPtr Parser::parse_while_loop() {
    next_token();
    if (current_token.get_type() != TokenType::T_WHILE)
        return nullptr;

    next_token();
    ExprPtr condition = parse_condition();

    next_token();
    BlockPtr block = parse_block();
    if (!block)
        throw std::runtime_error("Expected block");

    return std::make_unique<WhileLoopStatement>(
            std::move(condition), std::move(block));
}

/*
    for_loop    = for, for_loop_args, block, call_op, bind_front, ";"
*/

StatementPtr Parser::parse_for_loop() {
    next_token();
    if (current_token.get_type() != TokenType::T_FOR)
        return nullptr;

    next_token();
    ForLoopArgsPtr args = parse_for_loop_args();
    if (!args)
        throw std::runtime_error("Expected arguments");

    next_token();
    BlockPtr block = parse_block();
    if (!block)
        throw std::runtime_error("Expected block");

    next_token();
    if (current_token.get_type() != TokenType::T_CALL)
        throw std::runtime_error("Expected '->'");

    // TODO: Edit this to accept bindfront here
    next_token();
    if (current_token.get_type() != TokenType::T_IDENTIFIER)
        throw std::runtime_error("Expected identifier");

    std::string identifier = current_token.get_value<std::string>();

    next_token();
    if (current_token.get_type() != TokenType::T_SEMICOLON)
        throw std::runtime_error("Expected ;");

    return std::make_unique<ForLoopStatement>(
            std::move(args),
            std::move(block),
            identifier);
}

/*
    for_loop_args = "(", (identifier | assignment), ";", expression, ")"
*/

ForLoopArgsPtr Parser::parse_for_loop_args() {
    if (current_token.get_type() != TokenType::T_LPAREN)
        throw std::runtime_error("Expected (");

    next_token();
    StatementPtr assign = parse_assign();
    std::string identifier;
    if (!assign && current_token.get_type() == TokenType::T_IDENTIFIER) {
        identifier = current_token.get_value<std::string>();
    } else {
        throw std::runtime_error("Expected assign or identifier");
    }

    std::variant<std::monostate, StatementPtr, std::string> iterator;
    if (assign) {
        iterator = std::move(assign);
    } else {
        iterator = identifier;
    }

    next_token();
    if (current_token.get_type() != TokenType::T_SEMICOLON)
        throw std::runtime_error("Expected ;");
    
    next_token();
    ExprPtr condition = parse_or_expression();
    if (!condition)
        throw std::runtime_error("Expected expression");

    next_token();
    if (current_token.get_type() != TokenType::T_RPAREN)
        throw std::runtime_error("Expected )");


    ForLoopArgsPtr args = std::make_unique<ForLoopArgs>();
    args->iterator = std::move(iterator);
    args->condition = std::move(condition);
    return args;
}

/*
 * if_statement = if, condition, block, [elif_st | else_st]
 * elif_st      = elif, condition, block, [elif_st | else_st]
*/

StatementPtr Parser::parse_conditional_statement(TokenType st_type) {
    next_token();
    if (current_token.get_type() != st_type)
        return nullptr;

    next_token();
    ExprPtr condition = parse_condition();

    next_token();
    BlockPtr block = parse_block();
    if (!block)
        throw std::runtime_error("Expected block after conditional statement");

    StatementPtr else_st = parse_conditional_statement(TokenType::T_ELIF);
    if (!else_st)
        else_st = parse_else_statement();

    if (!else_st)
        return std::make_unique<ConditionalStatement>(
                TokenType::T_IF,
                std::move(condition),
                std::move(block)
                );

    return std::make_unique<ConditionalStatement>(
            TokenType::T_IF,
            std::move(condition),
            std::move(block),
            std::move(else_st)
            );
}

/*
 * else_st      = else, block
*/

StatementPtr Parser::parse_else_statement() {
    next_token();
    if (current_token.get_type() != TokenType::T_ELSE)
        return nullptr;

    BlockPtr block = parse_block();
    if (!block)
        throw std::runtime_error("Expected block after else");

    return std::make_unique<ElseStatement>(std::move(block));
}


/*
 * condition    = "(", expresion, ")"
*/

ExprPtr Parser::parse_condition() {
    if (current_token.get_type() != TokenType::T_LPAREN)
        throw std::runtime_error("Expected '('");

    ExprPtr condition = parse_or_expression();

    if (!condition)
        throw std::runtime_error("Expected expression");

    next_token();
    if (current_token.get_type() != TokenType::T_RPAREN)
        throw std::runtime_error("Expected ')'");

    return condition;
}

/*
 * or_expression = and_expression, {logical_or, and_expression}
*/

ExprPtr Parser::parse_or_expression() {
    ExprPtr left = parse_and_expression();
    if (!left) {
        return nullptr;
    }
    next_token();
    while (current_token.get_type() == TokenType::T_OR) {
        next_token();
        ExprPtr right = parse_and_expression();
        if (!right)
            throw std::runtime_error("Error while parsing OR. Expected identifier");
        left = std::make_unique<BinaryExpr>(
                std::move(left),
                TokenType::T_OR,
                std::move(right));
    }
    return left;
}

/*
 * and_expression = comp_expression, {logical_and, comp_expression}
*/

ExprPtr Parser::parse_and_expression() {
    ExprPtr left = parse_comp_expression();
    if (!left) {
        return nullptr;
    }
    while (current_token.get_type() == TokenType::T_AND) {
        next_token();
        ExprPtr right = parse_comp_expression();
        if (!right)
            throw std::runtime_error("Error while parsing AND. Expected identifier");
        left = std::make_unique<BinaryExpr>(
                std::move(left),
                TokenType::T_AND,
                std::move(right));
    }
    return left;
}

/*
 * comp_expression = additive_expression, [comp_operator, additive_expression]
*/

ExprPtr Parser::parse_comp_expression() {
    ExprPtr left = parse_additive_expression();
    if (!left) {
        return nullptr;
    }
    TokenType type = current_token.get_type();
    if (is_comparative(type)) {
        next_token();
        ExprPtr right = parse_additive_expression();
        if (!right)
            throw std::runtime_error("Error while parsing comp. Expected identifier");
        left = std::make_unique<BinaryExpr>(
                std::move(left),
                type,
                std::move(right));
    }
    return left;
}

/*
 * additive_expression = term, {add_sub_op, term}
*/

ExprPtr Parser::parse_additive_expression() {
    ExprPtr left = parse_term();
    if (!left) {
        return nullptr;
    }
    TokenType op_type = current_token.get_type();
    while (op_type == TokenType::T_PLUS || op_type == TokenType::T_MINUS) {
        next_token();
        ExprPtr right = parse_term();
        if (!right)
            throw std::runtime_error("Error while parsing additive. Expected identifier");
        left = std::make_unique<BinaryExpr>(
                std::move(left),
                op_type,
                std::move(right));
        op_type = current_token.get_type();
    }
    return left;
}

/*
 * term = unary_factor, {mult_div_op, unary_factor}
*/

ExprPtr Parser::parse_term() {
    ExprPtr left = parse_unary();
    if (!left) {
        return nullptr;
    }
    next_token();
    TokenType op_type = current_token.get_type();
    if (op_type == TokenType::T_MULT || op_type == TokenType::T_DIV) {
        next_token();
        ExprPtr right = parse_unary();
        if (!right)
            throw std::runtime_error("Error while parsing terms. Expected identifier");
        left = std::make_unique<BinaryExpr>(
                std::move(left),
                op_type,
                std::move(right));
        op_type = current_token.get_type();
    }
    return left;
}

/*
 * unary_factor = [unary_operator], factor
*/

ExprPtr Parser::parse_unary() {
    TokenType type = current_token.get_type();
    std::optional<TokenType> unary_op;
    if (type == TokenType::T_MINUS || type == TokenType::T_NOT) {
        next_token();
        unary_op = type;
    }
    ExprPtr right = parse_factor();
    if (!right)
        throw std::runtime_error("Expected factor after unary operator");
    return unary_op ?
        std::make_unique<UnaryExpr>(*unary_op, std::move(right))
        : std::move(right);
}

/*
 * factor = function_call
 *        | number
 *        | identifier
 *        | string
 *        | "(" expression ")"
*/

ExprPtr Parser::parse_factor() {
    ExprPtr factor = parse_func_call();
    if (factor) return factor;

    TokenType type = current_token.get_type();

    if (type == TokenType::T_LPAREN) {
        next_token();
        factor = parse_or_expression();
        if (current_token.get_type() != TokenType::T_RPAREN)
            throw std::runtime_error("Expected ')' after expression");
        return factor;
    }

    if (is_factor(type)) {
        return std::make_unique<LiteralExpr>(current_token);
    }

    return nullptr;
}

ExprPtr Parser::parse_func_call() {
    std::optional<std::vector<ExprPtr>> args = parse_func_args();
    if (!args) {
        return nullptr;
    }
    next_token();
    if (current_token.get_type() != TokenType::T_CALL) {
        return nullptr;
    }
    next_token();
    if (current_token.get_type() != TokenType::T_IDENTIFIER) {
        throw std::runtime_error("Error while parsing func call. expected identifier");
   }
    std::string name = current_token.get_value<std::string>();
    return std::make_unique<CallExpr>(name, args.value());
}

std::optional<std::vector<ExprPtr>> Parser::parse_func_args() {
    if (current_token.get_type() != TokenType::T_LPAREN) {
        return std::nullopt;
    }
    std::vector<ExprPtr> args;
    next_token();
    TokenType type = current_token.get_type();
    while (type == TokenType::T_COMMA || type != TokenType::T_RPAREN) {
        std::optional<ExprPtr> arg = parse_or_expression();
        if (!arg) {
            throw std::runtime_error("error while parsing func args");
        }
        args.push_back(*arg);
        next_token();
        type = current_token.get_type();
    }
    return std::move(args);
}


bool Parser::is_comparative(TokenType type) const {
    switch (type) {
        case TokenType::T_NEQ: return true;
        case TokenType::T_EQ: return true;
        case TokenType::T_LTE: return true;
        case TokenType::T_GTE: return true;
        case TokenType::T_LT: return true;
        case TokenType::T_GT: return true;
        default: return false;
    }
}

bool Parser::is_factor(TokenType type) const {
    switch (type) {
        case TokenType::T_INT: return true;
        case TokenType::T_FLT: return true;
        case TokenType::T_BOOL: return true;
        case TokenType::T_IDENTIFIER: return true;
        case TokenType::T_STRING: return true;
        default: return false;
    }
}

bool Parser::is_type(TokenType type) const {
    switch (type) {
        case TokenType::T_INT_TYPE: return true;
        case TokenType::T_FLT_TYPE: return true;
        case TokenType::T_STRING_TYPE: return true;
        case TokenType::T_BOOL_TYPE: return true;
        default: return false;
    }
}

bool Parser::is_type_or_void(TokenType type) const {
    return (is_type(type) || type == TokenType::T_VOID_TYPE);
}

std::optional<Type> Parser::translate_token_to_type(TokenType type) const {
    switch(type) {
        case TokenType::T_INT_TYPE: return Type::INT;
        case TokenType::T_FLT_TYPE: return Type::INT;
        case TokenType::T_STRING_TYPE: return Type::STRING;
        case TokenType::T_BOOL_TYPE: return Type::BOOL;
        case TokenType::T_VOID_TYPE: return Type::VOID;
        default: return std::nullopt;
    }
}

void Parser::next_token() {
    current_token = lexer->get_token();
}
