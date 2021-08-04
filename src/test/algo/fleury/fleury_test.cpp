#include <algorithm>

#include <doctest.h>

#include "adjacency_list.hpp"
#include "algo/fleury/algorithm.hpp"
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

  for (g::VertexIdentifier i{1}; i <= 9; ++i) { FL_ADD_VERTEX(i); }

  FL_ADD_EDGE(1, 2);
  FL_ADD_EDGE(1, 3);
  FL_ADD_EDGE(2, 4);
  FL_ADD_EDGE(4, 5);
  FL_ADD_EDGE(2, 5);
  FL_ADD_EDGE(3, 5);
  FL_ADD_EDGE(3, 6);
  FL_ADD_EDGE(5, 6);
  FL_ADD_EDGE(5, 8);
  FL_ADD_EDGE(5, 7);
  FL_ADD_EDGE(8, 9);
  FL_ADD_EDGE(7, 9);
  FL_ADD_EDGE(3, 2);

  return g;
}
} // anonymous namespace

TEST_CASE("fleury should construct a circle")
{
  const graph_type                       g{makeGraph()};
  const std::vector<g::VertexIdentifier> result{g::algo::fleury::algorithm(g)};

  REQUIRE_UNARY_FALSE(result.empty());
  const g::VertexIdentifier front{result.front()};
  const g::VertexIdentifier last{result.back()};

  CHECK_EQ(front, last);
}

TEST_CASE("fleury should traverse all edges")
{
  const graph_type                       g{makeGraph()};
  const std::vector<g::VertexIdentifier> result{g::algo::fleury::algorithm(g)};

  REQUIRE_UNARY_FALSE(result.empty());

  auto contains
    = [&g, &result](g::VertexIdentifier src, g::VertexIdentifier target) {
        const std::vector<g::VertexIdentifier> search{src, target};
        const auto                             it1{std::search(
          result.begin(), result.end(), search.begin(), search.end())};

        if (it1 == result.end()) {
          const auto it2{std::search(
            result.begin(), result.end(), search.rbegin(), search.rend())};
          return it2 != result.end();
        }

        return true;
      };

  CHECK_UNARY(contains(1, 2));
  CHECK_UNARY(contains(1, 3));
  CHECK_UNARY(contains(2, 4));
  CHECK_UNARY(contains(4, 5));
  CHECK_UNARY(contains(2, 5));
  CHECK_UNARY(contains(3, 5));
  CHECK_UNARY(contains(3, 6));
  CHECK_UNARY(contains(5, 6));
  CHECK_UNARY(contains(5, 8));
  CHECK_UNARY(contains(5, 7));
  CHECK_UNARY(contains(8, 9));
  CHECK_UNARY(contains(7, 9));
  CHECK_UNARY(contains(3, 2));
}

TEST_CASE("Fleury should find a traversable path")
{
  const graph_type                       g{makeGraph()};
  const std::vector<g::VertexIdentifier> result{g::algo::fleury::algorithm(g)};
  REQUIRE_UNARY_FALSE(result.empty());

  for (std::size_t i{0}; i < (result.size() - 1); ++i) {
    const g::VertexIdentifier current{result.at(i)};
    const g::VertexIdentifier next{result.at(i + 1)};

    REQUIRE_UNARY(g.isDirectlyReachable(current, next));
  }
}
