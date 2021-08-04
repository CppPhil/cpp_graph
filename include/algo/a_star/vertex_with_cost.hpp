#ifndef INCG_G_ALGO_A_STAR_VERTEX_WITH_COST_HPP
#define INCG_G_ALGO_A_STAR_VERTEX_WITH_COST_HPP
#include <cstdint>

#include "vertex_identifier.hpp"

namespace g::algo::a_star {
class VertexWithCost {
public:
  VertexWithCost();

  VertexWithCost(VertexIdentifier vertex, std::int64_t g);

  VertexIdentifier vertex() const;

  std::int64_t g() const;

private:
  VertexIdentifier m_vertex;
  std::int64_t     m_g;
};
} // namespace g::algo::a_star
#endif // INCG_G_ALGO_A_STAR_VERTEX_WITH_COST_HPP
