#ifndef INCG_G_ALGO_DIJKSTRA_VERTEX_WITH_PRIORITY_HPP
#define INCG_G_ALGO_DIJKSTRA_VERTEX_WITH_PRIORITY_HPP
#include <cstdint>

#include "vertex_identifier.hpp"

namespace g::algo::dijkstra {
class VertexWithPriority {
public:
  VertexWithPriority(VertexIdentifier vertex, std::int64_t priority);

  [[nodiscard]] VertexIdentifier vertex() const;

  std::int64_t priority() const;

private:
  VertexIdentifier m_vertex;
  std::int64_t     m_priority;
};
} // namespace g::algo::dijkstra
#endif // INCG_G_ALGO_DIJKSTRA_VERTEX_WITH_PRIORITY_HPP
