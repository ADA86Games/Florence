#include "linker.hpp"
#include <fstream>
#include "utils/fileio.hpp"

std::string Florence::Linker::link(const std::string & source_code, std::vector<std::string> filenames) {
    std::vector<std::string> libraries_to_add;
    libraries_to_add.insert(libraries_to_add.end(), filenames.begin(), filenames.end()); // Add the extensions.
    std::string generated_assembly;
    for (const std::string &module : std_lib) {
        generated_assembly += module;
    }
    generated_assembly += source_code;
    return generated_assembly;
}

std::string Florence::Linker::link(const std::string & source_code) {
    std::vector<std::string> empty;
    return link(source_code, empty);
}