#include "exceptions.h"
#include "operators.h"
#include "parser.h"

Parser::Parser(std::shared_ptr<Lexer> lexer) : lexer(std::move(lexer)) {}

/*
 *      start_symbol = {function_def}
 */

ProgramPtr Parser::parse() {
    std::vector<std::unique_ptr<Function>> functions;
    FuncPtr function;

    next_token();
    while (auto function = parse_func_def()) {
        functions.push_back(std::move(function));
    }
    shall(is_token(TokenType::T_EOF), "Expected function def");

    ProgramPtr program = std::make_unique<Program>(std::move(functions));
    return program;
}

/*
 *      function_def = func_signature, block
 */

FuncPtr Parser::parse_func_def() {
    FuncSignPtr signature = parse_func_signature();
    if (!signature) return nullptr;

    BlockPtr body = shall(parse_block(), "Expected block");

    return std::make_unique<Function>(std::move(signature), std::move(body));
}

/*
 *      func_signature  = type, identifier, [function_sign_op,
 * function_def_params] func_def_params = func_param, {",",
 * func_param}
 */

FuncSignPtr Parser::parse_func_signature() {
    const Position pos = get_position();
    TypePtr ret_type = parse_type(true);
    if (!ret_type) {
        return nullptr;
    }

    shall(is_token(TokenType::T_IDENTIFIER), "Expected function identifier");

    std::string func_name = current_token.get_value<std::string>();
    std::vector<std::unique_ptr<VariableSignature>> params;

    if (!is_next_token(TokenType::T_FUNC_SIGN)) {
        shall(is_token(TokenType::T_LBLOCK), "Expected '::'");
        return std::make_unique<FuncSignature>(pos, std::move(ret_type), std::move(params),
                                               func_name);
    }

    ParamPtr param;
    do {
        next_token();
        if ((param = shall(parse_func_param(), "Expected parameter")))
            params.push_back(std::move(param));
    } while (is_next_token(TokenType::T_COMMA));

    return std::make_unique<FuncSignature>(pos, std::move(ret_type), std::move(params), func_name);
}

/*
 *      func_param = type_non_void, identifier
 */

ParamPtr Parser::parse_func_param() {
    TypePtr current_arg_type = shall(parse_type(), "Expected type");
    auto identifier = shall(parse_identifier(), "Expected parameter name");
    return std::make_unique<VariableSignature>(std::move(current_arg_type), std::move(identifier));
}

/*
 *      block = "{", {statement}, "}"
 */

BlockPtr Parser::parse_block() {
    if (!is_token(TokenType::T_LBLOCK)) return nullptr;

    std::vector<StatementPtr> statements;

    next_token();
    while (auto statement = parse_statement()) {
        statements.push_back(std::move(statement));
    }
    shall(is_token(TokenType::T_RBLOCK), "Expected }");

    next_token();
    return std::make_unique<Block>(std::move(statements));
}

/*
 *      statement = if_statement
 *                | for_loop
 *                | while_loop
 *                | return_statement
 *                | expression_st
 *                | assignment
 */

StatementPtr Parser::parse_statement() {
    StatementPtr statement = parse_conditional_statement(TokenType::T_IF);
    if (statement) return statement;

    statement = parse_while_loop();
    if (statement) return statement;

    statement = parse_for_loop();
    if (statement) return statement;

    statement = parse_ret_statement();
    if (statement) return statement;

    statement = parse_assign_or_call();
    if (statement) return statement;

    return nullptr;
}

/*
 *      assignment    = expression, assign_op, [type_non_void_mut], identifier,
 * ";" expression_st = expression, ";"
 *
 *      Parsed together due to common construction at start
 */

StatementPtr Parser::parse_assign_or_call() {
    const Position pos = get_position();
    ExprPtr expr = parse_expression();
    if (!expr) return nullptr;

    TokenType op_type = current_token.get_type();
    if (op_type != TokenType::T_ASSIGN) {
        shall(was_last_token(TokenType::T_SEMICOLON), "Expected '=>'");
        return std::make_unique<CallStatement>(pos, std::move(expr));
    }

    next_token();
    TypePtr type = shall(parse_type(), "Expected type");

    auto identifier = shall(parse_identifier(), "Expected identifier");

    ParamPtr signature =
        std::make_unique<VariableSignature>(std::move(type), std::move(identifier));

    shall(is_token(TokenType::T_SEMICOLON), "Expected ';'");
    next_token();

    return std::make_unique<AssignStatement>(pos, std::move(expr), std::move(signature));
}

/*
 *    while_loop = while, condition, block
 */

