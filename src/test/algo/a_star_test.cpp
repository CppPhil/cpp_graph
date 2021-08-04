#include <doctest.h>

#include "adjacency_list.hpp"
#include "algo/a_star/algorithm.hpp"
#include "nm/create_graph.hpp"
#include "nm/heuristic.hpp"
#include "romania/create_graph.hpp"
#include "romania/heuristic.hpp"

TEST_CASE("A* should work for Romania")
{
  const g::romania::graph_type<g::AdjacencyList> graph{
    g::romania::createGraph<g::AdjacencyList>()};
  const g::algo::a_star::Path            path{g::algo::a_star::algorithm(
    graph,
    {g::romania::City::Arad},
    [](g::VertexIdentifier city) {
      return city == g::romania::City::Bucharest;
    },
    &g::romania::heuristic,
    /* useClosedList */ true)};
  constexpr std::int64_t                 expectedTotalCost{418};
  const std::vector<g::VertexIdentifier> expectedPath{
    g::romania::City::Arad,
    g::romania::City::Sibiu,
    g::romania::City::RimnicuVilcea,
    g::romania::City::Pitesti,
    g::romania::City::Bucharest};

  CHECK_EQ(path.g(), expectedTotalCost);
  CHECK_EQ(path.rawPath(), expectedPath);
}

TEST_CASE("A* should work for the non-monotonic graph")
{
  const g::VertexIdentifier                 startVertex{1};
  const g::nm::graph_type<g::AdjacencyList> graph{
    g::nm::createGraph<g::AdjacencyList>()};
  const g::algo::a_star::Path path{g::algo::a_star::algorithm(
    graph,
    {startVertex},
    [](g::VertexIdentifier v) { return v == 6; },
    &g::nm::heuristic,
    false)};

  constexpr std::int64_t expectedTotalCost{20};
  CHECK_EQ(path.g(), expectedTotalCost);

  const std::vector<g::VertexIdentifier> expectedPath{1, 3, 4, 5, 6};
  CHECK_EQ(path.rawPath(), expectedPath);
}
