#include "algo/dijkstra/vertex_with_priority.hpp"

namespace g::algo::dijkstra {
VertexWithPriority::VertexWithPriority(
  VertexIdentifier vertex,
  std::int64_t     priority)
  : m_vertex{vertex}, m_priority{priority}
{
}

VertexIdentifier VertexWithPriority::vertex() const { return m_vertex; }

std::int64_t VertexWithPriority::priority() const { return m_priority; }
} // namespace g::algo::dijkstra
