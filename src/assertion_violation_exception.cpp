#include "assertion_violation_exception.hpp"
#include "build_error_message.hpp"

namespace g {
AssertionViolationException::AssertionViolationException(
  std::size_t      line,
  std::string_view function,
  std::string_view file,
  std::string_view message)
  : runtime_error{buildErrorMessage(
    "AssertionViolationException",
    line,
    function,
    file,
    message)}
{
}
} // namespace g
