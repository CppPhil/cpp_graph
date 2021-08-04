#ifndef INCG_G_BUILD_ERROR_MESSAGE_HPP
#define INCG_G_BUILD_ERROR_MESSAGE_HPP
#include <cstddef>

#include <string>
#include <string_view>

namespace g {
/*!
 * \brief Creates an error message.
 * \param exceptionName The name of the exception type.
 * \param line The line where the exception is thrown.
 * \param function The function in which the exception is thrown.
 * \param file The file in which the exception is thrown.
 * \param message The exception message.
 * \return The error message.
 **/
std::string buildErrorMessage(
  std::string_view exceptionName,
  std::size_t      line,
  std::string_view function,
  std::string_view file,
  std::string_view message);
} // namespace g
#endif // INCG_G_BUILD_ERROR_MESSAGE_HPP
