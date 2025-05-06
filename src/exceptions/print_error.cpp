#include "exceptions.h"
#include <iostream>
#include <fstream>

void print_error(const ParserError& e, std::string input_file) {
        std::ifstream file(input_file);
        if (file) {
            const Position pos = e.get_position();
            const uint32_t line_num = pos.get_line();
            const uint32_t column_num = pos.get_column();

            std::string line;
            unsigned int current_line = 1;

            std::string prev_line, curr_line, next_line;

            while (std::getline(file, line)) {
                if (current_line + 1 == line_num)
                    prev_line = line;
                else if (current_line == line_num)
                    curr_line = line;
                else if (current_line - 1 == line_num) {
                    next_line = line;
                    break;
                }
                current_line++;
            }

            if (!prev_line.empty())
                std::cerr << "  " << line_num - 1 << " | " << prev_line << std::endl;

            std::cerr << "\033[1;31m> " << line_num << " | " << curr_line << "\033[0m" << std::endl;
            std::cerr << "    " << std::string(column_num + std::to_string(line_num).size(), ' ') << "\033[1;31m^\033[0m" << std::endl;
            std::cerr << "    " << std::string(column_num + std::to_string(line_num).size(), ' ') << e.what() << std::endl;

            if (!next_line.empty())
                std::cerr << "  " << line_num + 1 << " | " << next_line << std::endl;

        } else {
            std::cerr << e.what() << std::endl;
            std::cerr << "Failed to open file to display error context." << std::endl;
        }
}
