#include <ostream>
#include <utility>

#include "adjacency_matrix.hpp"
#include "assert.hpp"

namespace g {
std::ostream& operator<<(std::ostream& os, const AdjacencyMatrix& m)
{
  for (std::size_t row{0}; row < m.rowCount(); ++row) {
    for (std::size_t col{0}; col < m.columnCount(); ++col) {
      os << m.m_data[row][col] << ' ';
    }
    os << '\n';
  }

  return os;
}

AdjacencyMatrix::AdjacencyMatrix() : m_data{} {}

bool AdjacencyMatrix::isDirectlyReachable(
  VertexIdentifier source,
  VertexIdentifier target) const
{
  if (const std::optional<const_reference> optional{fetch(source, target)};
      optional.has_value()) {
    return *optional;
  }

  return false;
}

void AdjacencyMatrix::addTo(VertexIdentifier source, VertexIdentifier target)
{
  resizeToFitIfNecessary(source, target);
  std::optional<reference> optional{fetch(source, target)};

  G_ASSERT(optional.has_value(), "source: %zu, target: %zu", source, target);

  *optional = true;
}

std::vector<VertexIdentifier> AdjacencyMatrix::directlyReachables(
  VertexIdentifier source) const
{
  std::vector<VertexIdentifier> result{};

  for (std::size_t column{0}; column < columnCount(); ++column) {
    if (isDirectlyReachable(source, VertexIdentifier{column})) {
      result.push_back(VertexIdentifier{column});
    }
  }

  return result;
}

void AdjacencyMatrix::removeAdjacentFrom(
  VertexIdentifier source,
  VertexIdentifier target)
{
  std::optional<reference> optional{fetch(source, target)};

  if (!optional.has_value()) { return; }

  *optional = false;
}

void AdjacencyMatrix::removeVertex(VertexIdentifier toRemove)
{
  for (std::size_t column{0}; column < columnCount(); ++column) {
    removeAdjacentFrom(toRemove, VertexIdentifier{column});
  }

  for (std::size_t row{0}; row < rowCount(); ++row) {
    removeAdjacentFrom(VertexIdentifier{row}, toRemove);
  }
}

void AdjacencyMatrix::clear() { m_data.clear(); }

void AdjacencyMatrix::swap(this_type& other) noexcept
{
  m_data.swap(other.m_data);
}

std::size_t AdjacencyMatrix::rowCount() const { return m_data.size(); }

std::size_t AdjacencyMatrix::columnCount() const
{
  if (m_data.empty()) { return 0; }

  return m_data.front().size();
}

std::optional<AdjacencyMatrix::reference> AdjacencyMatrix::fetch(
  std::size_t row,
  std::size_t column)
{
  if (row >= rowCount()) { return std::nullopt; }

  if (column >= columnCount()) { return std::nullopt; }

  return m_data[row][column];
}

std::optional<AdjacencyMatrix::const_reference> AdjacencyMatrix::fetch(
  std::size_t row,
  std::size_t column) const
{
  const std::optional<reference> optional{
    const_cast<this_type*>(this)->fetch(row, column)};

  if (!optional.has_value()) { return std::nullopt; }

  return *optional;
}

void AdjacencyMatrix::resizeToFitIfNecessary(
  VertexIdentifier source,
  VertexIdentifier target)
{
  const std::size_t newRowCount{source + 1};
  const std::size_t newColumnCount{target + 1};

  if (newRowCount > rowCount()) {
    const std::vector<bool> defaultValue(columnCount());
    m_data.resize(newRowCount, defaultValue);
  }
  G_ASSERT(
    newRowCount <= rowCount(),
    "newRowCount: %zu, rowCount(): %zu",
    newRowCount,
    rowCount());

  if (newColumnCount > columnCount()) {
    for (std::vector<bool>& row : m_data) {
      if (row.size() < newColumnCount) { row.resize(newColumnCount); }
      G_ASSERT(
        row.size() >= newColumnCount,
        "row.size(): %zu, newColumnCount: %zu",
        row.size(),
        newColumnCount);
    }
  }
  G_ASSERT(
    newColumnCount <= columnCount(),
    "newColumnCount: %zu, columnCount(): %zu",
    newColumnCount,
    columnCount());
}

void swap(AdjacencyMatrix& lhs, AdjacencyMatrix& rhs) noexcept
{
  lhs.swap(rhs);
}
} // namespace g
