#ifndef INCG_G_ALGO_BELLMAN_FORD_HPP
#define INCG_G_ALGO_BELLMAN_FORD_HPP
#include <cinttypes>
#include <cstddef>
#include <cstdint>

#include <optional>
#include <unordered_map>

#include "algo/shortest_paths.hpp"
#include "assert.hpp"
#include "graph.hpp"
#include "graph_exception.hpp"
#include "throw.hpp"
#include "vertex_identifier.hpp"

namespace g::algo {
template<Directionality TheDirectionality, typename Implementation>
ShortestPaths bellmanFord(
  const Graph<TheDirectionality, Implementation, std::nullptr_t, std::int64_t>&
                   graph,
  VertexIdentifier source)
{
  constexpr std::int64_t                infinity{INT32_MAX};
  const std::optional<VertexIdentifier> undefined{std::nullopt};
  const std::vector<VertexIdentifier>   vertices{graph.vertices()};
  const std::vector<Edge> edges{graph.edgesUndirectedWithReverseEdges()};

  std::unordered_map<VertexIdentifier, std::int64_t> distance{};
  std::unordered_map<VertexIdentifier, std::optional<VertexIdentifier>>
    predecessor{};

  for (VertexIdentifier vertex : vertices) {
    distance[vertex]    = infinity;
    predecessor[vertex] = undefined;
  }

  distance[source] = 0;

  for (std::size_t i{1}; i < vertices.size(); ++i) {
    for (const Edge& edge : edges) {
      const VertexIdentifier            u{edge.source()};
      const VertexIdentifier            v{edge.target()};
      const std::optional<std::int64_t> optEdgeLength{
        graph.fetchEdgeData(u, v)};
      G_ASSERT(
        optEdgeLength.has_value(), "No value for edge from %zu to %zu!", u, v);
      const std::int64_t w{*optEdgeLength};
      const std::int64_t distU{distance[u]};
      const std::unordered_map<VertexIdentifier, std::int64_t>::iterator vIt{
        distance.find(v)};
      G_ASSERT(vIt != distance.end(), "Could not find %zu in distance map!", v);
      std::int64_t& distV{vIt->second};

      if (const std::int64_t alt{distU + w}; alt < distV) {
        distV          = alt;
        predecessor[v] = u;
      }
    }
  }

  for (const Edge& edge : edges) {
    const VertexIdentifier            u{edge.source()};
    const VertexIdentifier            v{edge.target()};
    const std::optional<std::int64_t> optEdgeLength{graph.fetchEdgeData(u, v)};
    G_ASSERT(
      optEdgeLength.has_value(), "No value for edge from %zu to %zu!", u, v);
    const std::int64_t w{*optEdgeLength};
    const std::int64_t distU{distance[u]};

    if (const std::int64_t distV{distance[v]}; distU + w < distV) {
      G_THROW(
        GraphException,
        "Negative weight cycle detected! distU (%" PRId64 ") + w (%" PRId64
        ") which maskes %" PRId64 " was less than distV (%" PRId64 ")!",
        distU,
        w,
        distU + w,
        distV);
    }
  }

  return ShortestPaths{source, predecessor, distance};
}
} // namespace g::algo
#endif // INCG_G_ALGO_BELLMAN_FORD_HPP
