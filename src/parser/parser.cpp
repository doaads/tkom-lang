#include "parser.h"
#include "operators.h"

Parser::Parser(std::shared_ptr<Lexer> lexer, bool verbose) :
    verbose(verbose), lexer(std::move(lexer)) {
        visitor = std::make_shared<ParserPrinter>(std::cout);
    }

Parser::Parser(std::shared_ptr<Lexer> lexer, std::shared_ptr<ParserVisitor> visitor) :
    verbose(true),  // assume user wanted verbose output if visitor is provided
    visitor(std::move(visitor)),
    lexer(std::move(lexer)) {}

ProgramPtr Parser::parse() {
    std::vector<std::unique_ptr<Function>> functions;
    FuncPtr function;

    while (!is_next_token(TokenType::T_EOF)) {
        function = parse_func_def();
        if (!function) throw std::runtime_error("Expected function def");

        function->accept(*visitor);
        functions.push_back(std::move(function));
    }

    return std::make_unique<Program>(std::move(functions));
}

FuncPtr Parser::parse_func_def() {
    FuncSignPtr signature = parse_func_signature();
    if (!signature) return nullptr;

    BlockPtr body = parse_block();
    if (!body) throw std::runtime_error("Expected block");


    return std::make_unique<Function>(std::move(signature), std::move(body));
}

FuncSignPtr Parser::parse_func_signature() {
    TypePtr ret_type = parse_type();
    if (!ret_type) {
        if (!is_token(TokenType::T_VOID_TYPE)) {
            return nullptr;
        }
        next_token();
    }

    if (!is_token(TokenType::T_IDENTIFIER))
        throw std::runtime_error("Expected function identifier");

    std::string func_name = current_token.get_value<std::string>();

    if (!is_next_token(TokenType::T_FUNC_SIGN))
        throw std::runtime_error("Expected '::'");

    std::vector<std::unique_ptr<Variable>> params;
    TypePtr current_arg_type;
    std::string current_arg_name;
    while (!is_next_token(TokenType::T_LBLOCK) || is_token(TokenType::T_COMMA)) {
        current_arg_type = parse_type();
        if (!current_arg_type)
            throw std::runtime_error("Expected type or {");
        if (!is_token(TokenType::T_IDENTIFIER))
            throw std::runtime_error("Expected parameter name");
        current_arg_name = current_token.get_value<std::string>();
        params.push_back(std::make_unique<Variable>(std::move(current_arg_type), current_arg_name));
    }

    return std::make_unique<FuncSignature>(
            std::move(ret_type),
            std::move(params),
            func_name);
}

BlockPtr Parser::parse_block() {
    if (!is_token(TokenType::T_LBLOCK))
        throw std::runtime_error("Expected '{'");

    std::vector<StatementPtr> statements;

    while (!is_next_token(TokenType::T_RBLOCK)) {
        StatementPtr statement = parse_statement();
        if (!statement)
            throw std::runtime_error("Expected statement or }");
        statements.push_back(std::move(statement));
    }
    return std::make_unique<Block>(std::move(statements));
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

    statement = parse_assign_or_call();
    if (statement) return statement;

    return nullptr;
}

StatementPtr Parser::parse_assign_or_call() {
    ExprPtr expr = parse_expression();
    if (!expr)
        return nullptr;

    TokenType op_type = current_token.get_type();
    if (op_type != TokenType::T_ASSIGN) {
        if (op_type == TokenType::T_SEMICOLON)
            // if statement ends here, we have a ready CallStatement
            return std::make_unique<CallStatement>(std::move(expr));
        throw std::runtime_error("Expected '=>'");
    }

    next_token();
    TypePtr type = parse_type();
    if (!type) throw std::runtime_error("Expected type");

    if (!is_token(TokenType::T_IDENTIFIER))
        throw std::runtime_error("Expected identifier");

    std::string identifier = current_token.get_value<std::string>();

    if (!is_next_token(TokenType::T_SEMICOLON))
        throw std::runtime_error("Expected ;");

    return std::make_unique<AssignStatement>(std::move(expr), std::move(type), identifier);
}

