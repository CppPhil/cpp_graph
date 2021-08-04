#include "graph_exception.hpp"
#include "build_error_message.hpp"

namespace g {
GraphException::GraphException(
  std::size_t      line,
  std::string_view function,
  std::string_view file,
  std::string_view message)
  : runtime_error{
    buildErrorMessage("GraphException", line, function, file, message)}
{
}
} // namespace g
