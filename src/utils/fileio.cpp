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
    input_file.close();
    return file_content;
}

void Florence::Utils::write_source(const std::string &file_name, const std::string &content) {
    std::ofstream ofstream (file_name);
    if (ofstream.is_open()) {
        ofstream << content;
    } else {
        std::cerr << "Err: Write failed.\n";
    }
    ofstream.close();
}