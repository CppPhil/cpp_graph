#include "algo/a_star/vertex_with_cost.hpp"

namespace g::algo::a_star {
VertexWithCost::VertexWithCost() : VertexWithCost{0, 0} {}

VertexWithCost::VertexWithCost(VertexIdentifier vertex, std::int64_t g)
  : m_vertex{vertex}, m_g{g}
{
}

VertexIdentifier VertexWithCost::vertex() const { return m_vertex; }

std::int64_t VertexWithCost::g() const { return m_g; }
} // namespace g::algo::a_star
