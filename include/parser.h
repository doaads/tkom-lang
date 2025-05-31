#pragma once

#include <optional>

#include "block.h"
#include "exceptions.h"
#include "expression.h"
#include "function.h"
#include "lexer.h"
#include "program.h"
#include "statement.h"
#include "statement_specific.h"
#include "token.h"
#include "tokens.h"
#include "type.h"

using ExprPtr = std::unique_ptr<Expression>;
using ArgOrExpr = std::variant<ExprPtr, std::vector<ExprPtr>>;
using StatementPtr = std::unique_ptr<Statement>;
using BlockPtr = std::unique_ptr<Block>;
using ForLoopArgsPtr = std::unique_ptr<ForLoopArgs>;
using FuncPtr = std::unique_ptr<Function>;
using TypePtr = std::unique_ptr<Type>;
using FuncSignPtr = std::unique_ptr<FuncSignature>;
using ProgramPtr = std::unique_ptr<Program>;
using ParamPtr = std::unique_ptr<VariableSignature>;

/**
 * @brief A class responsible for parsing source code into a program tree.
 */
class Parser {
   private:
    std::shared_ptr<Lexer> lexer;  ///< Source lexer for tokens.
    Token current_token;           ///< Current token being processed.

    /**
     * @brief Parses a binary expression using given condition and next-step logic.
     *
     * @tparam ConditionFunc Function to check operator precedence.
     * @tparam NextFunc Function to get the next expression level.
     * @param condition Operator condition function.
     * @param next Next parsing function.
     * @param limited Whether to limit parsing scope.
     * @return Parsed expression.
     */
    template <typename ConditionFunc, typename NextFunc>
    auto parse_binary_expr(ConditionFunc condition, NextFunc next, bool limited = false) -> ExprPtr;

    /**
     * @brief Parses a function definition.
     * `function_def = func_signature, block`
     *
     * @return A unique pointer to the parsed function.
     */
    auto parse_func_def() -> FuncPtr;

    /**
     * @brief Parses a function signature.
     * `func_signature  = type, identifier, [function_sign_op, function_def_params]`
     * `func_def_params = type_non_void, identifier, {",", type_non_void, identifier}`
     * @return A unique pointer to the parsed function signature.
     */
    auto parse_func_signature() -> FuncSignPtr;

    /**
     * @brief Parses a function parameter.
     * `func_param  = type_non_void, identifier`
     * @return A unique pointer to the parsed function parameter.
     */
    auto parse_func_param() -> ParamPtr;

    /**
     * @brief Parses a block of code.
     * `block = "{", {statement}, "}"`
     *
     * @return A unique pointer to the parsed block.
     */
    auto parse_block() -> BlockPtr;

    /**
     * @brief Parses an OR expression (logical OR).
     * `or_expression = and_expression, {logical_or, and_expression}`
     *
     * @return A unique pointer to the parsed OR expression.
     */
    auto parse_or_expression() -> ExprPtr;

    /**
     * @brief Parses an AND expression (logical AND).
     * `and_expression = comp_expression, {logical_and, comp_expression}`
     *
     * @return A unique pointer to the parsed AND expression.
     */
    auto parse_and_expression() -> ExprPtr;

    /**
     * @brief Parses a comparative expression (e.g., equality, relational operators).
     * `comp_expression = additive_expression, [comp_operator, additive_expression]`
     *
     * @return A unique pointer to the parsed comparative expression.
     */
    auto parse_comp_expression() -> ExprPtr;

    /**
     * @brief Parses an additive expression (addition, subtraction).
     * `additive_expression = term, {add_sub_op, term}`
     *
     * @return A unique pointer to the parsed additive expression.
     */
    auto parse_additive_expression() -> ExprPtr;

    /**
     * @brief Parses a term (multiplication, division).
     * `term = unary_factor, {mult_div_op, unary_factor}`
     *
     * @return A unique pointer to the parsed term.
     */
    auto parse_term() -> ExprPtr;

    /**
     * @brief Parses a unary expression (negation, logical NOT).
     *
     * `unary_factor = [unary_operator], factor`
     * @return A unique pointer to the parsed unary expression.
     */
    auto parse_unary() -> ExprPtr;

