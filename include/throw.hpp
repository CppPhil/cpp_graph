#ifndef INCG_G_THROW_HPP
#define INCG_G_THROW_HPP
#include <cstddef>

#include "pretty_function.hpp"
#include "string_format.hpp"

/*!
 * \def G_THROW
 * \brief Throws an exception of type `ExceptionType` using the printf-style
 *        format string `formatString`.
 **/
#define G_THROW(ExceptionType, formatString, ...)                    \
  throw ExceptionType                                                \
  {                                                                  \
    static_cast<std::size_t>(__LINE__), G_PRETTY_FUNCTION, __FILE__, \
      ::g::stringFormat(formatString, __VA_ARGS__)                   \
  }

#endif // INCG_G_THROW_HPP