StatementPtr Parser::parse_while_loop() {
    if (!is_token(TokenType::T_WHILE)) return nullptr;

    const Position pos = get_position();

    next_token();
    ExprPtr condition = parse_condition();

    next_token();
    BlockPtr block = shall(parse_block(), "Expected block");

    return std::make_unique<WhileLoopStatement>(pos, std::move(condition), std::move(block));
}

/*
    for_loop    = for, for_loop_args, block, call_op, bind_front, ";"
*/

StatementPtr Parser::parse_for_loop() {
    if (!is_token(TokenType::T_FOR)) return nullptr;
    const Position pos = get_position();

    next_token();

    ForLoopArgsPtr args = shall(parse_for_loop_args(), "Expected arguments");

    BlockPtr block = shall(parse_block(), "Expected block");

    shall(is_token(TokenType::T_CALL), "Expected ->");
    next_token();

    ExprPtr on_iter = shall(parse_bind_front(), "Expected bind front or identifier");

    shall(was_last_token(TokenType::T_SEMICOLON), "Expected ';'");

    return std::make_unique<ForLoopStatement>(pos, std::move(args), std::move(block),
                                              std::move(on_iter));
}

/*
 *    for_loop_args = "(", (identifier | assignment), ";", expression, ")"
 */

ForLoopArgsPtr Parser::parse_for_loop_args() {
    shall(is_token(TokenType::T_LPAREN), "Expected (");

    next_token();
    StatementPtr assign = parse_assign_or_call();
    std::variant<std::monostate, StatementPtr, ExprPtr> iterator;
    if (!assign) {
        ExprPtr identifier = shall(parse_identifier(), "Expected assign or identifier");
        shall(is_token(TokenType::T_SEMICOLON), "Expected ';'");
    } else {
        iterator = std::move(assign);
    }

    ExprPtr condition = shall(parse_expression(), "Expected expression");

    shall(is_token(TokenType::T_RPAREN), "Expected ')'");

    ForLoopArgsPtr args = std::make_unique<ForLoopArgs>();
    args->iterator = std::move(iterator);
    args->condition = std::move(condition);
    next_token();
    return args;
}

/*
 *    ret_statement = ret, [expression]
 */

StatementPtr Parser::parse_ret_statement() {
    if (!is_token(TokenType::T_RET)) return nullptr;
    const Position pos = get_position();

    next_token();
    ExprPtr expression = parse_expression();

    shall(was_last_token(TokenType::T_SEMICOLON), "Expected ';'");

    return std::make_unique<RetStatement>(pos, std::move(expression));
}

/*
 *    if_statement = if, condition, block, [elif_st | else_st]
 *    elif_st      = elif, condition, block, [elif_st | else_st]
 */

StatementPtr Parser::parse_conditional_statement(TokenType st_type) {
    if (!is_token(st_type)) return nullptr;
    const Position pos = get_position();

    next_token();
    ExprPtr condition = parse_condition();

    next_token();
    BlockPtr block = shall(parse_block(), "Expected block after conditional statement");

    StatementPtr else_st = parse_conditional_statement(TokenType::T_ELIF);
    if (!else_st) else_st = parse_else_statement();

    return std::make_unique<ConditionalStatement>(pos, st_type, std::move(condition),
                                                  std::move(block), std::move(else_st));
}

/*
 *    else_st      = else, block
 */

StatementPtr Parser::parse_else_statement() {
    if (!is_token(TokenType::T_ELSE)) return nullptr;

    const Position pos = get_position();
    next_token();

    BlockPtr block = shall(parse_block(), "Expected block");

    return std::make_unique<ElseStatement>(pos, std::move(block));
}

/*
 *    condition    = "(", expresion, ")"
 */

ExprPtr Parser::parse_condition() {
    shall(is_token(TokenType::T_LPAREN), "Expected '('");
    next_token();
    ExprPtr condition = shall(parse_expression(), "Expected expression");

    shall(is_token(TokenType::T_RPAREN), "Expected ')'");

    return condition;
}

/*
 *    expression = or_expression;
 */

ExprPtr Parser::parse_expression() {
    ExprPtr expr = parse_or_expression();
    return expr;
}

template <typename ConditionFunc, typename NextFunc>
ExprPtr Parser::parse_binary_expr(ConditionFunc condition, NextFunc next, bool limited) {
    ExprPtr left = next();
    if (!left) {
        return nullptr;
    }

    BinaryOp op;
    while (condition()) {
        op = *token_to_binary_op(current_token.get_type());
        next_token();
        ExprPtr right = shall(next(), "Expected expression");
        left = std::make_unique<BinaryExpr>(left->get_position(), std::move(left), op,
                                            std::move(right));
        if (limited) break;
    }

    return left;
}

/*
 *    or_expression = and_expression, {logical_or, and_expression}
 */

