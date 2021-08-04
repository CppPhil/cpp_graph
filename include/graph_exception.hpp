#ifndef INCG_G_GRAPH_EXCEPTION_HPP
#define INCG_G_GRAPH_EXCEPTION_HPP
#include <stdexcept>
#include <string_view>

namespace g {
/*!
 * \brief Defines the exception type used in this project.
 **/
class GraphException : public std::runtime_error {
public:
  /*!
   * \brief Creates a `GraphException`.
   * \param line The line where the exception is thrown.
   * \param function The function in which the exception is thrown.
   * \param file The file in which the exception is thrown.
   * \param message The exception message.
   **/
  GraphException(
    std::size_t      line,
    std::string_view function,
    std::string_view file,
    std::string_view message);
};
} // namespace g
#endif // INCG_G_GRAPH_EXCEPTION_HPP
