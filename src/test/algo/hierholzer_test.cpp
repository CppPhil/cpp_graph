#include <algorithm>

#include <doctest.h>

#include "adjacency_list.hpp"
#include "algo/hierholzer.hpp"
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

#define HH_ADD_VERTEX(vert) \
  G_ASSERT(g.addVertex(vert, nullptr), "Couldn't add %zu to graph.", vert)

#define HH_ADD_EDGE(source, target)                \
  G_ASSERT(                                        \
    g.addEdge(source, target, nullptr),            \
    "Couldn't add edge from %zu to %zu to graph.", \
    source,                                        \
    target)

  for (g::VertexIdentifier i{1}; i <= 9; ++i) { HH_ADD_VERTEX(i); }

  HH_ADD_EDGE(1, 2);
  HH_ADD_EDGE(1, 3);
  HH_ADD_EDGE(2, 3);
  HH_ADD_EDGE(2, 4);
  HH_ADD_EDGE(2, 5);
  HH_ADD_EDGE(3, 4);
  HH_ADD_EDGE(3, 8);
  HH_ADD_EDGE(5, 6);
  HH_ADD_EDGE(5, 7);
  HH_ADD_EDGE(5, 9);
  HH_ADD_EDGE(6, 7);
  HH_ADD_EDGE(8, 9);

  return g;
}
} // anonymous namespace

TEST_CASE("Hierholzer's algorithm should produce a circle")
{
  const graph_type                       graph{makeGraph()};
  const std::vector<g::VertexIdentifier> result{g::algo::hierholzer(graph)};

  REQUIRE_UNARY_FALSE(result.empty());

  const g::VertexIdentifier first{result.front()};
  const g::VertexIdentifier last{result.back()};

  CHECK_EQ(first, last);
}

TEST_CASE(
  "Hierholzer's algorithm should produce a result that contains all edges")
{
  const graph_type                       graph{makeGraph()};
  const std::vector<g::VertexIdentifier> result{g::algo::hierholzer(graph)};

  REQUIRE_UNARY_FALSE(result.empty());

  auto contains{
    [&result](g::VertexIdentifier source, g::VertexIdentifier target) {
      const g::VertexIdentifier ary[2]{source, target};
      const auto                it1{std::search(
        result.begin(), result.end(), std::begin(ary), std::end(ary))};

      if (it1 == result.end()) {
        const auto it2{std::search(
          result.begin(), result.end(), std::rbegin(ary), std::rend(ary))};
        return it2 != result.end();
      }

      return true;
    }};

  REQUIRE_UNARY(contains(1, 2));
  REQUIRE_UNARY(contains(1, 3));
  REQUIRE_UNARY(contains(2, 3));
  REQUIRE_UNARY(contains(2, 4));
  REQUIRE_UNARY(contains(2, 5));
  REQUIRE_UNARY(contains(3, 4));
  REQUIRE_UNARY(contains(3, 8));
  REQUIRE_UNARY(contains(5, 6));
  REQUIRE_UNARY(contains(5, 7));
  REQUIRE_UNARY(contains(5, 9));
  REQUIRE_UNARY(contains(6, 7));
  REQUIRE_UNARY(contains(8, 9));
}

TEST_CASE("Hierholzer's algorithm should produce a traversable path")
{
  const graph_type                       graph{makeGraph()};
  const std::vector<g::VertexIdentifier> result{g::algo::hierholzer(graph)};
  REQUIRE_UNARY_FALSE(result.empty());

  for (std::size_t i{0}; i < (result.size() - 1); ++i) {
    const g::VertexIdentifier current{result.at(i)};
    const g::VertexIdentifier next{result.at(i + 1)};
    REQUIRE_UNARY(graph.isDirectlyReachable(current, next));
  }
}
