#ifndef INCG_G_ALGO_FORD_FULKERSON_ALGORITHM_HPP
#define INCG_G_ALGO_FORD_FULKERSON_ALGORITHM_HPP
#include <algorithm>

#include "algo/ford_fulkerson/breadth_first_search.hpp"
#include "assert.hpp"

namespace g::algo::ford_fulkerson {
class Result {
public:
  Result(
    std::int32_t                                  maxFlow,
    const std::unordered_map<Edge, std::int32_t>& flow);

  std::int32_t maxFlow() const;

  const std::unordered_map<Edge, std::int32_t>& flow() const;

private:
  std::int32_t                           m_maxFlow;
  std::unordered_map<Edge, std::int32_t> m_flow;
};

template<
  Directionality TheDirectionality,
  typename Implementation,
  typename VertexData>
Result algorithm(
  const Graph<TheDirectionality, Implementation, VertexData, std::int32_t>&
                   graph,
  VertexIdentifier source,
  VertexIdentifier sink)
{
  std::int32_t                           maxFlow{0};
  std::unordered_map<Edge, std::int32_t> flow{};
  std::unordered_map<Edge, std::int32_t> capacity{};

  for (const std::vector<VertexIdentifier> vertices{graph.vertices()};
       VertexIdentifier                    vertex1 : vertices) {
    for (VertexIdentifier vertex2 : vertices) {
      flow[Edge{vertex1, vertex2}]     = 0;
      capacity[Edge{vertex1, vertex2}] = 0;
    }
  }

  for (const std::vector<Edge> edges{graph.edgesUndirectedWithReverseEdges()};
       const Edge&             edge : edges) {
    const std::optional<std::int32_t> optionalEdgeCapacity{
      graph.fetchEdgeData(edge.source(), edge.target())};
    G_ASSERT(
      optionalEdgeCapacity.has_value(),
      "Couldn't find edge capacity for edge from %zu to %zu",
      edge.source(),
      edge.target());
    capacity[edge] = *optionalEdgeCapacity;
  }

  std::optional<
    std::unordered_map<VertexIdentifier, std::optional<VertexIdentifier>>>
    predecessor{std::nullopt};

  while ((predecessor = breadthFirstSearch(graph, source, sink, capacity, flow))
           .has_value()) {
    std::int32_t increment{INT32_MAX};

    for (VertexIdentifier u{sink}; (*predecessor)[u].has_value();
         u = (*predecessor)[u].value()) {
      increment = std::min(
        increment,
        capacity[Edge{(*predecessor)[u].value(), u}]
          - flow[Edge{(*predecessor)[u].value(), u}]);
    }

    for (VertexIdentifier u{sink}; (*predecessor)[u].has_value();
         u = (*predecessor)[u].value()) {
      flow[Edge{(*predecessor)[u].value(), u}] += increment;
      flow[Edge{u, (*predecessor)[u].value()}] -= increment;
    }

    maxFlow += increment;
  }

  return Result{maxFlow, flow};
}
} // namespace g::algo::ford_fulkerson
#endif // INCG_G_ALGO_FORD_FULKERSON_ALGORITHM_HPP
