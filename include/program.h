#pragma once

#include <memory>
#include <vector>

#include "function.h"

class Visitor;

/**
 * @brief class representing the program syntax three
 *
 * The class consists of a vector of pointers to the function definitions.
 */
class Program {
   private:
    std::vector<std::unique_ptr<Function>> functions;  //< user-defined functions

   public:
    Program(std::vector<std::unique_ptr<Function>> functions) : functions(std::move(functions)) {}

    void accept(Visitor &visitor) const;

    /**
     * @brief get the functions vector as a vector of const pointers to Function
     */
    [[nodiscard]] auto get_functions() const -> const std::vector<const Function *>;
};
