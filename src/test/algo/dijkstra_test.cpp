#include <doctest.h>

#include "adjacency_list.hpp"
#include "adjacency_matrix.hpp"
#include "algo/dijkstra/algorithm.hpp"
#include "algo/shortest_paths.hpp"
#include "nm/create_graph.hpp"
#include "romania/create_graph.hpp"

TEST_CASE("dijkstra should work for Romania")
{
  const g::romania::graph_type<g::AdjacencyList> romaniaGraph{
    g::romania::createGraph<g::AdjacencyList>()};

  const g::algo::ShortestPaths result{
    g::algo::dijkstra::algorithm(romaniaGraph, g::romania::City::Arad)};

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

  CHECK_EQ(result.distanceTo(g::romania::City::Arad), aradExpectedDistance);
  CHECK_EQ(
    result.distanceTo(g::romania::City::Bucharest), bucharestExpectedDistance);
  CHECK_EQ(
    result.distanceTo(g::romania::City::Craiova), craiovaExpectedDistance);
  CHECK_EQ(
    result.distanceTo(g::romania::City::Dobreta), dobretaExpectedDistance);
  CHECK_EQ(result.distanceTo(g::romania::City::Eforie), eforieExpectedDistance);
  CHECK_EQ(
    result.distanceTo(g::romania::City::Fagaras), fagarasExpectedDistance);
  CHECK_EQ(
    result.distanceTo(g::romania::City::Giurgiu), giurgiuExpectedDistance);
  CHECK_EQ(
    result.distanceTo(g::romania::City::Hirsova), hirsovaExpectedDistance);
  CHECK_EQ(result.distanceTo(g::romania::City::Iasi), iasiExpectedDistance);
  CHECK_EQ(result.distanceTo(g::romania::City::Lugoj), lugojExpectedDistance);
  CHECK_EQ(
    result.distanceTo(g::romania::City::Mehadia), mehadiaExpectedDistance);
  CHECK_EQ(result.distanceTo(g::romania::City::Neamt), neamtExpectedDistance);
  CHECK_EQ(result.distanceTo(g::romania::City::Oradea), oradeaExpectedDistance);
  CHECK_EQ(
    result.distanceTo(g::romania::City::Pitesti), pitestiExpectedDistance);
  CHECK_EQ(
    result.distanceTo(g::romania::City::RimnicuVilcea),
    rimnicuVilceaExpectedDistance);
  CHECK_EQ(result.distanceTo(g::romania::City::Sibiu), sibiuExpectedDistance);
  CHECK_EQ(
    result.distanceTo(g::romania::City::Timisoara), timisoaraExpectedDistance);
  CHECK_EQ(
    result.distanceTo(g::romania::City::Urziceni), urziceniExpectedDistance);
  CHECK_EQ(result.distanceTo(g::romania::City::Vaslui), vasluiExpectedDistance);
  CHECK_EQ(result.distanceTo(g::romania::City::Zerind), zerindExpectedDistance);

  using Path = std::vector<g::VertexIdentifier>;
  const Path aradExpectedPath{g::romania::City::Arad};
  const Path bucharestExpectedPath{
    g::romania::City::Arad,
    g::romania::City::Sibiu,
    g::romania::City::RimnicuVilcea,
    g::romania::City::Pitesti,
    g::romania::City::Bucharest};
  const Path craiovaExpectedPath{
    g::romania::City::Arad,
    g::romania::City::Sibiu,
    g::romania::City::RimnicuVilcea,
    g::romania::City::Craiova};
  const Path dobretaExpectedPath{
    g::romania::City::Arad,
    g::romania::City::Timisoara,
    g::romania::City::Lugoj,
    g::romania::City::Mehadia,
    g::romania::City::Dobreta};
  const Path eforieExpectedPath{
    g::romania::City::Arad,
    g::romania::City::Sibiu,
    g::romania::City::RimnicuVilcea,
    g::romania::City::Pitesti,
    g::romania::City::Bucharest,
    g::romania::City::Urziceni,
    g::romania::City::Hirsova,
    g::romania::City::Eforie};
  const Path fagarasExpectedPath{
    g::romania::City::Arad, g::romania::City::Sibiu, g::romania::City::Fagaras};
  const Path giurgiuExpectedPath{
    g::romania::City::Arad,
    g::romania::City::Sibiu,
    g::romania::City::RimnicuVilcea,
    g::romania::City::Pitesti,
    g::romania::City::Bucharest,
    g::romania::City::Giurgiu};
  const Path hirsovaExpectedPath{
    g::romania::City::Arad,
    g::romania::City::Sibiu,
    g::romania::City::RimnicuVilcea,
    g::romania::City::Pitesti,
    g::romania::City::Bucharest,
    g::romania::City::Urziceni,
    g::romania::City::Hirsova};
  const Path iasiExpectedPath{
    g::romania::City::Arad,
    g::romania::City::Sibiu,
    g::romania::City::RimnicuVilcea,
    g::romania::City::Pitesti,
    g::romania::City::Bucharest,
    g::romania::City::Urziceni,
    g::romania::City::Vaslui,
    g::romania::City::Iasi};
  const Path lugojExpectedPath{
    g::romania::City::Arad,
    g::romania::City::Timisoara,
    g::romania::City::Lugoj};
  const Path mehadiaExpectedPath{
    g::romania::City::Arad,
    g::romania::City::Timisoara,
    g::romania::City::Lugoj,
    g::romania::City::Mehadia};
  const Path neamtExpectedPath{
    g::romania::City::Arad,
    g::romania::City::Sibiu,
    g::romania::City::RimnicuVilcea,
    g::romania::City::Pitesti,
    g::romania::City::Bucharest,
    g::romania::City::Urziceni,
    g::romania::City::Vaslui,
    g::romania::City::Iasi,
    g::romania::City::Neamt};
  const Path oradeaExpectedPath{
    g::romania::City::Arad, g::romania::City::Zerind, g::romania::City::Oradea};
  const Path pitestiExpectedPath{
    g::romania::City::Arad,
    g::romania::City::Sibiu,
    g::romania::City::RimnicuVilcea,
    g::romania::City::Pitesti};
  const Path rimnicuVilceaExpectedPath{
    g::romania::City::Arad,
    g::romania::City::Sibiu,
    g::romania::City::RimnicuVilcea};
  const Path sibiuExpectedPath{g::romania::City::Arad, g::romania::City::Sibiu};
  const Path timisoaraExpectedPath{
    g::romania::City::Arad, g::romania::City::Timisoara};
  const Path urziceniExpectedPath{
    g::romania::City::Arad,
    g::romania::City::Sibiu,
    g::romania::City::RimnicuVilcea,
    g::romania::City::Pitesti,
    g::romania::City::Bucharest,
    g::romania::City::Urziceni};
  const Path vasluiExpectedPath{
    g::romania::City::Arad,
    g::romania::City::Sibiu,
    g::romania::City::RimnicuVilcea,
    g::romania::City::Pitesti,
    g::romania::City::Bucharest,
    g::romania::City::Urziceni,
    g::romania::City::Vaslui};
  const Path zerindExpectedPath{
    g::romania::City::Arad, g::romania::City::Zerind};

  CHECK_EQ(result.shortestPathTo(g::romania::City::Arad), aradExpectedPath);
  CHECK_EQ(
    result.shortestPathTo(g::romania::City::Bucharest), bucharestExpectedPath);
  CHECK_EQ(
    result.shortestPathTo(g::romania::City::Craiova), craiovaExpectedPath);
  CHECK_EQ(
    result.shortestPathTo(g::romania::City::Dobreta), dobretaExpectedPath);
  CHECK_EQ(result.shortestPathTo(g::romania::City::Eforie), eforieExpectedPath);
  CHECK_EQ(
    result.shortestPathTo(g::romania::City::Fagaras), fagarasExpectedPath);
  CHECK_EQ(
    result.shortestPathTo(g::romania::City::Giurgiu), giurgiuExpectedPath);
  CHECK_EQ(
    result.shortestPathTo(g::romania::City::Hirsova), hirsovaExpectedPath);
  CHECK_EQ(result.shortestPathTo(g::romania::City::Iasi), iasiExpectedPath);
  CHECK_EQ(result.shortestPathTo(g::romania::City::Lugoj), lugojExpectedPath);
  CHECK_EQ(
    result.shortestPathTo(g::romania::City::Mehadia), mehadiaExpectedPath);
  CHECK_EQ(result.shortestPathTo(g::romania::City::Neamt), neamtExpectedPath);
  CHECK_EQ(result.shortestPathTo(g::romania::City::Oradea), oradeaExpectedPath);
  CHECK_EQ(
    result.shortestPathTo(g::romania::City::Pitesti), pitestiExpectedPath);
  CHECK_EQ(
    result.shortestPathTo(g::romania::City::RimnicuVilcea),
    rimnicuVilceaExpectedPath);
  CHECK_EQ(result.shortestPathTo(g::romania::City::Sibiu), sibiuExpectedPath);
  CHECK_EQ(
    result.shortestPathTo(g::romania::City::Timisoara), timisoaraExpectedPath);
  CHECK_EQ(
    result.shortestPathTo(g::romania::City::Urziceni), urziceniExpectedPath);
  CHECK_EQ(result.shortestPathTo(g::romania::City::Vaslui), vasluiExpectedPath);
  CHECK_EQ(result.shortestPathTo(g::romania::City::Zerind), zerindExpectedPath);
}

TEST_CASE("dijkstra should work for a non monotonic graph")
{
  const g::nm::graph_type<g::AdjacencyList> graph{
    g::nm::createGraph<g::AdjacencyList>()};

  const g::VertexIdentifier startVertex{1};

  const g::algo::ShortestPaths result{
    g::algo::dijkstra::algorithm(graph, startVertex)};

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
