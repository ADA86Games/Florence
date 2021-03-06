#ifndef FLORENCE_ASSEMBLER_HPP
#define FLORENCE_ASSEMBLER_HPP
#include <string>

namespace Florence::Assembler {
    /**
     * Given an input and output filename, take the file,
     *  and generate an x86 DOS executable out of it.
     *
     * @param path Path to the binary.
     * @param input_filename Input filename.
     * @param output_filename Output filename.
	 * @param is_debug: retains the temporary assembly file.
     */
    void assemble(const std::string &input_filename, const std::string &output_filename, bool is_debug);
}

#endif //FLORENCE_ASSEMBLER_HPP
