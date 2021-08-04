#ifndef INCG_G_ALGO_HIERHOLZER_HPP
#define INCG_G_ALGO_HIERHOLZER_HPP
#include <deque>

#include "graph.hpp"

namespace g::algo {
namespace detail {
template<typename Implementation, typename VertexData, typename EdgeData>
VertexIdentifier takeStep(
  VertexIdentifier               sourceVertex,
  std::vector<VertexIdentifier>& eulerCircuit,
  Graph<Directionality::Undirected, Implementation, VertexData, EdgeData>&
                                graph,
  std::deque<VertexIdentifier>& openList)
{
  const std::vector<VertexIdentifier> directlyReachables{
    graph.directlyReachables(sourceVertex)};
  G_ASSERT(
    !directlyReachables.empty(),
    "Graph had no directly reachable vertices from vertex %zu",
    sourceVertex);
  const VertexIdentifier firstDirectlyReachable{directlyReachables.front()};

  eulerCircuit.push_back(firstDirectlyReachable);

  if (directlyReachables.size() > 1) { openList.push_back(sourceVertex); }

  G_ASSERT(
    graph.removeEdge(sourceVertex, firstDirectlyReachable),
    "Could not remove edge from %zu to %zu from graph!",
    sourceVertex,
    firstDirectlyReachable);

  return firstDirectlyReachable;
}

template<typename Implementation, typename VertexData, typename EdgeData>
std::vector<VertexIdentifier> createCircuit(
  VertexIdentifier startVertex,
  Graph<Directionality::Undirected, Implementation, VertexData, EdgeData>&
                                graph,
  std::deque<VertexIdentifier>& openList)
{
  VertexIdentifier              currentVertex{startVertex};
  std::vector<VertexIdentifier> eulerCircuit{currentVertex};

  do {
    currentVertex = takeStep(currentVertex, eulerCircuit, graph, openList);
  } while (currentVertex != startVertex);

  return eulerCircuit;
}

template<typename Implementation, typename VertexData, typename EdgeData>
VertexIdentifier chooseNextVertex(
  const Graph<Directionality::Undirected, Implementation, VertexData, EdgeData>&
                                graph,
  std::deque<VertexIdentifier>& openList)
{
  const VertexIdentifier vertex{openList.front()};

  if (const std::vector<VertexIdentifier> directlyReachables{
        graph.directlyReachables(vertex)};
      directlyReachables.empty()) {
    openList.pop_front();
    return chooseNextVertex(graph, openList);
  }

  return vertex;
}

void insertInto(
  std::vector<VertexIdentifier>&       eulerCircuit,
  const std::vector<VertexIdentifier>& subCircuit);

template<typename Implementation, typename VertexData, typename EdgeData>
void createEulerTour(
  Graph<Directionality::Undirected, Implementation, VertexData, EdgeData>&
                                 graph,
  std::vector<VertexIdentifier>& eulerCircuit,
  std::deque<VertexIdentifier>&  openList)
{
  if (!graph.hasEdges()) { return; }

  const VertexIdentifier chosenVertex{chooseNextVertex(graph, openList)};
  const std::vector<VertexIdentifier> subCircuit{
    createCircuit(chosenVertex, graph, openList)};
  insertInto(eulerCircuit, subCircuit);

  createEulerTour(graph, eulerCircuit, openList);
}
} // namespace detail

template<typename Implementation, typename VertexData, typename EdgeData>
[[nodiscard]] std::vector<VertexIdentifier> hierholzer(
  Graph<Directionality::Undirected, Implementation, VertexData, EdgeData> graph)
{
  G_ASSERT(graph.hasVertices(), "%s", "The graph given had no vertices.");

  const std::vector<VertexIdentifier> vertices{graph.vertices()};
  const VertexIdentifier              startVertex{vertices.front()};
  std::deque<VertexIdentifier>        openList{};
  std::vector<VertexIdentifier>       eulerCircuit{
    detail::createCircuit(startVertex, graph, openList)};
  detail::createEulerTour(graph, eulerCircuit, openList);
  return eulerCircuit;
}
} // namespace g::algo
#endif // INCG_G_ALGO_HIERHOLZER_HPP
