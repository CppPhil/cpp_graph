#include <cinttypes>

#include <locale>
#include <sstream>

#include <doctest.h>

#include "adjacency_list.hpp"
#include "algo/bellman_ford.hpp"
#include "nm/create_graph.hpp"
#include "romania/create_graph.hpp"

namespace doctest {
template<>
struct StringMaker<std::vector<g::VertexIdentifier>> {
  static String convert(const std::vector<g::VertexIdentifier>& vector)
  {
    std::ostringstream os{};
    os.imbue(std::locale::classic());
    os << "vector{";

    for (std::size_t i{0}; i < vector.size(); ++i) {
      if (i != (vector.size() - 1)) { os << vector[i] << ", "; }
      else {
        os << vector[i];
      }
    }

    os << '}';
    return String{os.str().c_str()};
  }
};
} // namespace doctest

TEST_CASE("bellman-ford should work for a non-monotonic graph")
{
  const g::nm::graph_type<g::AdjacencyList> graph{
    g::nm::createGraph<g::AdjacencyList>()};

  const g::VertexIdentifier startVertex{1};

  const g::algo::ShortestPaths result{g::algo::bellmanFord(graph, startVertex)};

  const std::int64_t oneExpectedDistance{0};
  const std::int64_t twoExpectedDistance{5};
  const std::int64_t threeExpectedDistance{5};
  const std::int64_t fourExpectedDistance{10};
  const std::int64_t fiveExpectedDistance{15};
  const std::int64_t sixExpectedDistance{20};

  CHECK_EQ(result.distanceTo(1), oneExpectedDistance);
  CHECK_EQ(result.distanceTo(2), twoExpectedDistance);
  CHECK_EQ(result.distanceTo(3), threeExpectedDistance);
  CHECK_EQ(result.distanceTo(4), fourExpectedDistance);
  CHECK_EQ(result.distanceTo(5), fiveExpectedDistance);
  CHECK_EQ(result.distanceTo(6), sixExpectedDistance);

  using Path = std::vector<g::VertexIdentifier>;
  const Path oneExpectedPath{1};
  const Path twoExpectedPath{1, 2};
  const Path threeExpectedPath{1, 3};
  const Path fourExpectedPath{1, 3, 4};
  const Path fiveExpectedPath{1, 3, 4, 5};
  const Path sixExpectedPath{1, 3, 4, 5, 6};

  CHECK_EQ(result.shortestPathTo(1), oneExpectedPath);
  CHECK_EQ(result.shortestPathTo(2), twoExpectedPath);
  CHECK_EQ(result.shortestPathTo(3), threeExpectedPath);
  CHECK_EQ(result.shortestPathTo(4), fourExpectedPath);
  CHECK_EQ(result.shortestPathTo(5), fiveExpectedPath);
  CHECK_EQ(result.shortestPathTo(6), sixExpectedPath);
}

