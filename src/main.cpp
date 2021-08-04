#define DOCTEST_CONFIG_IMPLEMENT
#include <iostream>

#include <doctest.h>

#include "adjacency_list.hpp"
#include "adjacency_matrix.hpp"
#include "graph.hpp"

int main(int argc, char* argv[])
{
  g::Graph<
    g::Directionality::Directed,
    g::AdjacencyList,
    std::nullptr_t,
    std::nullptr_t>
    graph{};

  for (g::VertexIdentifier i{0}; i <= 4; ++i) {
    G_ASSERT(
      graph.addVertex(i, nullptr), "Could not add vertex %zu to graph.", i);
  }

  G_ASSERT(
    graph.addEdge(1, 2, nullptr),
    "Couldn't add edge from %zu to %zu to graph",
    1,
    2);
  G_ASSERT(
    graph.addEdge(1, 3, nullptr),
    "Couldn't add edge from %zu to %zu to graph",
    1,
    3);
  G_ASSERT(
    graph.addEdge(3, 1, nullptr),
    "Couldn't add edge from %zu to %zu to graph",
    3,
    3);
  G_ASSERT(
    graph.addEdge(4, 1, nullptr),
    "Couldn't add edge from %zu to %zu to graph",
    4,
    1);
  G_ASSERT(
    graph.addEdge(4, 4, nullptr),
    "Couldn't add edge from %zu to %zu to graph",
    4,
    4);

  std::cout << "graph:\n" << graph << '\n';

  doctest::Context context{};
  context.setOption("order-by", "name");

  context.applyCommandLine(argc, argv);

  const int res{context.run()};

  if (context.shouldExit()) { return res; }
}
