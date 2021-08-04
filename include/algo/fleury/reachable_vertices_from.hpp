#ifndef INCG_G_ALGO_FLEURY_REACHABLE_VERTICES_FROM_HPP
#define INCG_G_ALGO_FLEURY_REACHABLE_VERTICES_FROM_HPP
#include <cstdint>

#include <unordered_map>

#include "graph.hpp"

namespace g::algo::fleury {
namespace detail {
template<typename Implementation, typename VertexData, typename EdgeData>
[[nodiscard]] std::int64_t reachableVerticesFromImpl(
  const Graph<Directionality::Undirected, Implementation, VertexData, EdgeData>&
                                              graph,
  VertexIdentifier                            vertex,
  std::unordered_map<VertexIdentifier, bool>& isVisited)
{
  isVisited[vertex] = true;
  std::int64_t count{1};

  for (const std::vector<VertexIdentifier> neighbors{
         graph.directlyReachables(vertex)};
       VertexIdentifier neighbor : neighbors) {
    if (!isVisited[neighbor]) {
      count += reachableVerticesFromImpl(graph, neighbor, isVisited);
    }
  }

  return count;
}
} // namespace detail

template<typename Implementation, typename VertexData, typename EdgeData>
[[nodiscard]] std::int64_t reachableVerticesFrom(
  const Graph<Directionality::Undirected, Implementation, VertexData, EdgeData>&
                   graph,
  VertexIdentifier vertex)
{
  std::unordered_map<VertexIdentifier, bool> isVisited{};

  for (const std::vector<VertexIdentifier> vertices{graph.vertices()};
       VertexIdentifier                    v : vertices) {
    isVisited.emplace(v, false);
  }

  return detail::reachableVerticesFromImpl(graph, vertex, isVisited);
}
} // namespace g::algo::fleury
#endif // INCG_G_ALGO_FLEURY_REACHABLE_VERTICES_FROM_HPP
