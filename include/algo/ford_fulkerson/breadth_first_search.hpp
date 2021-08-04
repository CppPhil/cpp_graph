#ifndef INCG_G_ALGO_FORD_FULKERSON_BREADTH_FIRST_SEARCH_HPP
#define INCG_G_ALGO_FORD_FULKERSON_BREADTH_FIRST_SEARCH_HPP
#include <cstdint>

#include <optional>
#include <queue>
#include <unordered_map>

#include "edge.hpp"
#include "graph.hpp"

namespace g::algo::ford_fulkerson {
enum class Color { White, Gray, Black };

void enqueue(
  std::queue<VertexIdentifier>&                q,
  VertexIdentifier                             x,
  std::unordered_map<VertexIdentifier, Color>& color);

VertexIdentifier dequeue(
  std::queue<VertexIdentifier>&                q,
  std::unordered_map<VertexIdentifier, Color>& color);

template<
  Directionality TheDirectionality,
  typename Implementation,
  typename VertexData>
std::optional<
  std::unordered_map<VertexIdentifier, std::optional<VertexIdentifier>>>
breadthFirstSearch(
  const Graph<TheDirectionality, Implementation, VertexData, std::int32_t>&
                                                graph,
  VertexIdentifier                              start,
  VertexIdentifier                              target,
  const std::unordered_map<Edge, std::int32_t>& capacity,
  const std::unordered_map<Edge, std::int32_t>& flow)
{
  std::unordered_map<VertexIdentifier, Color>                           color{};
  std::unordered_map<VertexIdentifier, std::optional<VertexIdentifier>> pred{};
  std::queue<VertexIdentifier>                                          q{};

  for (const std::vector<VertexIdentifier> vertices{graph.vertices()};
       VertexIdentifier                    u : vertices) {
    color[u] = Color::White;
    pred[u]  = std::nullopt;
  }

  enqueue(q, start, color);

  while (!q.empty()) {
    const VertexIdentifier u{dequeue(q, color)};

    for (const std::vector<VertexIdentifier> targets{
           graph.directlyReachables(u)};
         VertexIdentifier v : targets) {
      if (
        color[v] == Color::White
        && capacity.at(Edge{u, v}) - flow.at(Edge{u, v}) > 0) {
        enqueue(q, v, color);
        pred[v] = u;
      }
    }
  }

  if (color[target] == Color::Black) { return pred; }

  return std::nullopt;
}
} // namespace g::algo::ford_fulkerson
#endif // INCG_G_ALGO_FORD_FULKERSON_BREADTH_FIRST_SEARCH_HPP
