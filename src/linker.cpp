#include "linker.hpp"
#include <fstream>
#include "utils/fileio.hpp"

std::string Florence::Linker::link(const std::string &path, const std::string & source_code, std::vector<std::string> filenames) {
    std::vector<std::string> libraries_to_add;
    libraries_to_add.insert(libraries_to_add.end(), std_lib.begin(), std_lib.end()); // Add the standard library modules.
    libraries_to_add.insert(libraries_to_add.end(), filenames.begin(), filenames.end()); // Add the extensions.
    std::string generated_assembly;
    for (const std::string &filename : libraries_to_add) {
        generated_assembly += Florence::Utils::read_source(path + "/" + filename);
    }
    generated_assembly += source_code;
    return generated_assembly;
}

std::string Florence::Linker::link(const std::string &path, const std::string & source_code) {
    std::vector<std::string> empty;
    return link(path, source_code, empty);
}