StatementPtr Parser::parse_while_loop() {
    if (!is_token(TokenType::T_WHILE))
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
    if (!is_token(TokenType::T_FOR))
        return nullptr;

    next_token();
    ForLoopArgsPtr args = parse_for_loop_args();
    if (!args)
        throw std::runtime_error("Expected arguments");

    next_token();
    BlockPtr block = parse_block();
    if (!block)
        throw std::runtime_error("Expected block");

    if (!is_next_token(TokenType::T_CALL))
        throw std::runtime_error("Expected '->'");

    next_token();
    ExprPtr on_iter = parse_bind_front();
    if (!on_iter) throw std::runtime_error("Expected bind front or identifier");

    if (!is_token(TokenType::T_SEMICOLON))
        throw std::runtime_error("Expected ;");

    return std::make_unique<ForLoopStatement>(
            std::move(args),
            std::move(block),
            std::move(on_iter));
}

/*
 *    for_loop_args = "(", (identifier | assignment), ";", expression, ")"
*/

ForLoopArgsPtr Parser::parse_for_loop_args() {
    if (!is_token(TokenType::T_LPAREN))
        throw std::runtime_error("Expected (");

    next_token();
    // TODO: make this ONLY assign
    StatementPtr assign = parse_assign_or_call();
    std::string identifier;
    if (!assign) { 
        if (is_token(TokenType::T_IDENTIFIER)) {
            identifier = current_token.get_value<std::string>();
        } else {
            throw std::runtime_error("Expected assign or identifier");
        }
    }

    std::variant<std::monostate, StatementPtr, std::string> iterator;
    if (assign) {
        iterator = std::move(assign);
    } else {
        iterator = identifier;
    }

    if (!is_token(TokenType::T_SEMICOLON))
        throw std::runtime_error("Expected ;");
    
    next_token();
    ExprPtr condition = parse_expression();
    if (!condition)
        throw std::runtime_error("Expected expression");

    if (!is_token(TokenType::T_RPAREN))
        throw std::runtime_error("Expected )");


    ForLoopArgsPtr args = std::make_unique<ForLoopArgs>();
    args->iterator = std::move(iterator);
    args->condition = std::move(condition);
    return args;
}

/*
 *    ret_statement = ret, expression
*/

StatementPtr Parser::parse_ret_statement() {
    if (!is_token(TokenType::T_RET))
        return nullptr;

    next_token();
    ExprPtr expression = parse_expression();

    return std::make_unique<RetStatement>(std::move(expression));
}

/*
 *    if_statement = if, condition, block, [elif_st | else_st]
 *    elif_st      = elif, condition, block, [elif_st | else_st]
*/

