#include "fileio.hpp"
#include <fstream>
#include <string>
#include <iostream>

std::string Florence::Utils::read_source(std::string file_name) {
    std::ifstream input_file (file_name);
    std::string file_content;
    std::string line;
    if (input_file.is_open()) {
        while (std::getline(input_file, line)) {
            file_content += line + "\n";
        }
    } else {
        std::cerr << "Err: File not found!.\n";
    }
    return file_content;
}