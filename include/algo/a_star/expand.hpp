#ifndef INCG_G_ALGO_A_STAR_EXPAND_HPP
#define INCG_G_ALGO_A_STAR_EXPAND_HPP
#include <cstdint>

#include <algorithm>
#include <vector>

#include "algo/a_star/vertex_with_cost.hpp"
#include "assert.hpp"
#include "graph.hpp"

namespace g::algo::a_star {
template<
  Directionality TheDirectionality,
  typename Implementation,
  typename VertexData>
std::vector<VertexWithCost> expand(
  VertexIdentifier vertexToExpand,
  const Graph<TheDirectionality, Implementation, VertexData, std::int64_t>&
    graph)
{
  const std::vector<VertexIdentifier> directlyReachableVertices{
    graph.directlyReachables(vertexToExpand)};
  std::vector<VertexWithCost> result(directlyReachableVertices.size());
  std::transform(
    directlyReachableVertices.begin(),
    directlyReachableVertices.end(),
    result.begin(),
    [&graph, source = vertexToExpand](VertexIdentifier target) {
      const std::optional<std::int64_t> optionalEdgeLength{
        graph.fetchEdgeData(source, target)};
      G_ASSERT(
        optionalEdgeLength.has_value(),
        "Couldn't fetch edge data for edge from %zu to %zu",
        source,
        target);
      return VertexWithCost{target, *optionalEdgeLength};
    });
  return result;
}
} // namespace g::algo::a_star
#endif // INCG_G_ALGO_A_STAR_EXPAND_HPP
