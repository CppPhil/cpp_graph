#ifndef INCG_G_ALGO_DIJKSTRA_ALGORITHM_HPP
#define INCG_G_ALGO_DIJKSTRA_ALGORITHM_HPP
#include <cstdint>

#include <algorithm>
#include <deque>
#include <optional>
#include <unordered_map>
#include <vector>

#include "algo/dijkstra/insert.hpp"
#include "algo/dijkstra/vertex_with_priority.hpp"
#include "algo/shortest_paths.hpp"
#include "assert.hpp"
#include "directionality.hpp"
#include "graph.hpp"

namespace g::algo::dijkstra {
template<
  Directionality TheDirectionality,
  typename Implementation,
  typename VertexData>
[[nodiscard]] ShortestPaths algorithm(
  const Graph<TheDirectionality, Implementation, VertexData, std::int64_t>&
                   graph,
  VertexIdentifier source)
{
  constexpr std::int64_t                infinity{INT64_MAX};
  const std::optional<VertexIdentifier> undefined{std::nullopt};

  std::unordered_map<VertexIdentifier, std::optional<VertexIdentifier>> prev{};
  std::unordered_map<VertexIdentifier, std::int64_t>                    dist{};
  std::deque<VertexWithPriority>                                        queue{};
  const std::vector<VertexIdentifier> vertices{graph.vertices()};

  dist[source] = 0;

  for (VertexIdentifier v : vertices) {
    if (v != source) { dist[v] = infinity; }

    prev[v] = undefined;
    insert(queue, VertexWithPriority{v, dist[v]});
  }

  while (!queue.empty()) {
    const VertexWithPriority u{queue.front()};
    queue.pop_front();
    const std::vector<VertexIdentifier> directlyReachables{
      graph.directlyReachables(u.vertex())};

    for (VertexIdentifier directlyReachable : directlyReachables) {
      const auto queueIterator{std::find_if(
        queue.begin(),
        queue.end(),
        [directlyReachable](const VertexWithPriority& vertexWithPriority) {
          return vertexWithPriority.vertex() == directlyReachable;
        })};

      if (queueIterator == queue.end()) { continue; }

      const std::optional<std::int64_t> edgeData{
        graph.fetchEdgeData(u.vertex(), directlyReachable)};

      G_ASSERT(
        edgeData.has_value(),
        "edgeData optional had no value for edge from %zu to %zu",
        u.vertex(),
        directlyReachable);

      const std::int64_t alt{dist[u.vertex()] + *edgeData};
      const std::unordered_map<VertexIdentifier, std::int64_t>::iterator
        neighborIt{dist.find(directlyReachable)};
      G_ASSERT(
        neighborIt != dist.end(),
        "Could not find %zu in dist.",
        directlyReachable);
      std::int64_t& distanceNeighbor{neighborIt->second};

      if (alt < distanceNeighbor) {
        distanceNeighbor        = alt;
        prev[directlyReachable] = u.vertex();
        queue.erase(queueIterator);
        insert(queue, VertexWithPriority{directlyReachable, alt});
      }
    }
  }

  return ShortestPaths{source, prev, dist};
}
} // namespace g::algo::dijkstra
#endif // INCG_G_ALGO_DIJKSTRA_ALGORITHM_HPP
