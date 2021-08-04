#ifndef INCG_G_ADJACENCY_LIST_HPP
#define INCG_G_ADJACENCY_LIST_HPP
#include <iosfwd>
#include <vector>

#include "vertex_identifier.hpp"

namespace g {
/*!
 * \brief The adjacency list implementation type for use with the `Graph` type.
 **/
class AdjacencyList {
public:
  using this_type = AdjacencyList;

  /*!
   * \brief Prints an `AdjacencyList` to `os`.
   * \param os The ostream to print to.
   * \param l The `AdjacencyList` to print.
   * \return `os`.
   **/
  friend std::ostream& operator<<(std::ostream& os, const this_type& l);

  /*!
   * \brief Creates an empty adjacency list.
   **/
  AdjacencyList();

  /*!
   * \brief Determines whether `target` is directly reachable from `source`.
   * \param source The source vertex.
   * \param target The target vertex.
   * \return true if `target` is directly reachable from `source`; otherwise
   *         false.
   **/
  bool isDirectlyReachable(VertexIdentifier source, VertexIdentifier target)
    const;

  /*!
   * \brief Sets `target` to be directly reachable from `source`.
   * \param source The source vertex.
   * \param target The target vertex.
   **/
  void addTo(VertexIdentifier source, VertexIdentifier target);

  /*!
   * \brief Fetches all vertices that are directly reachable from `source`.
   * \param source The source vertex.
   * \return All vertices that are directly reachable from `source`.
   **/
  std::vector<VertexIdentifier> directlyReachables(
    VertexIdentifier source) const;

  /*!
   * \brief Sets `target` to not be directly reachable from `source`.
   * \param source The source vertex.
   * \param target The target vertex.
   **/
  void removeAdjacentFrom(VertexIdentifier source, VertexIdentifier target);

  /*!
   * \brief Removes the vertex `toRemove`.
   * \param toRemove The vertex to remove.
   **/
  void removeVertex(VertexIdentifier toRemove);

  /*!
   * \brief Clears this adjacency list, making it empty.
   **/
  void clear();

  /*!
   * \brief Swaps this `AdjacencyList` with `other`.
   * \param other The other `AdjacencyList` to swap with.
   **/
  void swap(this_type& other) noexcept;

private:
  /*!
   * \brief Determines whether `haystack` contains `needle` as an element.
   * \param haystack The container to search in.
   * \param needle The element to find.
   * \return true if `haystack` contains `needle` as an element; otherwise
   *         false.
   **/
  static bool contains(
    const std::vector<VertexIdentifier>& haystack,
    VertexIdentifier                     needle);

  /*!
   * \brief Fetches a pointer to the adjacency list for the given vertex.
   * \param vertexIdentifier The vertex to get the adjacency list of.
   * \return A pointer to the adjacency list of `vertexIdentifier`, or nullptr
   *         if `vertexIdentifier` is not in the `AdjacencyList`.
   **/
  std::vector<VertexIdentifier>* listOf(VertexIdentifier vertexIdentifier);

  /*!
   * \brief Fetches a pointer to the adjacency list for the given vertex.
   * \param vertexIdentifier The vertex to get the adjacency list of.
   * \return A pointer to the adjacency list of `vertexIdentifier`, or nullptr
   *         if `vertexIdentifier` is not in the `AdjacencyList`.
   **/
  const std::vector<VertexIdentifier>* listOf(
    VertexIdentifier vertexIdentifier) const;

  /*!
   * \brief The underlying data.
   *
   * This is indexed with the vertices fetching the adjacency list of that
   * vertex.
   **/
  std::vector<std::vector<VertexIdentifier>> m_data;
};

/*!
 * \brief Swaps two adjacency lists.
 * \param lhs The left hand side operand.
 * \param rhs The right hand side operand.
 **/
void swap(AdjacencyList& lhs, AdjacencyList& rhs) noexcept;
} // namespace g
#endif // INCG_G_ADJACENCY_LIST_HPP
