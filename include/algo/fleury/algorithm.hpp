#ifndef INCG_G_ALGO_FLEURY_ALGORITHM_HPP
#define INCG_G_ALGO_FLEURY_ALGORITHM_HPP
#include <algorithm>

#include "algo/fleury/is_bridge.hpp"
#include "graph.hpp"

namespace g::algo::fleury {
namespace detail {
template<typename Implementation, typename VertexData, typename EdgeData>
[[nodiscard]] VertexIdentifier pickNextVertex(
  const Graph<Directionality::Undirected, Implementation, VertexData, EdgeData>&
                                       graph,
  VertexIdentifier                     currentVertex,
  const std::vector<VertexIdentifier>& neighbors)
{
  if (neighbors.size() == 1) { return neighbors.front(); }

  const std::vector<VertexIdentifier>::const_iterator it{std::find_if(
    neighbors.begin(),
    neighbors.end(),
    [&graph, currentVertex](VertexIdentifier target) {
      return !isBridge(graph, currentVertex, target);
    })};
  G_ASSERT(it != neighbors.end(), "%s", "Could not find bridge in neighbors!");

  return *it;
}
} // namespace detail

template<typename Implementation, typename VertexData, typename EdgeData>
[[nodiscard]] std::vector<VertexIdentifier> algorithm(
  Graph<Directionality::Undirected, Implementation, VertexData, EdgeData> graph)
{
  G_ASSERT(graph.hasVertices(), "%s", "Graph had no vertices!");

  const std::vector<VertexIdentifier> vertices{graph.vertices()};
  const VertexIdentifier              startVertex{vertices.front()};
  VertexIdentifier                    currentVertex{startVertex};
  std::vector<VertexIdentifier>       result{currentVertex};
  std::vector<VertexIdentifier>       neighbors{};

  while (!(neighbors = graph.directlyReachables(currentVertex)).empty()) {
    const VertexIdentifier nextVertex{
      detail::pickNextVertex(graph, currentVertex, neighbors)};
    result.push_back(nextVertex);
    G_ASSERT(
      graph.removeEdge(currentVertex, nextVertex),
      "Couldn't remove edge from %zu to %zu from graph.",
      currentVertex,
      nextVertex);
    currentVertex = nextVertex;
  }

  return result;
}
} // namespace g::algo::fleury
#endif // INCG_G_ALGO_FLEURY_ALGORITHM_HPP
