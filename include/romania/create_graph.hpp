#ifndef INCG_G_ROMANIA_CREATE_GRAPH_HPP
#define INCG_G_ROMANIA_CREATE_GRAPH_HPP
#include <cstddef>
#include <cstdint>

#include "assert.hpp"
#include "graph.hpp"
#include "romania/city.hpp"

namespace g::romania {
template<typename Implementation>
using graph_type = Graph<
  /* TheDirectionality */ Directionality::Undirected,
  /* Implementation */ Implementation,
  /* VertexData */ std::nullptr_t,
  /* EdgeData */ std::int64_t>;

template<typename Implementation>
[[nodiscard]] bool addVertex(
  graph_type<Implementation>* graph,
  VertexIdentifier            city)
{
  return graph->addVertex(city, nullptr);
}

template<typename Implementation>
[[nodiscard]] bool addEdge(
  graph_type<Implementation>* graph,
  VertexIdentifier            source,
  VertexIdentifier            target,
  std::int64_t                edgeLength)
{
  return graph->addEdge(source, target, edgeLength);
}

template<typename Implementation>
void addAllVertices(graph_type<Implementation>* graph)
{
#define X(CityName) \
  G_ASSERT(         \
    addVertex(graph, City::CityName), "Could not insert \"%s\"!", #CityName);
  G_ROMANIA_CITY_X_MACRO
#undef X
}

template<typename Implementation>
void addAllEdges(graph_type<Implementation>* graph)
{
#define G_ADD_EDGE(SourceCity, TargetCity, EdgeLength)              \
  G_ASSERT(                                                         \
    addEdge(graph, City::SourceCity, City::TargetCity, EdgeLength), \
    "Could not add %s -> %s with length %d",                        \
    #SourceCity,                                                    \
    #TargetCity,                                                    \
    EdgeLength)

  G_ADD_EDGE(Oradea, Zerind, 71);
  G_ADD_EDGE(Zerind, Arad, 75);
  G_ADD_EDGE(Oradea, Sibiu, 151);
  G_ADD_EDGE(Arad, Sibiu, 140);
  G_ADD_EDGE(Arad, Timisoara, 118);
  G_ADD_EDGE(Timisoara, Lugoj, 111);
  G_ADD_EDGE(Lugoj, Mehadia, 70);
  G_ADD_EDGE(Mehadia, Dobreta, 75);
  G_ADD_EDGE(Dobreta, Craiova, 120);
  G_ADD_EDGE(RimnicuVilcea, Craiova, 146);
  G_ADD_EDGE(Craiova, Pitesti, 138);
  G_ADD_EDGE(RimnicuVilcea, Sibiu, 80);
  G_ADD_EDGE(Sibiu, Fagaras, 99);
  G_ADD_EDGE(RimnicuVilcea, Pitesti, 97);
  G_ADD_EDGE(Pitesti, Bucharest, 101);
  G_ADD_EDGE(Fagaras, Bucharest, 211);
  G_ADD_EDGE(Bucharest, Giurgiu, 90);
  G_ADD_EDGE(Bucharest, Urziceni, 85);
  G_ADD_EDGE(Urziceni, Hirsova, 98);
  G_ADD_EDGE(Hirsova, Eforie, 86);
  G_ADD_EDGE(Urziceni, Vaslui, 142);
  G_ADD_EDGE(Vaslui, Iasi, 92);
  G_ADD_EDGE(Iasi, Neamt, 87);
}

template<typename Implementation>
void initializeGraph(graph_type<Implementation>* graph)
{
  addAllVertices(graph);
  addAllEdges(graph);
}

template<typename Implementation>
graph_type<Implementation> createGraph()
{
  graph_type<Implementation> graph{};
  initializeGraph(&graph);
  return graph;
}
} // namespace g::romania
#endif // INCG_G_ROMANIA_CREATE_GRAPH_HPP
