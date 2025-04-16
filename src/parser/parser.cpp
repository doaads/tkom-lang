#include "parser.h"

BlockType Parser::parse_block() {
    next_token();
    if (current_token.get_type() != TokenType::T_LBLOCK)
        throw std::runtime_error("Expected '{'");

    next_token();
    TokenType type = current_token.get_type();
    std::vector<StatementPtr> statements;
    while (type != TokenType::T_RBLOCK) {
        StatementType statement = parse_statement();
        if (!statement.index())
            throw std::runtime_error("Expected statement or }");
        statements.push_back(std::get<StatementPtr>(statement));
    }
    return std::make_unique<Block>(statements);
}

StatementType Parser::parse_statement() {
    StatementType statement = parse_conditional_statement(TokenType::T_IF);
    if (statement.index()) return statement;

    statement = parse_while_loop();
    if (statement.index()) return statement;

    statement = parse_for_loop();
    if (statement.index()) return statement;

    statement = parse_ret_statement();
    if (statement.index()) return statement;

    statement = parse_call_statement();
    if (statement.index()) return statement;

    return std::monostate();
}

StatementType Parser::parse_conditional_statement(TokenType st_type) {
    next_token();
    if (current_token.get_type() != st_type)
        return std::monostate();

    next_token();
    if (current_token.get_type() != TokenType::T_LPAREN)
        throw std::runtime_error("Expected '('");

    ExprType condition = parse_or_expression();

    if (!condition.index())
        throw std::runtime_error("Expected expression");

    next_token();
    if (current_token.get_type() != TokenType::T_RPAREN)
        throw std::runtime_error("Expected ')'");

    BlockType block = parse_block();
    if (!block.index())
        throw std::runtime_error("Expected block after if");

    StatementType else_st = parse_conditional_statement(TokenType::T_ELIF);
    if (!else_st.index())
        else_st = parse_else_statement();

    if (!else_st.index())
        return std::make_unique<ConditionalStatement>(
                TokenType::T_IF,
                std::move(std::get<ExprPtr>(condition)),
                std::move(std::get<BlockPtr>(block))
                );

    return std::make_unique<ConditionalStatement>(
            TokenType::T_IF,
            std::move(std::get<ExprPtr>(condition)),
            std::move(std::get<BlockPtr>(block)),
            std::move(std::get<StatementPtr>(else_st))
            );
}

StatementType Parser::parse_else_statement() {
    next_token();
    if (current_token.get_type() != TokenType::T_ELSE)
        return std::monostate();

    BlockType block = parse_block();
    if (!block.index())
        throw std::runtime_error("Expected block after else");

    return std::make_unique<ElseStatement>(std::move(std::get<BlockPtr>(block)));
}


ExprType Parser::parse_or_expression() {
    std::variant<std::monostate, ExprPtr> left = parse_and_expression();
    if (!left.index()) {
        return std::monostate();
    }
    next_token();
    while (current_token.get_type() == TokenType::T_OR) {
        next_token();
        std::variant<std::monostate, ExprPtr> right = parse_and_expression();
        if (!right.index())
            throw std::runtime_error("Error while parsing OR. Expected identifier");
        left = std::make_unique<BinaryExpr>(
                std::move(std::get<ExprPtr>(left)),
                TokenType::T_OR,
                std::move(std::get<ExprPtr>(right)));
    }
    return left;
}


ExprType Parser::parse_and_expression() {
    std::variant<std::monostate, ExprPtr> left = parse_comp_expression();
    if (!left.index()) {
        return std::monostate();
    }
    next_token();
    while (current_token.get_type() == TokenType::T_AND) {
        next_token();
        std::variant<std::monostate, ExprPtr> right = parse_comp_expression();
        if (!right.index())
            throw std::runtime_error("Error while parsing AND. Expected identifier");
        left = std::make_unique<BinaryExpr>(
                std::move(std::get<ExprPtr>(left)),
                TokenType::T_AND,
                std::move(std::get<ExprPtr>(right)));
    }
    return left;
}


ExprType Parser::parse_comp_expression() {
    std::variant<std::monostate, ExprPtr> left = parse_additive_expression();
    if (!left.index()) {
        return std::monostate();
    }
    next_token();
    TokenType type = current_token.get_type();
    if (is_comparative(type)) {
        next_token();
        std::variant<std::monostate, ExprPtr> right = parse_additive_expression();
        if (!right.index())
            throw std::runtime_error("Error while parsing comp. Expected identifier");
        left = std::make_unique<BinaryExpr>(
                std::move(std::get<ExprPtr>(left)),
                type,
                std::move(std::get<ExprPtr>(right)));
    }
    return left;
}


