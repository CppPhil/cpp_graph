#include <ostream>

#include "edge.hpp"

namespace g {
std::ostream& operator<<(std::ostream& os, const Edge& edge)
{
  return os << "Edge{source: " << edge.m_source << ", target: " << edge.m_target
            << '}';
}

Edge::Edge(VertexIdentifier source, VertexIdentifier target)
  : m_source{source}, m_target{target}
{
}

VertexIdentifier Edge::source() const { return m_source; }

VertexIdentifier Edge::target() const { return m_target; }
} // namespace g
