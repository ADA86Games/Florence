#ifndef FLORENCE_LINKER_HPP
#define FLORENCE_LINKER_HPP
#include <string>
#include <vector>

static const char *header_asm =
#include "../include/header.asm"
;

static const char *render_asm =
#include "../include/render.asm"
;

static const char *userio_asm =
#include "../include/userio.asm"
;

static const char *utils_asm =
#include "../include/utils.asm"
;

static const std::vector<std::string> std_lib {header_asm, render_asm, userio_asm, utils_asm}; // Standard library.

namespace Florence::Linker {
    /**
     * Link the source codes of the files with the given filenames.
     * @param path Path to installation directory.
     * @param source_code Source code to add the links.
     * @param filenames Filenames of the sources to link.
     * @return the linked source code.
     */
    std::string link(const std::string & source_code, std::vector<std::string> filenames);

    /**
     * Link the source code with just the standard library.
     * @param path Path to installation directory.
     * @param source_code Source code to link.
     * @return The linked source code.
     */
    std::string link(const std::string & source_code);
}

#endif //FLORENCE_LINKER_HPP
