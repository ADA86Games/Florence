#include "assembler.hpp"
#include "utils/fileio.hpp"
#include "fscompiler/Compiler.hpp"
#include "fscompiler/Parser.hpp"
#include "fscompiler/Tokenizer.hpp"
#include "linker.hpp"
#include <cstdlib>
#include <zconf.h>
#include <libgen.h>

void Florence::Assembler::assemble(const std::string &path, const std::string &input_filename, const std::string &output_filename) {
    std::string source = Florence::Utils::read_source(input_filename); // Get the source code.
    auto *logger = new Florence::FSCompiler::ErrorLogger(); // Initialise the error logger.
    auto *tokenizer = new Florence::FSCompiler::Tokenizer(source, logger); // Initialise the tokenizer.
    std::queue<Florence::FSCompiler::Tokens::Token*> tokens = tokenizer->tokenize(); // Tokenize the source.
    auto *parser = new Florence::FSCompiler::Parser(tokens, logger); // Initialise parser.
    std::queue<Florence::FSCompiler::IRElements::IRElement*> ir = parser->parse(); // Parse the tokens.
    auto *compiler = new Florence::FSCompiler::Compiler(ir, logger); // Initialise the compiler.
    std::string source_code = compiler->compile(); // Generate source code from the Intermediate representation.
    char *buffer = (char*) malloc(PATH_MAX * sizeof(char));
    int location = readlink("/proc/self/exe", buffer, PATH_MAX * sizeof(char));
    buffer[location] = 0;

    std::string generated_assembly = Florence::Linker::link(dirname(buffer), source_code); // Link the source code to standard library.

    Florence::Utils::write_source("temp.asm", generated_assembly); // Write the code to an output file
    system(("fasm temp.asm " + output_filename).c_str()); // Create executable.
//    system("rm temp.asm"); // Remove the temporary file created by the process.
}