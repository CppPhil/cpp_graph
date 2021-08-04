#ifndef INCG_G_NM_CREATE_GRAPH_HPP
#define INCG_G_NM_CREATE_GRAPH_HPP
#include <cinttypes>
#include <cstdint>

#include "assert.hpp"
#include "graph.hpp"

namespace g::nm {
template<typename Implementation>
using graph_type = Graph<
  Directionality::Directed,
  Implementation,
  std::nullptr_t,
  std::int64_t>;

template<typename Implementation>
graph_type<Implementation> createGraph()
{
  graph_type<Implementation> graph{};

  auto addVertex{[&graph](VertexIdentifier vertex) {
    G_ASSERT(
      graph.addVertex(vertex, nullptr), "Could not add vertex %zu", vertex);
  }};

  auto addEdge{[&graph](
                 VertexIdentifier source,
                 VertexIdentifier target,
                 std::int64_t     edgeLength) {
    G_ASSERT(
      graph.addEdge(source, target, edgeLength),
      "Could not add edge from %zu to %zu with length %" PRId64,
      edgeLength);
  }};

  for (VertexIdentifier i{1}; i <= 6; ++i) { addVertex(i); }

  addEdge(1, 2, 5);
  addEdge(1, 3, 5);
  addEdge(2, 4, 6);
  addEdge(3, 4, 5);
  addEdge(4, 5, 5);
  addEdge(5, 6, 5);

  return graph;
}
} // namespace g::nm
#endif // INCG_G_NM_CREATE_GRAPH_HPP
