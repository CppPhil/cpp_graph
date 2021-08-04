#ifndef INCG_G_ALGO_A_STAR_PATH_HPP
#define INCG_G_ALGO_A_STAR_PATH_HPP
#include <cstdint>

#include <vector>

#include "algo/a_star/vertex_with_cost.hpp"

namespace g::algo::a_star {
class Path {
public:
  using this_type = Path;

  Path();

  void append(VertexWithCost vertexWithCost);

  VertexWithCost& back();

  const VertexWithCost& back() const;

  std::int64_t g() const;

  std::vector<VertexIdentifier> rawPath() const;

private:
  std::int64_t                m_totalG;
  std::vector<VertexWithCost> m_vector;
};
} // namespace g::algo::a_star
#endif // INCG_G_ALGO_A_STAR_PATH_HPP
