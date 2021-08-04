#include <locale>
#include <sstream>

#include "build_error_message.hpp"

namespace g {
std::string buildErrorMessage(
  std::string_view exceptionName,
  std::size_t      line,
  std::string_view function,
  std::string_view file,
  std::string_view message)
{
  std::ostringstream oss{};
  oss.imbue(std::locale::classic());

  oss << exceptionName << " was thrown:\n"
      << "Line    : " << line << '\n'
      << "Function: " << function << '\n'
      << "File    : " << file << '\n'
      << "Message : " << message << '\n';

  return oss.str();
}
} // namespace g