ExprPtr Parser::parse_or_expression() {
    return parse_binary_expr([this]() { return is_token(TokenType::T_OR); },
                             [this]() { return parse_and_expression(); });
}

/*
 *    and_expression = comp_expression, {logical_and, comp_expression}
 */

ExprPtr Parser::parse_and_expression() {
    return parse_binary_expr([this]() { return is_token(TokenType::T_AND); },
                             [this]() { return parse_comp_expression(); });
}

/*
 *    comp_expression = additive_expression, [comp_operator,
 * additive_expression]
 */

ExprPtr Parser::parse_comp_expression() {
    return parse_binary_expr([this]() { return is_comparative(current_token.get_type()); },
                             [this]() { return parse_additive_expression(); }, true);
}

/*
 *    additive_expression = term, {add_sub_op, term}
 */

ExprPtr Parser::parse_additive_expression() {
    return parse_binary_expr(
        [this]() { return is_token(TokenType::T_PLUS) || is_token(TokenType::T_MINUS); },
        [this]() { return parse_term(); });
}

/*
 * term = unary_factor, {mult_div_op, unary_factor}
 */

ExprPtr Parser::parse_term() {
    return parse_binary_expr(
        [this]() { return is_token(TokenType::T_MULT) || is_token(TokenType::T_DIV); },
        [this]() { return parse_unary(); });
}

/*
 * unary_factor = [unary_operator], factor
 */

ExprPtr Parser::parse_unary() {
    TokenType type = current_token.get_type();
    const Position pos = get_position();
    std::optional<UnaryOp> unary_op = token_to_unary_op(type);
    if (unary_op) next_token();
    ExprPtr right = parse_factor();
    if (unary_op && !right)
        throw ParserError(get_position(), "Expected factor after unary operator");
    if (unary_op) {
        return std::make_unique<UnaryExpr>(pos, *unary_op, std::move(right));
    } else {
        return right;
    }
}

/*
 * factor = function_call
 *        | bind_front
 *        | decorator
 *        | number
 *        | identifier
 *        | string
 *        | "(" expression ")"
 */

ExprPtr Parser::parse_factor() {
    TokenType type = current_token.get_type();
    if (is_factor(type)) {
        std::unique_ptr<LiteralExpr> expr =
            std::make_unique<LiteralExpr>(get_position(), current_token);
        next_token();
        return expr;
    }

    ExprPtr factor = parse_decorator();
    if (factor) return factor;

    factor = parse_func_call_or_parens();
    if (factor) return factor;

    return nullptr;
}

/*
 *      expr can equal: '(', expr, ')'
 *      this can also be a func arg.
 *      we will parse the first expr and then check whether it is
 *      a valid function call or bindfront
 *
 *      function_call = function_args, call_op, bind_front
 */

ExprPtr Parser::parse_func_call_or_parens() {
    auto args = parse_func_args();
    if (!args) return nullptr;

    ExprPtr bindfrt = parse_bindfrt_or_call(*args);
    if (bindfrt) return bindfrt;

    if (args->empty()) throw ParserError(get_position(), "Expected expression");

    return std::move(args->front());
}

/*
 *      we have parsed the arguments, now
 *      after parsing args, parse either call or bind front
 */

ExprPtr Parser::parse_bindfrt_or_call(std::vector<ExprPtr>& args) {
    ExprPtr bindfrt = parse_bind_front_right(args);
    if (bindfrt) return bindfrt;

    const Position pos = get_position();

    if (!is_token(TokenType::T_CALL)) return nullptr;
    next_token();

    ExprPtr right = parse_bind_front();
    return std::make_unique<CallExpr>(pos, std::move(right), std::move(args));
}

/*
 *      bind_front = [func_args, bindfrt_op], decorator
 */

ExprPtr Parser::parse_bind_front_right(std::vector<ExprPtr>& args) {
    if (!is_token(TokenType::T_BINDFRT)) return nullptr;

    const Position pos = get_position();

    next_token();
    ExprPtr decorator = shall(parse_decorator(), "Expected identifier or decorator");
    return std::make_unique<BindFrtExpr>(pos, std::move(decorator), std::move(args));
}

ExprPtr Parser::parse_bind_front() {
    ExprPtr decorator = parse_decorator();
    if (decorator) return decorator;

    std::optional<std::vector<ExprPtr>> args = parse_func_args();
    if (!args) return nullptr;

    ExprPtr bind_front = parse_bind_front_right(*args);
    if (!bind_front) return nullptr;

    return bind_front;
}

/*
 * decorator = identifier, [decorate_op, identifier]
 */