ExprType Parser::parse_additive_expression() {
    std::variant<std::monostate, ExprPtr> left = parse_term();
    if (!left.index()) {
        return std::monostate();
    }
    next_token();
    TokenType op_type = current_token.get_type();
    while (op_type == TokenType::T_PLUS || op_type == TokenType::T_MINUS) {
        next_token();
        std::variant<std::monostate, ExprPtr> right = parse_term();
        if (!right.index())
            throw std::runtime_error("Error while parsing additive. Expected identifier");
        left = std::make_unique<BinaryExpr>(
                std::move(std::get<ExprPtr>(left)),
                op_type,
                std::move(std::get<ExprPtr>(right)));
        op_type = current_token.get_type();
    }
    return left;
}

ExprType Parser::parse_term() {
    std::variant<std::monostate, ExprPtr> left = parse_unary();
    if (!left.index()) {
        return std::monostate();
    }
    next_token();
    TokenType op_type = current_token.get_type();
    if (op_type == TokenType::T_MULT || op_type == TokenType::T_DIV) {
        next_token();
        std::variant<std::monostate, ExprPtr> right = parse_unary();
        if (!right.index())
            throw std::runtime_error("Error while parsing terms. Expected identifier");
        left = std::make_unique<BinaryExpr>(
                std::move(std::get<ExprPtr>(left)),
                op_type,
                std::move(std::get<ExprPtr>(right)));
        op_type = current_token.get_type();
    }
    return left;
}


ExprType Parser::parse_unary() {
    TokenType type = current_token.get_type();
    std::variant<std::monostate, TokenType> unary_op;
    if (type == TokenType::T_MINUS || type == TokenType::T_NOT) {
        next_token();
        unary_op = type;
    }
    std::variant<std::monostate, ExprPtr> right = parse_factor();
    if (!right.index())
        throw std::runtime_error("Expected factor after unary operator");
    return unary_op.index() ?
        std::make_unique<UnaryExpr>(std::get<TokenType>(unary_op),
                std::move(std::get<ExprPtr>(right)))
        : std::move(std::get<ExprPtr>(right));
}

ExprType Parser::parse_factor() {
    std::variant<std::monostate, ExprPtr> factor = parse_func_call();
    if (factor.index()) return factor;

    TokenType type = current_token.get_type();

    if (type == TokenType::T_LPAREN) {
        next_token();
        factor = parse_or_expression();
        // throw if monostate?
        next_token();
        // throw if not rparen
        return factor;
    }

    if (is_factor(type)) {
        return std::make_unique<LiteralExpr>(current_token);
    }

    return std::monostate();
}

ExprType Parser::parse_func_call() {
    std::variant<std::monostate, std::vector<ExprPtr>> args = parse_func_args();
    if (!args.index()) {
        return std::monostate();
    }
    next_token();
    if (current_token.get_type() != TokenType::T_CALL) {
        return std::monostate();
    }
    next_token();
    if (current_token.get_type() != TokenType::T_IDENTIFIER) {
        throw std::runtime_error("Error while parsing func call. expected identifier");
    }
    std::string name = current_token.get_value<std::string>();
    return std::make_unique<CallExpr>(name, std::get<std::vector<ExprPtr>>(args));
}

std::variant<std::monostate, std::vector<ExprPtr>> Parser::parse_func_args() {
    if (current_token.get_type() != TokenType::T_LPAREN) {
        return std::monostate();
    }
    std::vector<ExprPtr> args;
    next_token();
    TokenType type = current_token.get_type();
    while (type == TokenType::T_COMMA || type != TokenType::T_RPAREN) {
        std::variant<std::monostate, ExprPtr> arg = parse_or_expression();
        if (!arg.index()) {
            throw std::runtime_error("error while parsing func args");
        }
        args.push_back(std::get<ExprPtr>(arg));
        next_token();
        type = current_token.get_type();
    }
    return args;
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
        case TokenType::T_IDENTIFIER: return true;
        case TokenType::T_STRING: return true;
        default: return false;
    }
}

void Parser::next_token() {
    current_token = lexer->get_token();
}
