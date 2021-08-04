#ifndef INCG_G_ALGO_FLEURY_IS_BRIDGE_HPP
#define INCG_G_ALGO_FLEURY_IS_BRIDGE_HPP
#include "algo/fleury/reachable_vertices_from.hpp"
#include "assert.hpp"
#include "graph.hpp"

namespace g::algo::fleury {
template<typename Implementation, typename VertexData, typename EdgeData>
[[nodiscard]] bool isBridge(
  const Graph<Directionality::Undirected, Implementation, VertexData, EdgeData>&
                   graph,
  VertexIdentifier edgeSource,
  VertexIdentifier edgeTarget)
{
  const std::int64_t count1{reachableVerticesFrom(graph, edgeSource)};
  Graph<Directionality::Undirected, Implementation, VertexData, EdgeData>
    graphCopy{graph};
  G_ASSERT(
    graphCopy.removeEdge(edgeSource, edgeTarget),
    "Couldn't remove edge from %zu to %zu from graph.",
    edgeSource,
    edgeTarget);
  const std::int64_t count2{reachableVerticesFrom(graphCopy, edgeSource)};
  return count2 < count1;
}
} // namespace g::algo::fleury
#endif // INCG_G_ALGO_FLEURY_IS_BRIDGE_HPP
