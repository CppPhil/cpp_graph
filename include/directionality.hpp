#ifndef INCG_G_DIRECTIONALITY_HPP
#define INCG_G_DIRECTIONALITY_HPP
#include <iosfwd>

namespace g {
/*!
 * \brief A scoped enum type for the directionalities of a `Graph`.
 **/
enum class Directionality {
  Undirected, /*!< Indicates that the edges of a `Graph` are undirected. */
  Directed    /*!< Indicates that the edges of a `Graph` are directed. */
};

/*!
 * \brief Prints a `Directionality` enumerator.
 * \param os The ostream to print to.
 * \param d The `Directionality` enumerator to print.
 * \return `os`.
 **/
std::ostream& operator<<(std::ostream& os, Directionality d);
} // namespace g
#endif // INCG_G_DIRECTIONALITY_HPP