ExprPtr Parser::parse_decorator() {
    ExprPtr left = parse_identifier();
    if (!left) return nullptr;

    if (!is_token(TokenType::T_DECORATE)) return left;
    next_token();

    ExprPtr right = shall(parse_identifier(), "Expected identifier");

    return std::make_unique<BinaryExpr>(left->get_position(), std::move(left), BinaryOp::DECORATE,
                                        std::move(right));
}

ExprPtr Parser::parse_identifier() {
    if (!is_token(TokenType::T_IDENTIFIER)) return nullptr;
    std::string value = current_token.get_value<std::string>();
    const Position pos = get_position();

    next_token();
    return std::make_unique<IdentifierExpr>(pos, value);
}

/*
 * func_args = "(", [function_arg_list], ")"
 * function_arg_list = expression, {",", expression}
 */

std::optional<std::vector<ExprPtr>> Parser::parse_func_args() {
    if (!is_token(TokenType::T_LPAREN)) return std::nullopt;

    std::vector<ExprPtr> args;
    do {
        next_token();
        ExprPtr arg = parse_expression();
        if (!arg) break;
        args.push_back(std::move(arg));
    } while (is_token(TokenType::T_COMMA));

    shall(is_token(TokenType::T_RPAREN), "Expected ')'");

    next_token();
    return args;
}

/*
 *      type              = type_non_void | void
 *      type_non_void     = var_type | func_type
 */

TypePtr Parser::parse_type(bool allow_void) {
    TypePtr result = parse_var_type(allow_void);
    if (result) return result;

    result = parse_func_type();
    return result;
}

/*
 *
 *      var_type_mut      = var_type [mut]
 *      var_type          = int
 *                        | flt
 *                        | string
 *                        | bool
 */

TypePtr Parser::parse_var_type(bool allow_void) {
    bool mut = false;
    if (is_token(TokenType::T_MUT)) {
        mut = true;
        next_token();
    }

    std::optional<BaseType> type;

    if (!(type = translate_token_to_type(current_token.get_type()))) {
        shall(!mut, "Expected type");
        return nullptr;
    }

    if (*type == BaseType::VOID && (!allow_void || mut))
        throw ParserError(get_position(), "Expected non-void type");

    next_token();
    return std::make_unique<VarType>(*type, mut);
}

/*
 *
 *      func_type         = "[", type, function_sign_op, func_type_args
 *      func_type_args    = type_non_void, {",", type_non_void}
 */

TypePtr Parser::parse_func_type() {
    if (!is_token(TokenType::T_LFTYPE)) return nullptr;

    next_token();
    TypePtr ret_type = shall(parse_type(true), "Expected type");

    shall(is_token(TokenType::T_FUNC_SIGN), "Expected '::'");

    std::vector<std::unique_ptr<Type>> params;
    TokenType separator;
    while (!is_token(TokenType::T_RFTYPE)) {
        next_token();
        params.push_back(parse_type());
        separator = current_token.get_type();
        if (separator != TokenType::T_COMMA && separator != TokenType::T_RFTYPE)
            throw ParserError(get_position(), "Expected ',' or ']'");
    }

    next_token();

    return std::make_unique<FuncType>(std::move(ret_type), std::move(params));
}

bool Parser::is_comparative(TokenType type) const {
    switch (type) {
        case TokenType::T_NEQ:
            return true;
        case TokenType::T_EQ:
            return true;
        case TokenType::T_LTE:
            return true;
        case TokenType::T_GTE:
            return true;
        case TokenType::T_LT:
            return true;
        case TokenType::T_GT:
            return true;
        default:
            return false;
    }
}

bool Parser::is_factor(TokenType type) const {
    switch (type) {
        case TokenType::T_INT:
            return true;
        case TokenType::T_FLT:
            return true;
        case TokenType::T_BOOL:
            return true;
        case TokenType::T_STRING:
            return true;
        default:
            return false;
    }
}

bool Parser::is_type(TokenType type) const {
    switch (type) {
        case TokenType::T_INT_TYPE:
            return true;
        case TokenType::T_FLT_TYPE:
            return true;
        case TokenType::T_STRING_TYPE:
            return true;
        case TokenType::T_BOOL_TYPE:
            return true;
        default:
            return false;
    }
}

bool Parser::is_type_or_void(TokenType type) const {
    return (is_type(type) || type == TokenType::T_VOID_TYPE);
}

bool Parser::is_next_token(TokenType type) {
    next_token();
    return current_token.get_type() == type;
}

bool Parser::was_last_token(TokenType type) {
    bool match = current_token.get_type() == type;
    next_token();
    return match;
}

bool Parser::is_token(TokenType type) const { return current_token.get_type() == type; }

void Parser::next_token() {
    current_token = lexer->get_token();
    if (is_token(TokenType::T_COMMENT)) current_token = lexer->get_token();
}
