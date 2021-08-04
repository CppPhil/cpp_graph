#include <cinttypes>
#include <cstdio>

#include <sstream>

#include <doctest.h>

#include "adjacency_list.hpp"
#include "algo/ford_fulkerson/algorithm.hpp"

namespace {
const char data[] =
  R"(0 1 16
0 2 13
1 2 10
2 1 4
3 2 9
1 3 12
2 4 14
4 3 7
3 5 20
4 5 4)";

const int lines{10};

using graph_type = g::Graph<
  g::Directionality::Directed,
  g::AdjacencyList,
  std::nullptr_t,
  std::int32_t>;

graph_type createGraph()
{
  graph_type graph{};

  std::istringstream iss{data};

  for (int i{}; i < lines; ++i) {
    g::VertexIdentifier vertex1{}, vertex2{};
    std::int32_t        capacityVal{};

    std::string buf{};
    std::getline(iss, buf);

    std::sscanf(
      buf.data(), "%zu %zu %" PRId32, &vertex1, &vertex2, &capacityVal);

    if (!graph.hasVertex(vertex1)) {
      G_ASSERT(
        graph.addVertex(vertex1, nullptr),
        "Couldn't add %zu to graph.",
        vertex1);
    }

    if (!graph.hasVertex(vertex2)) {
      G_ASSERT(
        graph.addVertex(vertex2, nullptr),
        "Couldn't add %zu to graph.",
        vertex2);
    }

    if (!graph.hasEdge(vertex1, vertex2)) {
      G_ASSERT(
        graph.addEdge(vertex1, vertex2, capacityVal),
        "Couldn't add edge from %zu to %zu with a capacity of %" PRId32
        " to graph.",
        vertex1,
        vertex2,
        capacityVal);
    }
  }

  return graph;
}
} // anonymous namespace

TEST_CASE("Ford-Fulkerson should work")
{
  const graph_type          graph{createGraph()};
  const g::VertexIdentifier source{0};
  const g::VertexIdentifier sink{5};

  const g::algo::ford_fulkerson::Result result{
    g::algo::ford_fulkerson::algorithm(graph, source, sink)};
  constexpr std::int32_t expectedMaximumFlow{23};
  using K = g::Edge;
  const std::unordered_map<K, std::int32_t> expectedFlow{
    {K{0, 0}, 0},   {K{0, 1}, 12},  {K{0, 2}, 11},  {K{0, 3}, 0},
    {K{0, 4}, 0},   {K{0, 5}, 0},   {K{1, 0}, -12}, {K{1, 1}, 0},
    {K{1, 2}, 0},   {K{1, 3}, 12},  {K{1, 4}, 0},   {K{1, 5}, 0},
    {K{2, 0}, -11}, {K{2, 1}, 0},   {K{2, 2}, 0},   {K{2, 3}, 0},
    {K{2, 4}, 11},  {K{2, 5}, 0},   {K{3, 0}, 0},   {K{3, 1}, -12},
    {K{3, 2}, 0},   {K{3, 3}, 0},   {K{3, 4}, -7},  {K{3, 5}, 19},
    {K{4, 0}, 0},   {K{4, 1}, 0},   {K{4, 2}, -11}, {K{4, 3}, 7},
    {K{4, 4}, 0},   {K{4, 5}, 4},   {K{5, 0}, 0},   {K{5, 1}, 0},
    {K{5, 2}, 0},   {K{5, 3}, -19}, {K{5, 4}, -4},  {K{5, 5}, 0}};

  CHECK_EQ(expectedMaximumFlow, result.maxFlow());
  CHECK_EQ(expectedFlow, result.flow());
}
