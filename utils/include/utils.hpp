/**
 * @file  utils.hpp
 * @brief Utils header
 */

#ifndef _UTILS_HPP_
#define _UTILS_HPP_

#include <iostream>
#include <string>
#include <string.h>

/**
 * @brief Parse the file name from the file path
 */
#define __FILENAME__(file) (strrchr(file, '/') ? strrchr(file, '/') + 1 : file)

/**
 * @brief Print error log in the standard output
 * @param[in] log The error log
 */
inline void
PrintError(const char* file, size_t line, const std::string& message)
{
    std::cout << "ERROR - [" << __FILENAME__(file) << ":" << line << "] " << message << std::endl;
}

/**
 * @brief Log error message
 * @param[in] message Message to print
 */
#define LOG_ERROR(message) PrintError(__FILE__, __LINE__, message)

#endif /* _UTILS_HPP_ */
