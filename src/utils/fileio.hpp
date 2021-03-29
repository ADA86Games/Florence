#ifndef FUTIL_FILEIO_HPP
#define FUTIL_FILEIO_HPP
#include <string>
namespace Florence::Utils {
    /**
     * Read a file and return the source code.
    * 
    * @param file_name: Name of the file.
    * @return the source code of the file.
    */
    std::string read_source(std::string file_name);
}
#endif // FUTIL_FILEIO_HPP