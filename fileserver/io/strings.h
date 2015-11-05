#pragma once

#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

namespace fserv {
namespace io {

/**
 * safely read an int value
 */
int parse_int(const std::string &str, int default_value=0);

/**
 * string split function using a char delimiter
 */
std::vector<std::string> split(const std::string &str, char delim);

/**
 * read a whole file into a string
 */
std::string read_file(const std::string &filename);

/**
 * read a file split by new line
 */
std::vector<std::string> read_file_lines(const std::string &filename);

/**
 * read a file as map pairs
 */
std::unordered_map<std::string, std::string> read_file_map(const std::string &filename);

/**
 * reads first line from a file
 */
std::string read_line(const std::string &filename);

} // namespace io
}
