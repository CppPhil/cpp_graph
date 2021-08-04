#include <cstdarg>
#include <cstddef>
#include <cstdio>

#include <stdexcept>

#include "string_format.hpp"

namespace g {
namespace {
std::string vStringFormat(std::string_view formatString, std::va_list ap)
{
  std::va_list args2{};
  va_copy(args2, ap);
  const auto statusCode{std::vsnprintf(nullptr, 0, formatString.data(), ap)};

  if (statusCode < 0) {
    va_end(args2);
    throw std::runtime_error{
      "vStringFormat: vsnprintf failed, statusCode was less than 0!"};
  }

  const std::size_t bytesNeeded{1 + static_cast<std::size_t>(statusCode)};
  std::string       result(bytesNeeded, '\0');

  if (const auto statusCode2{
        std::vsnprintf(result.data(), bytesNeeded, formatString.data(), args2)};
      statusCode2 < 0) {
    va_end(args2);
    throw std::runtime_error{
      "vStringFormat: vsnprintf failed, statusCode2 was less than 0!"};
  }

  va_end(args2);
  result.resize(bytesNeeded - 1);
  return result;
}
} // anonymous namespace

std::string stringFormat(std::string_view formatString, ...)
{
  std::va_list args1{};
  va_start(args1, formatString);
  std::string result{vStringFormat(formatString, args1)};
  va_end(args1);
  return result;
}
} // namespace g