TEST_CASE("bellman-ford should work for Romania")
{
  const g::romania::graph_type<g::AdjacencyList> romaniaGraph{
    g::romania::createGraph<g::AdjacencyList>()};

  const g::algo::ShortestPaths result{
    g::algo::bellmanFord(romaniaGraph, g::romania::City::Arad)};

  const std::int64_t aradExpectedDistance{0};
  const std::int64_t bucharestExpectedDistance{418};
  const std::int64_t craiovaExpectedDistance{366};
  const std::int64_t dobretaExpectedDistance{374};
  const std::int64_t eforieExpectedDistance{687};
  const std::int64_t fagarasExpectedDistance{239};
  const std::int64_t giurgiuExpectedDistance{508};
  const std::int64_t hirsovaExpectedDistance{601};
  const std::int64_t iasiExpectedDistance{737};
  const std::int64_t lugojExpectedDistance{229};
  const std::int64_t mehadiaExpectedDistance{299};
  const std::int64_t neamtExpectedDistance{824};
  const std::int64_t oradeaExpectedDistance{146};
  const std::int64_t pitestiExpectedDistance{317};
  const std::int64_t rimnicuVilceaExpectedDistance{220};
  const std::int64_t sibiuExpectedDistance{140};
  const std::int64_t timisoaraExpectedDistance{118};
  const std::int64_t urziceniExpectedDistance{503};
  const std::int64_t vasluiExpectedDistance{645};
  const std::int64_t zerindExpectedDistance{75};

  using C = g::romania::City;
  REQUIRE_EQ(result.distanceTo(C::Arad), aradExpectedDistance);
  REQUIRE_EQ(result.distanceTo(C::Bucharest), bucharestExpectedDistance);
  REQUIRE_EQ(result.distanceTo(C::Craiova), craiovaExpectedDistance);
  REQUIRE_EQ(result.distanceTo(C::Dobreta), dobretaExpectedDistance);
  REQUIRE_EQ(result.distanceTo(C::Eforie), eforieExpectedDistance);
  REQUIRE_EQ(result.distanceTo(C::Fagaras), fagarasExpectedDistance);
  REQUIRE_EQ(result.distanceTo(C::Giurgiu), giurgiuExpectedDistance);
  REQUIRE_EQ(result.distanceTo(C::Hirsova), hirsovaExpectedDistance);
  REQUIRE_EQ(result.distanceTo(C::Iasi), iasiExpectedDistance);
  REQUIRE_EQ(result.distanceTo(C::Lugoj), lugojExpectedDistance);
  REQUIRE_EQ(result.distanceTo(C::Mehadia), mehadiaExpectedDistance);
  REQUIRE_EQ(result.distanceTo(C::Neamt), neamtExpectedDistance);
  REQUIRE_EQ(result.distanceTo(C::Oradea), oradeaExpectedDistance);
  REQUIRE_EQ(result.distanceTo(C::Pitesti), pitestiExpectedDistance);
  REQUIRE_EQ(
    result.distanceTo(C::RimnicuVilcea), rimnicuVilceaExpectedDistance);
  REQUIRE_EQ(result.distanceTo(C::Sibiu), sibiuExpectedDistance);
  REQUIRE_EQ(result.distanceTo(C::Timisoara), timisoaraExpectedDistance);
  REQUIRE_EQ(result.distanceTo(C::Urziceni), urziceniExpectedDistance);
  REQUIRE_EQ(result.distanceTo(C::Vaslui), vasluiExpectedDistance);
  REQUIRE_EQ(result.distanceTo(C::Zerind), zerindExpectedDistance);

  using Path = std::vector<g::VertexIdentifier>;
  const Path aradExpectedPath{C::Arad};
  const Path bucharestExpectedPath{
    C::Arad, C::Sibiu, C::RimnicuVilcea, C::Pitesti, C::Bucharest};
  const Path craiovaExpectedPath{
    C::Arad, C::Sibiu, C::RimnicuVilcea, C::Craiova};
  const Path dobretaExpectedPath{
    C::Arad, C::Timisoara, C::Lugoj, C::Mehadia, C::Dobreta};
  const Path eforieExpectedPath{
    C::Arad,
    C::Sibiu,
    C::RimnicuVilcea,
    C::Pitesti,
    C::Bucharest,
    C::Urziceni,
    C::Hirsova,
    C::Eforie};
  const Path fagarasExpectedPath{C::Arad, C::Sibiu, C::Fagaras};
  const Path giurgiuExpectedPath{
    C::Arad, C::Sibiu, C::RimnicuVilcea, C::Pitesti, C::Bucharest, C::Giurgiu};
  const Path hirsovaExpectedPath{
    C::Arad,
    C::Sibiu,
    C::RimnicuVilcea,
    C::Pitesti,
    C::Bucharest,
    C::Urziceni,
    C::Hirsova};
  const Path iasiExpectedPath{
    C::Arad,
    C::Sibiu,
    C::RimnicuVilcea,
    C::Pitesti,
    C::Bucharest,
    C::Urziceni,
    C::Vaslui,
    C::Iasi};
  const Path lugojExpectedPath{C::Arad, C::Timisoara, C::Lugoj};
  const Path mehadiaExpectedPath{C::Arad, C::Timisoara, C::Lugoj, C::Mehadia};
  const Path neamtExpectedPath{
    C::Arad,
    C::Sibiu,
    C::RimnicuVilcea,
    C::Pitesti,
    C::Bucharest,
    C::Urziceni,
    C::Vaslui,
    C::Iasi,
    C::Neamt};
  const Path oradeaExpectedPath{C::Arad, C::Zerind, C::Oradea};
  const Path pitestiExpectedPath{
    C::Arad, C::Sibiu, C::RimnicuVilcea, C::Pitesti};
  const Path rimnicuVilceaExpectedPath{C::Arad, C::Sibiu, C::RimnicuVilcea};
  const Path sibiuExpectedPath{C::Arad, C::Sibiu};
  const Path timisoaraExpectedPath{C::Arad, C::Timisoara};
  const Path urziceniExpectedPath{
    C::Arad, C::Sibiu, C::RimnicuVilcea, C::Pitesti, C::Bucharest, C::Urziceni};
  const Path vasluiExpectedPath{
    C::Arad,
    C::Sibiu,
    C::RimnicuVilcea,
    C::Pitesti,
    C::Bucharest,
    C::Urziceni,
    C::Vaslui};
  const Path zerindExpectedPath{C::Arad, C::Zerind};

  REQUIRE_EQ(result.shortestPathTo(C::Arad), aradExpectedPath);
  REQUIRE_EQ(result.shortestPathTo(C::Bucharest), bucharestExpectedPath);
  REQUIRE_EQ(result.shortestPathTo(C::Craiova), craiovaExpectedPath);
  REQUIRE_EQ(result.shortestPathTo(C::Dobreta), dobretaExpectedPath);
  REQUIRE_EQ(result.shortestPathTo(C::Eforie), eforieExpectedPath);
  REQUIRE_EQ(result.shortestPathTo(C::Fagaras), fagarasExpectedPath);
  REQUIRE_EQ(result.shortestPathTo(C::Giurgiu), giurgiuExpectedPath);
  REQUIRE_EQ(result.shortestPathTo(C::Hirsova), hirsovaExpectedPath);
  REQUIRE_EQ(result.shortestPathTo(C::Iasi), iasiExpectedPath);
  REQUIRE_EQ(result.shortestPathTo(C::Lugoj), lugojExpectedPath);
  REQUIRE_EQ(result.shortestPathTo(C::Mehadia), mehadiaExpectedPath);
  REQUIRE_EQ(result.shortestPathTo(C::Neamt), neamtExpectedPath);
  REQUIRE_EQ(result.shortestPathTo(C::Oradea), oradeaExpectedPath);
  REQUIRE_EQ(result.shortestPathTo(C::Pitesti), pitestiExpectedPath);
  REQUIRE_EQ(
    result.shortestPathTo(C::RimnicuVilcea), rimnicuVilceaExpectedPath);
  REQUIRE_EQ(result.shortestPathTo(C::Sibiu), sibiuExpectedPath);
  REQUIRE_EQ(result.shortestPathTo(C::Timisoara), timisoaraExpectedPath);
  REQUIRE_EQ(result.shortestPathTo(C::Urziceni), urziceniExpectedPath);
  REQUIRE_EQ(result.shortestPathTo(C::Vaslui), vasluiExpectedPath);
  REQUIRE_EQ(result.shortestPathTo(C::Zerind), zerindExpectedPath);
}