StatementPtr Parser::parse_conditional_statement(TokenType st_type) {
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
 *    else_st      = else, block
*/

StatementPtr Parser::parse_else_statement() {
    if (!is_token(TokenType::T_ELSE))
        return nullptr;

    BlockPtr block = parse_block();
    if (!block)
        throw std::runtime_error("Expected block after else");

    return std::make_unique<ElseStatement>(std::move(block));
}


/*
 *    condition    = "(", expresion, ")"
*/

ExprPtr Parser::parse_condition() {
    if (!is_token(TokenType::T_LPAREN))
        throw std::runtime_error("Expected '('");

    next_token();
    ExprPtr condition = parse_expression();

    if (!condition)
        throw std::runtime_error("Expected expression");

    if (!is_token(TokenType::T_RPAREN))
        throw std::runtime_error("Expected ')'");

    return condition;
}

/*
 *    expression = or_expression;
 */

ExprPtr Parser::parse_expression() {
    ExprPtr expr = parse_or_expression();
    return expr;
}

/*
 *    or_expression = and_expression, {logical_or, and_expression}
*/

ExprPtr Parser::parse_or_expression() {
    ExprPtr left = parse_and_expression();
    if (!left) {
        return nullptr;
    }
    while (is_token(TokenType::T_OR)) {
        next_token();
        ExprPtr right = parse_and_expression();
        if (!right)
            throw std::runtime_error("Error while parsing OR. Expected identifier");
        left = std::make_unique<BinaryExpr>(
                std::move(left),
                BinaryOp::OR,
                std::move(right));
    }
    return left;
}

/*
 *    and_expression = comp_expression, {logical_and, comp_expression}
*/

ExprPtr Parser::parse_and_expression() {
    ExprPtr left = parse_comp_expression();
    if (!left) {
        return nullptr;
    }
    while (is_token(TokenType::T_AND)) {
        next_token();
        ExprPtr right = parse_comp_expression();
        if (!right)
            throw std::runtime_error("Error while parsing AND. Expected identifier");
        left = std::make_unique<BinaryExpr>(
                std::move(left),
                BinaryOp::AND,
                std::move(right));
    }
    return left;
}

/*
 *    comp_expression = additive_expression, [comp_operator, additive_expression]
*/

ExprPtr Parser::parse_comp_expression() {
    ExprPtr left = parse_additive_expression();
    if (!left) {
        return nullptr;
    }
    std::optional<BinaryOp> type = token_to_binary_op(current_token.get_type());
    if (type) {
        next_token();
        ExprPtr right = parse_additive_expression();
        if (!right)
            throw std::runtime_error("Error while parsing comp. Expected identifier");
        left = std::make_unique<BinaryExpr>(
                std::move(left),
                *type,
                std::move(right));
    }
    return left;
}

/*
 *    additive_expression = term, {add_sub_op, term}
*/

ExprPtr Parser::parse_additive_expression() {
    ExprPtr left = parse_term();
    if (!left) {
        return nullptr;
    }
    std::optional<BinaryOp> op_type = token_to_binary_op(current_token.get_type());
    while (op_type && (*op_type == BinaryOp::ADD || *op_type == BinaryOp::SUB)) {
        next_token();
        ExprPtr right = parse_term();
        if (!right)
            throw std::runtime_error("Expected identifier");
        left = std::make_unique<BinaryExpr>(
                std::move(left),
                *op_type,
                std::move(right));
        op_type = token_to_binary_op(current_token.get_type());
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
    //next_token();
    std::optional<BinaryOp> op_type = token_to_binary_op(current_token.get_type());
    while (op_type && (*op_type == BinaryOp::MULT || *op_type == BinaryOp::DIV)) {
        next_token();
        ExprPtr right = parse_unary();
        if (!right)
            throw std::runtime_error("Error while parsing terms. Expected identifier");
        left = std::make_unique<BinaryExpr>(
                std::move(left),
                *op_type,
                std::move(right));
        op_type = token_to_binary_op(current_token.get_type());
    }
    return left;
}

/*
 * unary_factor = [unary_operator], factor
*/

ExprPtr Parser::parse_unary() {
    TokenType type = current_token.get_type();
    std::optional<UnaryOp> unary_op = token_to_unary_op(type);
    if (unary_op) next_token();
    ExprPtr right = parse_factor();
    if (unary_op && !right)
        throw std::runtime_error("Expected factor after unary operator");
    if (unary_op) {
        return std::make_unique<UnaryExpr>(*unary_op, std::move(right));
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
        std::unique_ptr<LiteralExpr> expr = std::make_unique<LiteralExpr>(current_token);
        next_token();
        return expr;
    }

    if (is_token(TokenType::T_IDENTIFIER)) {
        ExprPtr expr = std::make_unique<IdentifierExpr>(current_token.get_value<std::string>());
        next_token();
        return expr;
    }

    ExprPtr factor = parse_func_call_or_parens();
    if (factor) return factor;

    factor = parse_decorator();
    if (factor) return factor;

    return nullptr;
}

/*
 *      expr can equal: '(', expr, ')'
 *      this can also be a func arg.
 *      we will parse the first expr and then check whether it is
 *      a valid function call or bindfront
*/

ExprPtr Parser::parse_func_call_or_parens() {
    std::optional<std::vector<ExprPtr>> args = parse_func_args();
    if (!args)
        return nullptr;

    ArgOrExpr bindfrt = parse_bindfrt_or_call(std::move(*args));
    if (!bindfrt.index()) return std::get<ExprPtr>(std::move(bindfrt));

    // move args back
    args = std::get<std::vector<ExprPtr>>(std::move(bindfrt));

    if (args->size() != 1) throw std::runtime_error("Expected expression");

    return std::move((*args)[0]);
}

/*
 * we have parsed the arguments, now
 * after parsing args, parse either call or bind front
 * if function returns the ownership of arguments, it means it failed.
 */

ArgOrExpr Parser::parse_bindfrt_or_call(std::vector<ExprPtr> args) {
    ArgOrExpr bindfrt = parse_bind_front_right(std::move(args));
    if (!bindfrt.index()) return std::get<ExprPtr>(std::move(bindfrt));

    // move args back
    args = std::get<std::vector<ExprPtr>>(std::move(bindfrt));

    if (!is_token(TokenType::T_CALL))
        return args;  // fail, return ownership of args
    next_token();

    ExprPtr right = parse_bind_front();
    return std::make_unique<CallExpr>(std::move(right), std::move(args));
}

/*
 * bind_front = [func_args, bindfrt_op], decorator
 */ 

ArgOrExpr Parser::parse_bind_front_right(std::vector<ExprPtr> args) {
    if (!is_token(TokenType::T_BINDFRT))
        return args;

    next_token();
    ExprPtr decorator = parse_decorator();
    if (!decorator) throw std::runtime_error("Expected identifier or decorator");
    return std::make_unique<BindFrtExpr>(std::move(decorator), std::move(args));
}

ExprPtr Parser::parse_bind_front() {
    ExprPtr decorator = parse_decorator();
    if (decorator)
        return decorator;

    std::optional<std::vector<ExprPtr>> args = parse_func_args();
    if (!args)
        return nullptr;

    ArgOrExpr bind_front = parse_bind_front_right(std::move(*args));
    if (bind_front.index()) return nullptr;

    return std::get<ExprPtr>(std::move(bind_front));
}

/*
 * decorator = identifier, [decorate_op, identifier]
 */

ExprPtr Parser::parse_decorator() {
    if (!is_token(TokenType::T_IDENTIFIER))
        return nullptr;

    std::string value = current_token.get_value<std::string>();
    ExprPtr left = std::make_unique<IdentifierExpr>(value);

    if (!is_next_token(TokenType::T_DECORATE))
        return left;

    if (!is_next_token(TokenType::T_IDENTIFIER))
        throw std::runtime_error("Expected identifier");

    value = current_token.get_value<std::string>();
    ExprPtr right = std::make_unique<IdentifierExpr>(value);

    next_token();
    return std::make_unique<BinaryExpr>(std::move(left), BinaryOp::DECORATE, std::move(right));

}

/*
 * func_args = "(", [function_arg_list], ")"
 * function_arg_list = expression, {",", expression}
 */

std::optional<std::vector<ExprPtr>> Parser::parse_func_args() {
    if (!is_token(TokenType::T_LPAREN))
        return std::nullopt;

    std::vector<ExprPtr> args;
    do {
        next_token();
        ExprPtr arg = parse_expression();
        if (!arg) break;
        args.push_back(std::move(arg));
    } while (is_token(TokenType::T_COMMA));
    
    if (!is_token(TokenType::T_RPAREN))
        throw std::runtime_error("Expected ')'");

    next_token();
    return args;
}

TypePtr Parser::parse_type() {
    TypePtr result = parse_var_type();
    if (result) return result;

    result = parse_func_type();
    return result;
}

TypePtr Parser::parse_var_type() {
    bool mut = false;
    if (is_token(TokenType::T_MUT)) {
        mut = true;
        next_token();
    }

    std::optional<BaseType> type;

    if (!(type = translate_token_to_type(current_token.get_type()))) {
        if (mut) throw std::runtime_error("Expected type");
        return nullptr;
    }

    next_token();
    return std::make_unique<VarType>(*type, mut);
}

TypePtr Parser::parse_func_type() {
    if (!is_token(TokenType::T_LFTYPE))
        return nullptr;

    next_token();
    TypePtr ret_type = parse_type();
    if (!ret_type && !is_token(TokenType::T_VOID_TYPE))
        throw std::runtime_error("Expected type");
    // if ret_type = nullptr we have a void func

    if (!is_token(TokenType::T_FUNC_SIGN))
        throw std::runtime_error("Expected '::'");

    std::vector<std::unique_ptr<Type>> params;
    TokenType separator;
    while (!is_token(TokenType::T_RFTYPE)) {
        next_token();
        params.push_back(parse_type());
        separator = current_token.get_type();
        if (separator != TokenType::T_COMMA && separator != TokenType::T_RFTYPE)
            throw std::runtime_error("Expected ',' or ']'");
    }

    next_token();

    return std::make_unique<FuncType>(std::move(ret_type), std::move(params));
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

bool Parser::is_next_token(TokenType type) {
    next_token();
    return current_token.get_type() == type;
}

bool Parser::was_last_token(TokenType type) {
    bool match = current_token.get_type() == type;
    next_token();
    return match;
}

bool Parser::is_token(TokenType type) const {
    return current_token.get_type() == type;
}

void Parser::next_token() {
    current_token = lexer->get_token();
}
