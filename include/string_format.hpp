#ifndef INCG_G_STRING_FORMAT_HPP
#define INCG_G_STRING_FORMAT_HPP
#include <string>
#include <string_view>

namespace g {
/*!
 * \brief Formats a string using C-style varargs.
 * \param formatString The printf-style format string.
 * \return The string created.
 * \throws std::runtime_error if an error occurs.
 **/
[[nodiscard]] std::string stringFormat(std::string_view formatString, ...);
} // namespace g
#endif // INCG_G_STRING_FORMAT_HPP
