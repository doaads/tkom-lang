#include "input_stream.h"
#include "lexer.h"
#include <boost/program_options.hpp>

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

    std::shared_ptr<InputStream> input = std::make_unique<FileInputStream>(input_file);
    Lexer lexer(input, verbose);
    while(!lexer.end()) {
        lexer.get_token();
    }
    return 0;

}

