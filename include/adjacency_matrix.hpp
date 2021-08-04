#ifndef INCG_G_ADJACENCY_MATRIX_HPP
#define INCG_G_ADJACENCY_MATRIX_HPP
#include <cstddef>

#include <iosfwd>
#include <optional>
#include <vector>

#include "vertex_identifier.hpp"

namespace g {
/*!
 * \brief An implementation type for the `Graph` type, implementing an
 *        adjacency matrix.
 **/
class AdjacencyMatrix {
public:
  using this_type       = AdjacencyMatrix;
  using reference       = std::vector<bool>::reference;
  using const_reference = std::vector<bool>::const_reference;

  /*!
   * \brief Prints an `AdjacencyMatrix` to an ostream.
   * \param os The ostream to print to.
   * \param m The `AdjacencyMatrix` to print.
   * \return `os`.
   **/
  friend std::ostream& operator<<(std::ostream& os, const this_type& m);

  /*!
   * \brief Constructs an empty adjacency matrix.
   **/
  AdjacencyMatrix();

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
   * \brief Clears this adjacency matrix, making it empty.
   **/
  void clear();

  /*!
   * \brief Swaps this `AdjacencyMatrix` with `other`.
   * \param other The other `AdjacencyMatrix` to swap with.
   **/
  void swap(this_type& other) noexcept;

private:
  /*!
   * \brief Determines the count of rows.
   * \return The count of rows.
   **/
  std::size_t rowCount() const;

  /*!
   * \brief Determines the count of columns.
   * \return The count of columns.
   **/
  std::size_t columnCount() const;

  /*!
   * \brief Returns the cell at `row` and `column`.
   * \param row The row.
   * \param column The column.
   * \return An optional containing a reference proxy object to the cell value,
   *         or nullopt if there is no cell at `row` and `column`.
   **/
  std::optional<reference> fetch(std::size_t row, std::size_t column);

  /*!
   * \brief Returns the boolean value at `row` and `column`.
   * \param row The row.
   * \param column The column.
   * \return An optional containing the boolean value, or nullopt of there is
   *         no cell at `row` and `column`.
   **/
  std::optional<const_reference> fetch(std::size_t row, std::size_t column)
    const;

  /*!
   * \brief Resizes this adjacency matrix to be able to hold a cell value at
   *        `source` and `target` if necessary.
   * \param source The source vertex.
   * \param target The target vertex.
   **/
  void resizeToFitIfNecessary(VertexIdentifier source, VertexIdentifier target);

  /*!
   * \brief The underlying data.
   *
   * Represents the matrix.
   **/
  std::vector<std::vector<bool>> m_data;
};

/*!
 * \brief Swaps two adjacency matrices.
 * \param lhs The left hand side operand.
 * \param rhs The right hand side operand.
 **/
void swap(AdjacencyMatrix& lhs, AdjacencyMatrix& rhs) noexcept;
} // namespace g
#endif // INCG_G_ADJACENCY_MATRIX_HPP