#define G_BF_ADD_VERTEX(graph, vertex) \
  G_ASSERT(                            \
    graph.addVertex(vertex, nullptr), "Couldn't add %zu to graph!", vertex)

#define G_BF_ADD_EDGE(graph, source, target, weight)                    \
  G_ASSERT(                                                             \
    graph.addEdge(source, target, weight),                              \
    "Couldn't add edge from %zu to %zu (weight %" PRId64 ") to graph!", \
    source,                                                             \
    target,                                                             \
    static_cast<std::int64_t>(weight))

TEST_CASE("bellman-ford should work for a graph with a negative edge")
{
  g::Graph<
    g::Directionality::Directed,
    g::AdjacencyList,
    std::nullptr_t,
    std::int64_t>
    graph{};

  for (g::VertexIdentifier i{1}; i <= 6; ++i) { G_BF_ADD_VERTEX(graph, i); }

  G_BF_ADD_EDGE(graph, 1, 3, 20);
  G_BF_ADD_EDGE(graph, 1, 2, 10);
  G_BF_ADD_EDGE(graph, 2, 5, 10);
  G_BF_ADD_EDGE(graph, 2, 4, 50);
  G_BF_ADD_EDGE(graph, 3, 5, 33);
  G_BF_ADD_EDGE(graph, 3, 4, 20);
  G_BF_ADD_EDGE(graph, 4, 5, -19);
  G_BF_ADD_EDGE(graph, 4, 6, -2);
  G_BF_ADD_EDGE(graph, 5, 6, 1);

  const g::VertexIdentifier    startVertex{1};
  const g::algo::ShortestPaths result{g::algo::bellmanFord(graph, startVertex)};
  constexpr std::int64_t       oneExpectedDist{0}, twoExpectedDist{10},
    threeExpectedDist{20}, fourExpectedDist{40}, fiveExpectedDist{20},
    sixExpectedDist{21};
  CHECK(result.distanceTo(1) == oneExpectedDist);
  CHECK(result.distanceTo(2) == twoExpectedDist);
  CHECK(result.distanceTo(3) == threeExpectedDist);
  CHECK(result.distanceTo(4) == fourExpectedDist);
  CHECK(result.distanceTo(5) == fiveExpectedDist);
  CHECK(result.distanceTo(6) == sixExpectedDist);

  using Path = std::vector<g::VertexIdentifier>;
  const Path oneExpectedPath{1}, twoExpectedPath{1, 2}, threeExpectedPath{1, 3},
    fourExpectedPath{1, 3, 4}, fiveExpectedPath{1, 2, 5},
    sixExpectedPath{1, 2, 5, 6};

  CHECK(result.shortestPathTo(1) == oneExpectedPath);
  CHECK(result.shortestPathTo(2) == twoExpectedPath);
  CHECK(result.shortestPathTo(3) == threeExpectedPath);
  CHECK(result.shortestPathTo(4) == fourExpectedPath);
  CHECK(result.shortestPathTo(5) == fiveExpectedPath);
  CHECK(result.shortestPathTo(6) == sixExpectedPath);
}