    /**
     * @brief Parses a factor (base of expressions like numbers or identifiers).
     * `factor = function_call`
     *        `| bind_front`
     *        `| decorator`
     *        `| number`
     *        `| identifier`
     *        `| string`
     *        `| "(" expression ")"`
     *
     * @return A unique pointer to the parsed factor expression.
     */
    auto parse_factor() -> ExprPtr;

    /**
     * @brief Parses a function call or a parenthesized expression.
     * `function_call = function_args, call_op, bind_front`
     *
     * @return A unique pointer to the parsed expression.
     */
    auto parse_func_call_or_parens() -> ExprPtr;

    /**
     * @brief Parses an assignment or a function call statement.
     *
     * `assignment    = expression, assign_op, [type_non_void_mut], identifier, ";"`
     * `expression_st = expression, ";"`
     *
     * Parsed together due to common construction at start
     * @return A unique pointer to the parsed statement.
     */
    auto parse_assign_or_call() -> StatementPtr;

    /**
     * @brief Parses a conditional statement (if/else).
     * `if_statement = if, condition, block, [elif_st | else_st]`
     * `elif_st      = elif, condition, block, [elif_st | else_st]`
     *
     * @param st_type The type of the statement (if or else).
     * @return A unique pointer to the parsed conditional statement.
     */
    auto parse_conditional_statement(TokenType st_type) -> StatementPtr;

    /**
     * @brief Parses an else statement.
     * `else_st      = else, block`
     *
     * @return A unique pointer to the parsed else statement.
     */
    auto parse_else_statement() -> StatementPtr;

    /**
     * @brief Parses a for loop statement.
     * `for_loop = for, for_loop_args, block, call_op, bind_front, ";"`
     *
     * @return A unique pointer to the parsed for loop statement.
     */
    auto parse_for_loop() -> StatementPtr;

    /**
     * @brief Parses the arguments for a for loop.
     * `for_loop_args = "(", (identifier | assignment), ";", expression, ")"`
     *
     * @return A unique pointer to the parsed for loop arguments.
     */
    auto parse_for_loop_args() -> ForLoopArgsPtr;

    /**
     * @brief Parses a while loop statement.
     * `while_loop = while, condition, block`
     *
     * @return A unique pointer to the parsed while loop statement.
     */
    auto parse_while_loop() -> StatementPtr;

    /**
     * @brief Parses a return statement.
     * `ret_statement = ret, expression, ";"
     *
     * @return A unique pointer to the parsed return statement.
     */
    auto parse_ret_statement() -> StatementPtr;

    /**
     * @brief Parses the condition for conditional or loop statements.
     * `condition    = "(", expresion, ")"`
     *
     * @return A unique pointer to the parsed condition expression.
     */
    auto parse_condition() -> ExprPtr;

    /**
     * @brief Parses a bind front-right or function call.
     *
     * @param args The arguments for the bind front-right or function call.
     * @return ExprPtr if function succeeds, if it does not - it returns the argument vector to
     * previous owner
     *
     */
    auto parse_bindfrt_or_call(std::vector<ExprPtr>& args) -> ExprPtr;

    /**
     * @brief Parses a bind front-right expression.
     * `bind_front = [func_args, bindfrt_op], decorator`
     *
     * @param args The arguments for the bind front-right expression.
     * @return ExprPtr if function succeeds, if it does not - it returns the argument vector to
     * previous owner
     */
    auto parse_bind_front_right(std::vector<ExprPtr>& args) -> ExprPtr;

    /**
     * @brief Parses a bind front expression.
     * `bind_front = [func_args, bindfrt_op], decorator`
     *
     * @return A unique pointer to the parsed bind front expression.
     */
    auto parse_bind_front() -> ExprPtr;

    /**
     * @brief Parses a decorator expression.
     * `decorator = identifier, [decorate_op, identifier]`
     *
     * @return A unique pointer to the parsed decorator expression.
     */
    auto parse_decorator() -> ExprPtr;

