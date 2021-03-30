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

    /**
     * Write the source code to a file.
     * @param file_name Name of the file to write to.
     * @param content Content to write.
     */
    void write_source(const std::string &file_name, const std::string &content);
}
#endif // FUTIL_FILEIO_HPP