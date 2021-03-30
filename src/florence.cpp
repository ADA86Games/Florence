#include "assembler.hpp"
#include <iostream>
#include <cstring>

const std::string program_name = "florence 0.2";
const std::string description = "Florence is a compiler for FlorenceScript applications, "
                                "choose your own adventure game scripts. It compiles these scripts to "
                                "x86 DOS games.";
const std::string usage = "usage: florence <input> <output>";

int main(int argc, char *argv[]) {
    if (argc == 2 && std::strcmp(argv[1], "--help") == 0 || std::strcmp(argv[0], "-h") == 0) { // Print help message
        std::cout << program_name << std::endl << usage << std::endl << description << std::endl;
    } else if (argc == 3) {
        Florence::Assembler::assemble(argv[1], argv[2]);
        std::cout << "Assembled to x86 DOS Executable." << std::endl;
    } else {
        std::cout << usage << std::endl;
    }
}