    /**
     * @brief Parses an identifier expression.
     *
     * @return A unique pointer to the parsed identifier expression.
     */
    auto parse_identifier() -> ExprPtr;

    /**
     * @brief Parses a type expression (e.g., int, void).
     * `type              = type_non_void | void`
     * `type_non_void     = var_type | func_type`
     *
     * @param allow_void Whether to allow the void type. `true` by default.
     * @return A unique pointer to the parsed type expression.
     */
    auto parse_type(bool allow_void = false) -> TypePtr;

    /**
     * @brief Parses a variable type.
     * `var_type_mut      = [mut] var_type`
     * `var_type          = int`
     *                   `| flt`
     *                   `| string`
     *                   `| bool`
     *
     * @param allow_void Whether to allow the void type.
     * @return A unique pointer to the parsed variable type.
     */
    auto parse_var_type(bool allow_void) -> TypePtr;

    /**
     * @brief Parses a function type.
     * `func_type         = "[", type, function_sign_op, func_type_args`
     * `func_type_args    = type_non_void, {",", type_non_void}`
     * @return A unique pointer to the parsed function type.
     */
    auto parse_func_type() -> TypePtr;

    /**
     * @brief Parses function arguments.
     * `func_args = "(", [function_arg_list], ")"`
     * `function_arg_list = expression, {",", expression}`
     *
     * @return A vector of unique pointers to parsed function arguments.
     */
    auto parse_func_args() -> std::optional<std::vector<ExprPtr>>;

    /**
     * @brief Checks if the given token is a comparative operator.
     *
     * @param type The token type to check.
     * @return True if the token is a comparative operator, false otherwise.
     */
    [[nodiscard]] auto is_comparative(TokenType type) const -> bool;

    /**
     * @brief Checks if the given token is a factor operator.
     *
     * @param type The token type to check.
     * @return True if the token is a factor operator, false otherwise.
     */
    [[nodiscard]] auto is_factor(TokenType type) const -> bool;

    /**
     * @brief Checks if the given token represents a type.
     *
     * @param type The token type to check.
     * @return True if the token represents a type, false otherwise.
     */
    [[nodiscard]] auto is_type(TokenType type) const -> bool;

    /**
     * @brief Checks if the given token represents a type or void.
     *
     * @param type The token type to check.
     * @return True if the token represents a type or void, false otherwise.
     */
    [[nodiscard]] auto is_type_or_void(TokenType type) const -> bool;

    /**
     * @brief Checks if the next token is of the specified type.
     *
     * @param type The token type to check.
     * @return True if the next token matches the given type, false otherwise.
     */
    auto is_next_token(TokenType type) -> bool;

    /**
     * @brief Checks if the last token was of the specified type.
     *
     * @param type The token type to check.
     * @return True if the last token matches the given type, false otherwise.
     */
    auto was_last_token(TokenType type) -> bool;

    /**
     * @brief Checks if the current token matches the specified type.
     *
     * @param type The token type to check.
     * @return True if the current token matches the given type, false otherwise.
     */
    [[nodiscard]] auto is_token(TokenType type) const -> bool;

    [[nodiscard]] auto get_position() const -> const Position {
        return current_token.get_position();
    };

    template <typename T>
    auto shall(T arg, std::string error) -> T {
        if (!arg) throw ParserError(get_position(), error);
        return std::move(arg);
    }

   public:
    /**
     * @brief Constructs a parser with a lexer.
     *
     * @param lexer Shared pointer to the lexer.
     */
    Parser(std::shared_ptr<Lexer> lexer);

    /**
     * @brief Starts parsing and returns a full program tree.
     *
     * @return Smart pointer to parsed program.
     */
    auto parse() -> ProgramPtr;

    /**
     * @brief Parses a single statement.
     * @return Smart pointer to parsed statement.
     */
    auto parse_statement() -> StatementPtr;

    /**
     * @brief Parses an expression.
     *
     * @return Smart pointer to parsed expression.
     */
    auto parse_expression() -> ExprPtr;

    /**
     * @brief Advances to the next token.
     */
    void next_token();
};
