#include <algorithm>
#include <ostream>

#include "adjacency_list.hpp"
#include "assert.hpp"

namespace g {
std::ostream& operator<<(std::ostream& os, const AdjacencyList& l)
{
  os << "AdjacencyList:\n";

  for (std::size_t i{0}; i < l.m_data.size(); ++i) {
    os << i << ": ";

    for (const auto& e : l.m_data[i]) { os << e << ' '; }

    os << '\n';
  }

  return os;
}

AdjacencyList::AdjacencyList() : m_data{} {}

bool AdjacencyList::isDirectlyReachable(
  VertexIdentifier source,
  VertexIdentifier target) const
{
  const std::vector<VertexIdentifier>* list{listOf(source)};

  if (list == nullptr) { return false; }

  return contains(*list, target);
}

void AdjacencyList::addTo(VertexIdentifier source, VertexIdentifier target)
{
  while (source >= m_data.size()) { m_data.emplace_back(); }

  G_ASSERT(
    m_data.size() >= source,
    "m_data.size(): %zu, source: %zu",
    m_data.size(),
    source);

  std::vector<VertexIdentifier>* list{listOf(source)};

  G_ASSERT(list != nullptr, "source: %zu, target: %zu", source, target);

  if (!contains(*list, target)) { list->push_back(target); }
}

std::vector<VertexIdentifier> AdjacencyList::directlyReachables(
  VertexIdentifier source) const
{
  if (const std::vector<VertexIdentifier>* list{listOf(source)};
      list != nullptr) {
    return *list;
  }

  return {};
}

void AdjacencyList::removeAdjacentFrom(
  VertexIdentifier source,
  VertexIdentifier target)
{
  std::vector<VertexIdentifier>* list{listOf(source)};

  if (list == nullptr) { return; }

  if (const auto it{std::find(list->begin(), list->end(), target)};
      it != list->end()) {
    list->erase(it);
  }
}

void AdjacencyList::removeVertex(VertexIdentifier toRemove)
{
  if (std::vector<VertexIdentifier> * list{listOf(toRemove)}; list != nullptr) {
    list->clear();
  }

  for (std::size_t row{0}; row < m_data.size(); ++row) {
    removeAdjacentFrom(VertexIdentifier{row}, toRemove);
  }
}

void AdjacencyList::clear() { m_data.clear(); }

void AdjacencyList::swap(this_type& other) noexcept
{
  m_data.swap(other.m_data);
}

bool AdjacencyList::contains(
  const std::vector<VertexIdentifier>& haystack,
  VertexIdentifier                     needle)
{
  const auto it{std::find(haystack.begin(), haystack.end(), needle)};
  return it != haystack.end();
}

std::vector<VertexIdentifier>* AdjacencyList::listOf(
  VertexIdentifier vertexIdentifier)
{
  if (vertexIdentifier >= m_data.size()) { return nullptr; }

  return &m_data[vertexIdentifier];
}

const std::vector<VertexIdentifier>* AdjacencyList::listOf(
  VertexIdentifier vertexIdentifier) const
{
  return const_cast<this_type*>(this)->listOf(vertexIdentifier);
}

void swap(AdjacencyList& lhs, AdjacencyList& rhs) noexcept { lhs.swap(rhs); }
} // namespace g
