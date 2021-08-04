#include <doctest.h>

#include <cstdlib>
#include <ctime>

#include <algorithm>
#include <iostream>
#include <string>
#include <utility>

#include "adjacency_list.hpp"
#include "adjacency_matrix.hpp"
#include "graph.hpp"

using VertexData = std::string;
using EdgeData   = int;

#define TMPL template<typename Implementation>

TMPL using DirectedGraph
  = g::Graph<g::Directionality::Directed, Implementation, VertexData, EdgeData>;

TMPL using UndirectedGraph = g::
  Graph<g::Directionality::Undirected, Implementation, VertexData, EdgeData>;

#define DGRAPH DirectedGraph<Implementation>
#define UDGRAPH UndirectedGraph<Implementation>

#define CALL_TEST(TestFunction)     \
  TestFunction<g::AdjacencyList>(); \
  TestFunction<g::AdjacencyMatrix>()

#define IMPL_TEST(TestName) \
  TEST_CASE("Graph" #TestName) { CALL_TEST(TestName##Test); }

#define GRAPH_TEST(TestName)  \
  TMPL void TestName##Test(); \
                              \
  IMPL_TEST(TestName)         \
                              \
  TMPL void TestName##Test()

template<typename Graph>
using VertexMap = typename Graph::vertex_map;

template<typename Graph>
using EdgeMap = typename Graph::edge_map;

#define DVERTEX_MAP VertexMap<DGRAPH>
#define DEDGE_MAP EdgeMap<DGRAPH>

struct key_equal : std::equal_to<g::Edge> {
};

using namespace std::string_literals;

GRAPH_TEST(ShouldBeAbleToCreateEmptyDirectedGraph)
{
  DGRAPH g{};
  CHECK_FALSE(g.hasVertices());
  CHECK_FALSE(g.hasEdges());
  CHECK_EQ(0, g.vertexCount());
  CHECK_EQ(0, g.edgeCount());
  CHECK_EQ(DVERTEX_MAP{}, g.m_vertexMap);
  CHECK_UNARY(g.m_edgeMap.empty());
}

GRAPH_TEST(ShouldNotBeAbleToInsertEdgeOfNonExistantVerticesIntoDirectedGraph)
{
  DGRAPH g{};
  CHECK_FALSE(g.addEdge(1, 2, 5));
  CHECK_EQ(DVERTEX_MAP{}, g.m_vertexMap);
  CHECK_UNARY(g.m_edgeMap.empty());
}

GRAPH_TEST(
  ShouldNotBeAbleToInsertEdgeWhereTheSourceDoesNotExistIntoDirectedGraph)
{
  DGRAPH g{};
  REQUIRE_UNARY(g.addVertex(2, "two"s));
  CHECK_FALSE(g.addEdge(1, 2, 456));
  CHECK_UNARY(g.m_edgeMap.empty());
  CHECK_FALSE(g.isDirectlyReachable(1, 2));
}

GRAPH_TEST(ShouldNotBeAbleToInsertEdgeWhenTheTargetDoesNotExistInADirectedGraph)
{
  DGRAPH g{};
  REQUIRE_UNARY(g.addVertex(1, "one"s));
  CHECK_FALSE(g.addEdge(1, 2, 45956));
  CHECK_UNARY(g.m_edgeMap.empty());
  CHECK_FALSE(g.isDirectlyReachable(1, 2));
}

GRAPH_TEST(ShouldBeAbleToInsertEdgeIntoDirectedGraph)
{
  DGRAPH g{};
  REQUIRE_UNARY(g.addVertex(1, "one"s));
  REQUIRE_UNARY(g.addVertex(2, "two"s));
  CHECK_UNARY(g.addEdge(1, 2, 123));

  REQUIRE_EQ(1, g.m_edgeMap.size());
  const key_equal comparator{};
  const g::Edge   edge{1, 2};
  const auto      it{g.m_edgeMap.begin()};
  const auto      first{it->first};
  const bool      isEqual{comparator(edge, first)};
  CHECK_UNARY(isEqual);
  CHECK_EQ(123, g.m_edgeMap.begin()->second);

  CHECK_UNARY(g.isDirectlyReachable(1, 2));
  CHECK_EQ(std::vector<g::VertexIdentifier>{2}, g.directlyReachables(1));
  std::optional<EdgeData> data{g.fetchEdgeData(1, 2)};
  REQUIRE_UNARY(data.has_value());
  CHECK_EQ(123, *data);
}

GRAPH_TEST(ShouldBeAbleToInsertReverseEdgeIntoDirectedGraph)
{
  DGRAPH g{};
  REQUIRE_UNARY(g.addVertex(1, "one"s));
  REQUIRE_UNARY(g.addVertex(2, "two"s));
  CHECK_UNARY(g.addEdge(1, 2, 123));
  CHECK_UNARY(g.addEdge(2, 1, 321));
  CHECK_UNARY(g.isDirectlyReachable(1, 2));
  CHECK_UNARY(g.isDirectlyReachable(2, 1));
  CHECK_EQ(std::vector<g::VertexIdentifier>{2}, g.directlyReachables(1));
  CHECK_EQ(std::vector<g::VertexIdentifier>{1}, g.directlyReachables(2));
  std::optional<EdgeData> data1{g.fetchEdgeData(1, 2)};
  std::optional<EdgeData> data2{g.fetchEdgeData(2, 1)};
  REQUIRE_UNARY(data1.has_value());
  REQUIRE_UNARY(data2.has_value());
  CHECK_EQ(123, *data1);
  CHECK_EQ(321, *data2);
}

GRAPH_TEST(ShouldNotBeAbleToAddAnEdgeThatIsAlreadyInADirectedGraph)
{
  DGRAPH g{};
  REQUIRE_UNARY(g.addVertex(1, "one"s));
  REQUIRE_UNARY(g.addVertex(2, "two"s));
  REQUIRE_UNARY(g.addEdge(1, 2, 123));
  CHECK_FALSE(g.addEdge(1, 2, 45656));

  REQUIRE_EQ(1, g.m_edgeMap.size());
  const key_equal comparator{};
  CHECK_UNARY((comparator(g::Edge{1, 2}, g.m_edgeMap.begin()->first)));
  CHECK_EQ(123, g.m_edgeMap.begin()->second);

  std::optional<EdgeData> data{g.fetchEdgeData(1, 2)};
  REQUIRE_UNARY(data.has_value());
  CHECK_EQ(123, *data);
}

GRAPH_TEST(ShouldBeAbleToAddVertexToDirectedGraph)
{
  DGRAPH g{};
  CHECK_UNARY(g.addVertex(1, "one"s));
  CHECK_UNARY(g.hasVertices());
  CHECK_EQ(1, g.vertexCount());
  const std::vector<g::VertexIdentifier> vertices{g.vertices()};
  const std::vector<g::VertexIdentifier> CHECKedVertices{1};
  CHECK_EQ(CHECKedVertices, vertices);
}

GRAPH_TEST(ShouldNotBeAbleToReplaceVertex)
{
  DGRAPH g{};
  REQUIRE_UNARY(g.addVertex(1, "one"s));
  CHECK_FALSE(g.addVertex(1, "ONE"s));
  CHECK_EQ(1, g.vertexCount());
  std::optional<VertexData> data{g.fetchVertexData(1)};
  REQUIRE_UNARY(data.has_value());
  CHECK_EQ("one"s, *data);
}

GRAPH_TEST(ShouldBeAbleToClearDirectedGraph)
{
  DGRAPH g{};
  REQUIRE_UNARY(g.addVertex(1, "one"s));
  REQUIRE_UNARY(g.addVertex(2, "two"s));
  REQUIRE_UNARY(g.addEdge(1, 2, 555));
  g.clear();

  CHECK_EQ(DVERTEX_MAP{}, g.m_vertexMap);
  CHECK_UNARY(g.m_edgeMap.empty());
  CHECK_EQ(0, g.edgeCount());
  CHECK_EQ(0, g.vertexCount());
  CHECK_FALSE(g.hasVertices());
  CHECK_FALSE(g.hasEdges());
}

GRAPH_TEST(ShouldBeAbleToFetchDirectlyReachablesFromDirectedGraph)
{
  using V = std::vector<g::VertexIdentifier>;
  DGRAPH g{};
  CHECK_EQ(V{}, g.directlyReachables(1));
  REQUIRE_UNARY(g.addVertex(1, "one"s));
  REQUIRE_UNARY(g.addVertex(2, "two"s));
  REQUIRE_UNARY(g.addEdge(1, 2, 55));
  CHECK_EQ(V{2}, g.directlyReachables(1));
  CHECK_EQ(V{}, g.directlyReachables(2));
  REQUIRE_UNARY(g.addEdge(2, 1, 75545));
  CHECK_EQ(V{2}, g.directlyReachables(1));
  CHECK_EQ(V{1}, g.directlyReachables(2));
}

GRAPH_TEST(ShouldBeAbleToGetTheEdgeCountFromDirectedGraph)
{
  DGRAPH g{};
  CHECK_EQ(0, g.edgeCount());
  REQUIRE_UNARY(g.addVertex(1, "one"s));
  REQUIRE_UNARY(g.addVertex(2, "two"s));
  REQUIRE_UNARY(g.addEdge(1, 2, 55));
  CHECK_EQ(1, g.edgeCount());
  REQUIRE_UNARY(g.addEdge(2, 1, 456564));
  CHECK_EQ(2, g.edgeCount());
}

GRAPH_TEST(ShouldBeAbleToGetEdgesFromDirectedGraph)
{
  using V = std::vector<g::Edge>;
  DGRAPH g{};
  REQUIRE_UNARY(g.edges().empty());
  REQUIRE_UNARY(g.addVertex(1, "one"s));
  REQUIRE_UNARY(g.addVertex(2, "two"s));
  REQUIRE_UNARY(g.addEdge(1, 2, 55));

  const V v{g.edges()};
  REQUIRE_EQ(1, v.size());
  const key_equal comparator{};
  const bool      isTrue{comparator(g::Edge{1, 2}, *(v.begin()))};
  CHECK_UNARY(isTrue);

  REQUIRE_UNARY(g.addEdge(2, 1, 456564));
  const V CHECKedEdges{g::Edge{1, 2}, g::Edge{2, 1}};
  const V actualEdges{g.edges()};

  std::vector<std::pair<g::VertexIdentifier, g::VertexIdentifier>> CHECKedPairs(
    CHECKedEdges.size());
  std::vector<std::pair<g::VertexIdentifier, g::VertexIdentifier>> actualPairs(
    actualEdges.size());
  auto mapper = [](const g::Edge& edge) {
    return std::make_pair(edge.source(), edge.target());
  };
  std::transform(
    CHECKedEdges.begin(), CHECKedEdges.end(), CHECKedPairs.begin(), mapper);
  std::transform(
    actualEdges.begin(), actualEdges.end(), actualPairs.begin(), mapper);
  std::sort(CHECKedPairs.begin(), CHECKedPairs.end());
  std::sort(actualPairs.begin(), actualPairs.end());
  CHECK_EQ(CHECKedPairs, actualPairs);
}

GRAPH_TEST(ShouldBeAbleToFetchEdgeDataFromDirectedGraph)
{
  DGRAPH g{};
  REQUIRE_UNARY(g.addVertex(1, "oeuao"));
  REQUIRE_UNARY(g.addVertex(2, "oeuuaoeu"));
  REQUIRE_UNARY(g.addEdge(1, 2, 12345));
  std::optional<EdgeData>       data{g.fetchEdgeData(1, 2)};
  const std::optional<EdgeData> data2{std::as_const(g).fetchEdgeData(1, 2)};
  REQUIRE_UNARY(data.has_value());
  REQUIRE_UNARY(data2.has_value());
  CHECK_EQ(12345, *data);
  CHECK_EQ(12345, *data2);
}

GRAPH_TEST(ShouldNotBeAbleToFetchDataFromNonExistantEdgeInDirectedGraph)
{
  DGRAPH                        g{};
  std::optional<EdgeData>       data{g.fetchEdgeData(1, 2)};
  const std::optional<EdgeData> data2{std::as_const(g).fetchEdgeData(1, 2)};
  CHECK_UNARY_FALSE(data.has_value());
  CHECK_UNARY_FALSE(data2.has_value());
}

GRAPH_TEST(ShouldNotBeAbleToFetchEdgeDataFromReverseEdgeInDirectedGraph)
{
  DGRAPH g{};
  REQUIRE_UNARY(g.addVertex(1, "oeuao"));
  REQUIRE_UNARY(g.addVertex(2, "oeuuaoeu"));
  REQUIRE_UNARY(g.addEdge(1, 2, 12345));
  std::optional<EdgeData>       data{g.fetchEdgeData(1, 2)};
  const std::optional<EdgeData> data2{std::as_const(g).fetchEdgeData(1, 2)};
  REQUIRE_UNARY(data.has_value());
  REQUIRE_UNARY(data2.has_value());
  CHECK_EQ(12345, *data);
  CHECK_EQ(12345, *data2);
  CHECK_UNARY_FALSE(g.fetchEdgeData(2, 1).has_value());
}

GRAPH_TEST(ShouldBeAbleToFetchReverseEdgeDataIfReverseEdgeIsInDirectedGraph)
{
  DGRAPH g{};
  REQUIRE_UNARY(g.addVertex(1, "oeuao"));
  REQUIRE_UNARY(g.addVertex(2, "oeuuaoeu"));
  REQUIRE_UNARY(g.addEdge(1, 2, 12345));
  REQUIRE_UNARY(g.addEdge(2, 1, 444));
  std::optional<EdgeData>       data{g.fetchEdgeData(1, 2)};
  const std::optional<EdgeData> data2{std::as_const(g).fetchEdgeData(1, 2)};
  REQUIRE_UNARY(data.has_value());
  REQUIRE_UNARY(data2.has_value());
  CHECK_EQ(12345, *data);
  CHECK_EQ(12345, *data2);
  std::optional<EdgeData> revData{g.fetchEdgeData(2, 1)};
  REQUIRE_UNARY(revData.has_value());
  CHECK_EQ(444, *revData);
}

GRAPH_TEST(ShouldNotGetValidEdgeIteratorFromDirectedGraphIfEdgeDoesNotExist)
{
  const DGRAPH                               g{};
  const typename DGRAPH::const_edge_iterator edgeIterator{
    g.fetchEdgeIterator(1, 2)};
  CHECK_FALSE(g.isEdgeIteratorValid(edgeIterator));
}

GRAPH_TEST(ShouldBeAbleToGetEdgeIteratorFromDirectedGraph)
{
  DGRAPH g{};
  REQUIRE_UNARY(g.addVertex(5, "five"));
  REQUIRE_UNARY(g.addVertex(6, "uoenuaoeu"));
  REQUIRE_UNARY(g.addEdge(5, 6, 56464));
  const typename DGRAPH::edge_iterator edgeIterator{g.fetchEdgeIterator(5, 6)};
  REQUIRE_UNARY(g.isEdgeIteratorValid(edgeIterator));
  const auto& [key, value] = *edgeIterator;
  CHECK_EQ(5, key.source());
  CHECK_EQ(6, key.target());
  CHECK_EQ(56464, value);
}

GRAPH_TEST(
  ShouldNotBeAbleToFetchVertexDataOfVertexThatDoesNotExistInDirectedGraph)
{
  DGRAPH                    g{};
  std::optional<VertexData> vertexData{g.fetchVertexData(1)};
  CHECK_UNARY_FALSE(vertexData.has_value());
}

GRAPH_TEST(ShouldBeAbleToGetVertexDataFromDirectedGraph)
{
  DGRAPH g{};
  REQUIRE_UNARY(g.addVertex(1, "text"));
  std::optional<VertexData>       vertexData{g.fetchVertexData(1)};
  const std::optional<VertexData> constVertexData{
    std::as_const(g).fetchVertexData(1)};
  REQUIRE_UNARY(vertexData.has_value());
  REQUIRE_UNARY(constVertexData.has_value());
  CHECK_EQ("text", *vertexData);
  CHECK_EQ("text", *constVertexData);
}

GRAPH_TEST(
  ShouldNotGetValidVertexIteratorForAVertexThatDoesNotExistInDirectedGraph)
{
  const DGRAPH                                 g{};
  const typename DGRAPH::const_vertex_iterator it{g.fetchVertexIterator(1)};
  CHECK_FALSE(g.isVertexIteratorValid(it));
}

GRAPH_TEST(ShouldBeAbleToGetValidVertexIteratorFromDirectedGraph)
{
  DGRAPH g{};
  REQUIRE_UNARY(g.addVertex(1, "MyText"s));
  const typename DGRAPH::vertex_iterator it{g.fetchVertexIterator(1)};
  REQUIRE_UNARY(g.isVertexIteratorValid(it));
  const auto& [key, value] = *it;
  CHECK_EQ(1, key);
  CHECK_EQ("MyText"s, value);
}

GRAPH_TEST(ShouldNotHaveNonExistantEdgeInDirectedGraph)
{
  DGRAPH g{};
  CHECK_FALSE(g.hasEdge(1, 2));
}

GRAPH_TEST(ShouldHaveEdgeInDirectedGraph)
{
  DGRAPH g{};
  REQUIRE_UNARY(g.addVertex(1, "oeuoe"));
  REQUIRE_UNARY(g.addVertex(2, "oeuuoeuaoe"));
  REQUIRE_UNARY(g.addEdge(2, 1, 5642));
  CHECK_UNARY(g.hasEdge(2, 1));
  CHECK_FALSE(g.hasEdge(1, 2));
}

GRAPH_TEST(ShouldHaveReverseEdgeIfItWasAddedInDirectedGraph)
{
  DGRAPH g{};
  REQUIRE_UNARY(g.addVertex(1, "oeuoe"));
  REQUIRE_UNARY(g.addVertex(2, "oeuuoeuaoe"));
  REQUIRE_UNARY(g.addEdge(2, 1, 5642));
  REQUIRE_UNARY(g.addEdge(1, 2, 485945));
  CHECK_UNARY(g.hasEdge(2, 1));
  CHECK_UNARY(g.hasEdge(1, 2));
}

GRAPH_TEST(ShouldNotHaveVerticesInEmptyDirectedGraph)
{
  DGRAPH g{};
  CHECK_FALSE(g.hasVertices());
}

GRAPH_TEST(ShouldHaveVerticesInDirectedGraph)
{
  DGRAPH g{};
  REQUIRE_UNARY(g.addVertex(1, "oeiuiaooei"));
  CHECK_UNARY(g.hasVertices());
}

GRAPH_TEST(ShouldDoNothingWhenRemovingNonExistantEdgeFromDirectedGraph)
{
  DGRAPH g{};
  CHECK_FALSE(g.removeEdge(1, 2));
}

GRAPH_TEST(ShouldBeAbleToRemoveEdgeFromDirectedGraph)
{
  DGRAPH g{};
  REQUIRE_UNARY(g.addVertex(1, "one"));
  REQUIRE_UNARY(g.addVertex(2, "two"));
  REQUIRE_UNARY(g.addEdge(1, 2, 47565456));
  CHECK_UNARY(g.isDirectlyReachable(1, 2));
  CHECK_UNARY(g.removeEdge(1, 2));
  CHECK_FALSE(g.removeEdge(1, 2));
  std::optional<EdgeData> edgeData{g.fetchEdgeData(1, 2)};
  CHECK_UNARY_FALSE(edgeData.has_value());
  CHECK_FALSE(g.isDirectlyReachable(1, 2));
  CHECK_UNARY(g.m_edgeMap.empty());
}

GRAPH_TEST(ShouldBeAbleToRemoveReverseEdgeFromDirectedGraph)
{
  DGRAPH g{};
  REQUIRE_UNARY(g.addVertex(1, "one"));
  REQUIRE_UNARY(g.addVertex(2, "two"));
  REQUIRE_UNARY(g.addEdge(1, 2, 47565456));
  REQUIRE_UNARY(g.addEdge(2, 1, 54564));

  CHECK_UNARY(g.removeEdge(1, 2));
  CHECK_UNARY(g.removeEdge(2, 1));
  CHECK_EQ(0, g.edgeCount());
  CHECK_FALSE(g.isDirectlyReachable(1, 2));
  CHECK_FALSE(g.isDirectlyReachable(2, 1));
  CHECK_UNARY(g.m_edgeMap.empty());
}

GRAPH_TEST(ShouldNotAutomaticallyRemoveReverseEdgeInDirectedGraph)
{
  DGRAPH g{};
  REQUIRE_UNARY(g.addVertex(1, "one"));
  REQUIRE_UNARY(g.addVertex(2, "two"));
  REQUIRE_UNARY(g.addEdge(1, 2, 47565456));
  REQUIRE_UNARY(g.addEdge(2, 1, 54564));

  CHECK_UNARY(g.removeEdge(1, 2));
  CHECK_EQ(1, g.edgeCount());
  CHECK_FALSE(g.isDirectlyReachable(1, 2));
  CHECK_UNARY(g.isDirectlyReachable(2, 1));

  REQUIRE_EQ(1, g.m_edgeMap.size());
  const key_equal eq{};
  CHECK_UNARY(eq(g::Edge{2, 1}, g.m_edgeMap.begin()->first));
  CHECK_EQ(54564, g.m_edgeMap.begin()->second);

  std::optional<EdgeData> deletedData{g.fetchEdgeData(1, 2)};
  std::optional<EdgeData> data{g.fetchEdgeData(2, 1)};
  CHECK_UNARY_FALSE(deletedData.has_value());
  REQUIRE_UNARY(data.has_value());
  CHECK_EQ(54564, *data);
}

GRAPH_TEST(ShouldDoNothingWhenRemovingNonExistantVertexFromDirectedGraph)
{
  DGRAPH g{};
  CHECK_FALSE(g.removeVertex(1));
}

GRAPH_TEST(ShouldRemoveVertexWithNoEdges)
{
  DGRAPH g{};
  REQUIRE_UNARY(g.addVertex(1, "one"));
  REQUIRE_UNARY(g.addVertex(2, "two"));
  REQUIRE_UNARY(g.addVertex(3, "three"));
  REQUIRE_UNARY(g.addEdge(2, 3, 545544));
  REQUIRE_UNARY(g.removeVertex(1));

  std::vector<g::VertexIdentifier> vertices{g.vertices()};
  std::sort(vertices.begin(), vertices.end());
  const std::vector<g::VertexIdentifier> CHECKed{2, 3};
  CHECK_EQ(CHECKed, vertices);

  const std::vector<g::Edge> edges{g.edges()};
  const std::vector<g::Edge> CHECKedEdges{g::Edge{2, 3}};
  REQUIRE_EQ(CHECKedEdges.size(), edges.size());
  const key_equal eq{};
  const bool      b{eq(CHECKedEdges.front(), edges.front())};
  REQUIRE_UNARY(b);

  CHECK_UNARY(g.isDirectlyReachable(2, 3));
}

GRAPH_TEST(ShouldRemoveIncidentEdgeWhenDeletingVertexFromDirectedGraph)
{
  DGRAPH g{};
  REQUIRE_UNARY(g.addVertex(1, "one"));
  REQUIRE_UNARY(g.addVertex(2, "two"));
  REQUIRE_UNARY(g.addEdge(1, 2, 456465));

  CHECK_UNARY(g.removeVertex(1));
  CHECK_EQ(1, g.vertexCount());
  CHECK_EQ(0, g.edgeCount());
  CHECK_FALSE(g.hasEdges());
  CHECK_FALSE(g.isDirectlyReachable(1, 2));
}

GRAPH_TEST(ShouldRemoveMultipleIncidentEdgesWhenRemovingVertexFromDirectedGraph)
{
  DGRAPH g{};
  REQUIRE_UNARY(g.addVertex(1, "one"));
  REQUIRE_UNARY(g.addVertex(2, "two"));
  REQUIRE_UNARY(g.addVertex(3, "three"));
  REQUIRE_UNARY(g.addVertex(4, "four"));
  REQUIRE_UNARY(g.addEdge(1, 2, 654165642));
  REQUIRE_UNARY(g.addEdge(1, 3, 4254));
  REQUIRE_UNARY(g.addEdge(3, 4, 5445));

  CHECK_UNARY(g.removeVertex(1));
  CHECK_EQ(3, g.vertexCount());
  CHECK_EQ(1, g.edgeCount());
  CHECK_FALSE(g.isDirectlyReachable(1, 2));
  CHECK_FALSE(g.isDirectlyReachable(1, 3));
  CHECK_UNARY(g.isDirectlyReachable(3, 4));

  REQUIRE_EQ(1, g.m_edgeMap.size());
  const key_equal eq{};
  CHECK_UNARY(eq(g::Edge(3, 4), g.m_edgeMap.begin()->first));
  CHECK_EQ(5445, g.m_edgeMap.begin()->second);
}

GRAPH_TEST(ShouldBeAbleToSwapDirectedGraphs)
{
  DGRAPH g1{}, // 0, 1, 2, 3, 4
    g2{};      // 5, 6, 7, 8, 9

  for (int i{0}; i < 5; ++i) {
    REQUIRE_UNARY(g1.addVertex(i, std::to_string(i)));
    REQUIRE_UNARY(g2.addVertex(i + 5, std::to_string(i + 5)));
  }

  REQUIRE_UNARY(g1.addEdge(1, 1, 11));
  REQUIRE_UNARY(g1.addEdge(1, 2, 12));

  REQUIRE_UNARY(g2.addEdge(7, 8, 78));
  REQUIRE_UNARY(g2.addEdge(9, 8, 98));

  swap(g1, g2);

  // g2 reachability test
  for (int i{0}; i <= 9; ++i) {
    for (int j{0}; j <= 9; ++j) {
      if (i == 1 && j == 1) {
        REQUIRE_UNARY(g2.m_implementation.isDirectlyReachable(i, j));
      }
      else if (i == 1 && j == 2) {
        REQUIRE_UNARY(g2.m_implementation.isDirectlyReachable(i, j));
      }
      else {
        REQUIRE_FALSE(g2.m_implementation.isDirectlyReachable(i, j));
      }
    }
  }

  // g1 reachability test
  for (int i{0}; i <= 9; ++i) {
    for (int j{0}; j <= 9; ++j) {
      if (i == 7 && j == 8) {
        REQUIRE_UNARY(g1.m_implementation.isDirectlyReachable(i, j));
      }
      else if (i == 9 && j == 8) {
        REQUIRE_UNARY(g1.m_implementation.isDirectlyReachable(i, j));
      }
      else {
        REQUIRE_FALSE(g1.m_implementation.isDirectlyReachable(i, j));
      }
    }
  }
}

GRAPH_TEST(ShouldAddReverseEdgeAutomaticallyWhenTheGraphIsUndirected)
{
  UDGRAPH g{};
  REQUIRE_UNARY(g.addVertex(1, "one"));
  REQUIRE_UNARY(g.addVertex(2, "two"));
  CHECK_UNARY(g.addEdge(1, 2, 12));

  CHECK_UNARY(g.isDirectlyReachable(1, 2));
  CHECK_UNARY(g.isDirectlyReachable(2, 1));
}

GRAPH_TEST(ShouldHaveReachabilityInBothDirectionsWhenTheGraphIsUndirected)
{
  UDGRAPH g{};
  REQUIRE_UNARY(g.addVertex(1, "one"));
  REQUIRE_UNARY(g.addVertex(2, "two"));
  CHECK_UNARY(g.addEdge(1, 2, 12));

  const std::vector<g::VertexIdentifier> CHECKedFromOne{2};
  const std::vector<g::VertexIdentifier> CHECKedFromTwo{1};

  const std::vector<g::VertexIdentifier> actualFromOne{g.directlyReachables(1)};
  const std::vector<g::VertexIdentifier> actualFromTwo{g.directlyReachables(2)};

  CHECK_EQ(CHECKedFromOne, actualFromOne);
  CHECK_EQ(CHECKedFromTwo, actualFromTwo);
}

GRAPH_TEST(
  ShouldNotIncrementTheEdgeCountTwiceWhenAddingAnEdgeToAnUndirectedGraph)
{
  UDGRAPH g{};
  CHECK_EQ(0, g.edgeCount());
  REQUIRE_UNARY(g.addVertex(1, "one"));
  REQUIRE_UNARY(g.addVertex(2, "two"));
  CHECK_UNARY(g.addEdge(1, 2, 12));
  CHECK_EQ(1, g.edgeCount());
}

GRAPH_TEST(ShouldOnlyGetOneEdgeOutOfAnUndirectedGraphIfThereIsOnlyOne)
{
  UDGRAPH                    g{};
  const std::vector<g::Edge> CHECKedEdges{g::Edge{1, 2}};
  REQUIRE_UNARY(g.addVertex(1, "one"));
  REQUIRE_UNARY(g.addVertex(2, "two"));
  REQUIRE_UNARY(g.addEdge(1, 2, 56456456));
  const std::vector<g::Edge> actualEdges{g.edges()};
  REQUIRE_EQ(CHECKedEdges.size(), actualEdges.size());
  const key_equal equalityComparator{};
  const bool b{equalityComparator(CHECKedEdges.front(), actualEdges.front())};
  CHECK_UNARY(b);
}

GRAPH_TEST(ShouldBeAbleToGetEdgeDataBothWaysWithAnUndirectedGraph)
{
  UDGRAPH g{};
  REQUIRE_UNARY(g.addVertex(1, "one"));
  REQUIRE_UNARY(g.addVertex(2, "two"));
  REQUIRE_UNARY(g.addEdge(1, 2, 56456456));
  std::optional<EdgeData> data1{g.fetchEdgeData(1, 2)};
  std::optional<EdgeData> data2{g.fetchEdgeData(2, 1)};
  REQUIRE_UNARY(data1.has_value());
  REQUIRE_UNARY(data2.has_value());
  CHECK_EQ(56456456, *data1);
  CHECK_EQ(56456456, *data2);
}

GRAPH_TEST(ShouldBeAbleToFetchEdgeIteratorBothWaysInAnUndirectedGraph)
{
  UDGRAPH g{};
  REQUIRE_UNARY(g.addVertex(1, "one"));
  REQUIRE_UNARY(g.addVertex(2, "two"));
  REQUIRE_UNARY(g.addEdge(1, 2, 56456456));

  const typename UDGRAPH::edge_iterator it1{g.fetchEdgeIterator(1, 2)},
    it2{g.fetchEdgeIterator(2, 1)};
  REQUIRE_UNARY(g.isEdgeIteratorValid(it1));
  REQUIRE_UNARY(g.isEdgeIteratorValid(it2));

  const auto& [key1, value1] = *it1;
  const auto& [key2, value2] = *it2;

  CHECK_EQ(key1.source(), 1);
  CHECK_EQ(key1.target(), 2);
  CHECK_EQ(56456456, value1);

  CHECK_EQ(key1.source(), 1);
  CHECK_EQ(key1.target(), 2);
  CHECK_EQ(56456456, value2);
}

GRAPH_TEST(ShouldNotFindANonExistantEdgeInAnUndirectedGraph)
{
  const UDGRAPH                               g{};
  const typename UDGRAPH::const_edge_iterator it{g.fetchEdgeIterator(1, 2)};
  CHECK_FALSE(g.isEdgeIteratorValid(it));
}

GRAPH_TEST(ShouldHaveEdgeBothWaysInAnUndirectedGraph)
{
  UDGRAPH g{};
  REQUIRE_UNARY(g.addVertex(1, "uno"));
  REQUIRE_UNARY(g.addVertex(2, "dos"));
  REQUIRE_UNARY(g.addEdge(1, 2, 556500));

  CHECK_UNARY(g.hasEdge(1, 2));
  CHECK_UNARY(g.hasEdge(2, 1));
  CHECK_UNARY(g.hasEdges());
}

GRAPH_TEST(ShouldBeAbleToAddCyclycEdgeIntoUndirectedGraph)
{
  UDGRAPH g{};
  REQUIRE_UNARY(g.addVertex(1, "one"));
  REQUIRE_UNARY(g.addEdge(1, 1, 11));
  std::optional<EdgeData> edgeData{g.fetchEdgeData(1, 1)};
  REQUIRE_UNARY(edgeData.has_value());
  CHECK_EQ(11, *edgeData);
}

GRAPH_TEST(
  ShouldNotBeAbleToReplaceAnEdgeAddingTheReverseEdgeInAnUndirectedGraph)
{
  UDGRAPH g{};
  REQUIRE_UNARY(g.addVertex(1, "one"));
  REQUIRE_UNARY(g.addVertex(2, "two"));
  REQUIRE_UNARY(g.addEdge(1, 2, 55));
  CHECK_UNARY_FALSE(g.addEdge(2, 1, 57));
  const std::optional<EdgeData> optEdgeData{g.fetchEdgeData(1, 2)};
  const std::optional<EdgeData> opt2EdgeData{g.fetchEdgeData(2, 1)};
  REQUIRE_UNARY(optEdgeData.has_value());
  REQUIRE_UNARY(opt2EdgeData.has_value());
  CHECK(*optEdgeData == 55);
  CHECK(*opt2EdgeData == 55);
}

GRAPH_TEST(ShouldRemoveBothEdgesWhenGraphIsUndirected)
{
  UDGRAPH g{};
  REQUIRE_UNARY(g.addVertex(1, "one"));
  REQUIRE_UNARY(g.addVertex(2, "two"));
  REQUIRE_UNARY(g.addEdge(1, 2, 55));
  CHECK_UNARY(g.removeEdge(1, 2));
  CHECK_UNARY_FALSE(g.hasEdge(1, 2));
  CHECK_UNARY_FALSE(g.hasEdge(2, 1));

  REQUIRE_UNARY(g.addEdge(1, 2, 55));
  CHECK_UNARY(g.removeEdge(2, 1));
  CHECK_UNARY_FALSE(g.hasEdge(1, 2));
  CHECK_UNARY_FALSE(g.hasEdge(2, 1));
}

GRAPH_TEST(ShouldRemoveAllIncidentEdgesWhenGraphIsUndirected)
{
  UDGRAPH g{};
  REQUIRE_UNARY(g.addVertex(1, "one"));
  REQUIRE_UNARY(g.addVertex(2, "two"));
  REQUIRE_UNARY(g.addVertex(3, "three"));
  REQUIRE_UNARY(g.addVertex(4, "four"));
  REQUIRE_UNARY(g.addEdge(1, 2, 12));
  REQUIRE_UNARY(g.addEdge(3, 1, 31));
  REQUIRE_UNARY(g.addEdge(3, 4, 34));

  CHECK_UNARY(g.removeVertex(1));
  const std::vector<g::Edge> edges{g.edges()};
  const std::vector<g::Edge> expected{g::Edge{3, 4}};
  CHECK(expected == edges);
}

GRAPH_TEST(ShouldSustianRandomizedTestOnDirectedGraph)
{
  DGRAPH g{};

  constexpr int rounds{100000};
  std::srand(std::time(nullptr));
  constexpr int maxVal{100};

  for (int i{0}; i < rounds; ++i) {
    switch (std::rand() % 4) {
    case 0: {
      const int v{std::rand() % maxVal};
      (void)g.addVertex(v, std::to_string(v));
      break;
    }
    case 1: {
      const int v1{std::rand() % maxVal};
      const int v2{std::rand() % maxVal};
      (void)g.addEdge(v1, v2, v1 + v2);
      break;
    }
    case 2: {
      const int v1{std::rand() % maxVal};
      const int v2{std::rand() % maxVal};
      (void)g.removeEdge(v1, v2);
      break;
    }
    case 3: (void)g.removeVertex(std::rand() % maxVal); break;
    }
    volatile auto v1{g.edgeCount()};
    volatile auto v2{g.vertexCount()};
    volatile auto v3{g.edges()};
    volatile auto v4{g.vertices()};
    (void)v1;
    (void)v2;
    (void)v3;
    (void)v4;
    volatile auto v5{
      g.fetchEdgeData(std::rand() % maxVal, std::rand() % maxVal)};
    volatile auto v6{g.fetchVertexData(std::rand() % maxVal)};
    (void)v5;
    (void)v6;
  }

  CHECK_UNARY(true);
}

GRAPH_TEST(ShouldSustianRandomizedTestOnUndirectedGraph)
{
  UDGRAPH g{};

  constexpr int rounds{100000};
  std::srand(std::time(nullptr));
  constexpr int maxVal{100};

  for (int i{0}; i < rounds; ++i) {
    switch (std::rand() % 4) {
    case 0: {
      const int v{std::rand() % maxVal};
      (void)g.addVertex(v, std::to_string(v));
      break;
    }
    case 1: {
      const int v1{std::rand() % maxVal};
      const int v2{std::rand() % maxVal};
      (void)g.addEdge(v1, v2, v1 + v2);
      break;
    }
    case 2: {
      const int v1{std::rand() % maxVal};
      const int v2{std::rand() % maxVal};
      (void)g.removeEdge(v1, v2);
      break;
    }
    case 3: (void)g.removeVertex(std::rand() % maxVal); break;
    }
  }

  CHECK_UNARY(true);
}
