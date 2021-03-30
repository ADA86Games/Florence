#ifndef FLORENCE_LINKER_HPP
#define FLORENCE_LINKER_HPP
#include <string>
#include <vector>

const std::vector<std::string> std_lib {"../include/header.asm", "../include/render.asm",
                                        "../include/userio.asm", "../include/utils.asm"}; // Standard library.

namespace Florence::Linker {
    /**
     * Link the source codes of the files with the given filenames.
     * @param source_code Source code to add the links.
     * @param filenames Filenames of the sources to link.
     * @return the linked source code.
     */
    std::string link(const std::string & source_code, std::vector<std::string> filenames);

    /**
     * Link the source code with just the standard library.
     * @param source_code Source code to link.
     * @return The linked source code.
     */
    std::string link(const std::string & source_code);
}

#endif //FLORENCE_LINKER_HPP
