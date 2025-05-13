#include "exceptions.h"
#include "parser_visitor.h"
#include "lexer.h"
#include "print_error.h"
#include "parser.h"
#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>

namespace po = boost::program_options;

int main(int argc, char **argv) {
    std::string input_file;
    bool verbose = false;

    po::options_description desc("Allowed options");
    desc.add_options()
        ("verbose,V", po::bool_switch(&verbose), "enable verbose output")
        ("input", po::value<std::string>(&input_file), "input file")
        ("help,h", "show help message");

    po::positional_options_description pos_desc;
    pos_desc.add("input", 1);

    po::variables_map vm;
    try {
        po::store(po::command_line_parser(argc, argv)
                .options(desc)
                .positional(pos_desc)
                .run(),
            vm);
        po::notify(vm);
    } catch (const std::exception& e) {
        std::cerr << "Error parsing options: " << e.what() << std::endl;
        return 1;
    }

    if (input_file.empty()) {
        std::cout << "\033[1;31mError:\033[0m Input file is required" << std::endl;
        return 1;
    }

    std::shared_ptr<std::fstream> input = std::make_unique<std::fstream>(input_file, std::fstream::in);
    std::shared_ptr<Lexer> lexer = std::make_shared<Lexer>(input, verbose);
    Parser parser = Parser(std::move(lexer));
    std::unique_ptr<Program> program;
    if (verbose)
        std::cout << "--------------------------------\033[36m" << input_file << "\033[0m-------------------------------" << std::endl;

    try {
        program = parser.parse();
    } catch (const ParserError& e) {
        std::cerr << "\033[1;31mError:\033[0m " << e.what() << std::endl;

        print_error(e, input_file);
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "\033[1;31mUnhandled Error:\033[0m " << e.what() << std::endl;
        return 1;
    }

    if (verbose) {
        ParserPrinter printer = ParserPrinter(std::cout);
        printer.visit(*program);
        std::cout << "-------------------------------\033[36m/" << input_file << "\033[0m-------------------------------" << std::endl;
    }

    return 0;

}