TEST_CASE("bellman-ford should detect a negative weight cycle")
{
  g::Graph<
    g::Directionality::Directed,
    g::AdjacencyList,
    std::nullptr_t,
    std::int64_t>
    graph{};

  for (g::VertexIdentifier i{1}; i <= 6; ++i) { G_BF_ADD_VERTEX(graph, i); }

  G_BF_ADD_EDGE(graph, 1, 2, 10);
  G_BF_ADD_EDGE(graph, 2, 3, 1);
  G_BF_ADD_EDGE(graph, 3, 5, 3);
  G_BF_ADD_EDGE(graph, 4, 2, 4);
  G_BF_ADD_EDGE(graph, 5, 4, -10);
  G_BF_ADD_EDGE(graph, 5, 6, 22);

  CHECK_THROWS_AS(g::algo::bellmanFord(graph, 1), g::GraphException);
}

TEST_CASE("bellman-ford should work for a graph with a positive weight cycle")
{
  g::Graph<
    g::Directionality::Directed,
    g::AdjacencyList,
    std::nullptr_t,
    std::int64_t>
    graph{};

  for (g::VertexIdentifier v{1}; v <= 6; ++v) { G_BF_ADD_VERTEX(graph, v); }

  G_BF_ADD_EDGE(graph, 1, 2, 10);
  G_BF_ADD_EDGE(graph, 2, 3, 10);
  G_BF_ADD_EDGE(graph, 3, 5, 7);
  G_BF_ADD_EDGE(graph, 4, 2, 5);
  G_BF_ADD_EDGE(graph, 5, 4, 5);
  G_BF_ADD_EDGE(graph, 5, 6, 15);

  const g::VertexIdentifier    startVertex{1};
  const g::algo::ShortestPaths result{g::algo::bellmanFord(graph, startVertex)};

  constexpr std::int64_t oneExpectedDist{0}, twoExpectedDist{10},
    threeExpectedDist{20}, fourExpectedDist{32}, fiveExpectedDist{27},
    sixExpectedDist{42};

  CHECK_EQ(result.distanceTo(1), oneExpectedDist);
  CHECK_EQ(result.distanceTo(2), twoExpectedDist);
  CHECK_EQ(result.distanceTo(3), threeExpectedDist);
  CHECK_EQ(result.distanceTo(4), fourExpectedDist);
  CHECK_EQ(result.distanceTo(5), fiveExpectedDist);
  CHECK_EQ(result.distanceTo(6), sixExpectedDist);

  using Path = std::vector<g::VertexIdentifier>;
  const Path oneExpectedPath{1}, twoExpectedPath{1, 2},
    threeExpectedPath{1, 2, 3}, fourExpectedPath{1, 2, 3, 5, 4},
    fiveExpectedPath{1, 2, 3, 5}, sixExpectedPath{1, 2, 3, 5, 6};

  CHECK_EQ(result.shortestPathTo(1), oneExpectedPath);
  CHECK_EQ(result.shortestPathTo(2), twoExpectedPath);
  CHECK_EQ(result.shortestPathTo(3), threeExpectedPath);
  CHECK_EQ(result.shortestPathTo(4), fourExpectedPath);
  CHECK_EQ(result.shortestPathTo(5), fiveExpectedPath);
  CHECK_EQ(result.shortestPathTo(6), sixExpectedPath);
}
