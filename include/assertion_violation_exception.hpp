#ifndef INCG_G_ASSERTION_VIOLATION_EXCEPTION_HPP
#define INCG_G_ASSERTION_VIOLATION_EXCEPTION_HPP
#include <stdexcept>
#include <string_view>

namespace g {
class AssertionViolationException : public std::runtime_error {
public:
  AssertionViolationException(
    std::size_t      line,
    std::string_view function,
    std::string_view file,
    std::string_view message);
};
} // namespace g
#endif // INCG_G_ASSERTION_VIOLATION_EXCEPTION_HPP
