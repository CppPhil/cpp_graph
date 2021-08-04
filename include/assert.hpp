#ifndef INCG_G_ASSERT_HPP
#define INCG_G_ASSERT_HPP
#include "assertion_violation_exception.hpp"
#include "throw.hpp"

/*!
 * \def G_ASSERT
 * \brief Asserts that a condition is true.
 **/
#define G_ASSERT(condition, formatString, ...) \
  do {                                         \
    if (!(condition)) {                        \
      G_THROW(                                 \
        g::AssertionViolationException,        \
        "ASSERTION VIOLATION: \"" #condition   \
        "\" evaluated to false!\n"             \
        "Context : " formatString,             \
        __VA_ARGS__);                          \
    }                                          \
  } while (false)

#endif // INCG_G_ASSERT_HPP
