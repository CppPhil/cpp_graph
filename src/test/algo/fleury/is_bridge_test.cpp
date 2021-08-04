#include <array>

#include <doctest.h>

#include "adjacency_list.hpp"
#include "algo/fleury/is_bridge.hpp"
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

  for (g::VertexIdentifier v{1}; v <= 9; ++v) { FL_ADD_VERTEX(v); }

  FL_ADD_EDGE(1, 2);
  FL_ADD_EDGE(1, 3);
  FL_ADD_EDGE(2, 3);
  FL_ADD_EDGE(3, 4);
  FL_ADD_EDGE(4, 5);
  FL_ADD_EDGE(5, 6);
  FL_ADD_EDGE(5, 7);
  FL_ADD_EDGE(6, 8);
  FL_ADD_EDGE(7, 8);

  return g;
}
} // anonymous namespace

TEST_CASE("is bridge test should work")
{
  const graph_type                      g{makeGraph()};
  static constexpr std::array<bool, 9U> expectedArray{
    /* Edge 1 */ false,
    /* Edge 2 */ false,
    /* Edge 3 */ false,
    /* Edge 4 */ true,
    /* Edge 5 */ true,
    /* Edge 6 */ false,
    /* Edge 7 */ false,
    /* Edge 8 */ false,
    /* Edge 9 */ false};
  const std::vector<g::Edge> edges{
    g::Edge{1, 2},
    g::Edge{1, 3},
    g::Edge{2, 3},
    g::Edge{3, 4},
    g::Edge{4, 5},
    g::Edge{5, 6},
    g::Edge{5, 7},
    g::Edge{6, 8},
    g::Edge{7, 8}};

  REQUIRE_EQ(edges.size(), expectedArray.size());

  auto expected{[](std::size_t value) { return expectedArray.at(value - 1); }};

  for (std::size_t i{}; i < expectedArray.size(); ++i) {
    const std::size_t val{i + 1};

    REQUIRE_EQ(
      expected(val),
      g::algo::fleury::isBridge(g, edges.at(i).source(), edges.at(i).target()));
  }
}
