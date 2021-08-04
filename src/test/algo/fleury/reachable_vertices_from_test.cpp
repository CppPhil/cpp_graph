#include <doctest.h>

#include "adjacency_list.hpp"
#include "algo/fleury/reachable_vertices_from.hpp"
#include "graph.hpp"

namespace {
using graph_type = g::Graph<
  g::Directionality::Undirected,
  g::AdjacencyList,
  std::nullptr_t,
  std::nullptr_t>;

graph_type makeGraph()
{
  graph_type g{};

#define FL_ADD_VERTEX(vertex) \
  G_ASSERT(g.addVertex(vertex, nullptr), "Couldn't add %zu to graph.", vertex)

#define FL_ADD_EDGE(src, target)                   \
  G_ASSERT(                                        \
    g.addEdge(src, target, nullptr),               \
    "Couldn't add edge from %zu to %zu to graph.", \
    src,                                           \
    target)

  for (g::VertexIdentifier v{1}; v <= 7; ++v) { FL_ADD_VERTEX(v); }

  FL_ADD_EDGE(1, 4);
  FL_ADD_EDGE(1, 3);
  FL_ADD_EDGE(3, 2);
  FL_ADD_EDGE(5, 6);

  return g;
}
} // anonymous namespace

TEST_CASE("Fleury should be able to determine the count of vertices reachable")
{
  const graph_type g{makeGraph()};

  const std::int64_t ary[7]{4, 4, 4, 4, 2, 2, 1};

  for (g::VertexIdentifier i{1}; i <= 7; ++i) {
    const auto index{i - 1};
    REQUIRE_EQ(ary[index], g::algo::fleury::reachableVerticesFrom(g, i));
  }
}
