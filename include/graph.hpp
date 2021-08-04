#ifndef INCG_G_GRAPH_HPP
#define INCG_G_GRAPH_HPP
#include <cstddef>

#include <algorithm>
#include <iterator>
#include <optional>
#include <ostream>
#include <unordered_map>

#include "assert.hpp"
#include "directionality.hpp"
#include "edge.hpp"
#include "vertex_identifier.hpp"

namespace g {
/*!
 * \brief The graph data structure.
 * \tparam TheDirectionality The directionality of the edges in the graph.
 * \tparam Implementation The underlying implementation.
 * \tparam VertexData The type of the data that the vertices are associated
 *                    with.
 * \tparam EdgeData The type of the data that the edges are associated
 *                  with.
 **/
template<
  Directionality TheDirectionality,
  typename Implementation,
  typename VertexData,
  typename EdgeData>
class Graph {
public:
  using this_type           = Graph;
  using edge_map            = std::unordered_map<Edge, EdgeData>;
  using edge_iterator       = typename edge_map::iterator;
  using const_edge_iterator = typename edge_map::const_iterator;
  using vertex_map          = std::unordered_map<VertexIdentifier, VertexData>;
  using vertex_iterator     = typename vertex_map::iterator;
  using const_vertex_iterator = typename vertex_map::const_iterator;
  static constexpr inline Directionality directionality{TheDirectionality};

  /*!
   * \brief Prints a `Graph` to an ostream.
   * \param os The ostream to print to.
   * \param graph The `Graph` to print.
   * \return `os`.
   **/
  friend std::ostream& operator<<(std::ostream& os, const Graph& graph)
  {
    os << "Graph:\n";

    auto printMap{[&os](const auto& map) {
      for (const auto& pair : map) {
        const auto& key{pair.first};
        const auto& value{pair.second};
        os << key << " => " << value << '\n';
      }
    }};

    os << "vertexMap:\n";
    printMap(graph.m_vertexMap);
    os << "\nedgeMap:\n";
    printMap(graph.m_edgeMap);
    os << '\n' << graph.m_implementation;

    return os;
  }

  /*!
   * \brief Creates an empty `Graph`.
   **/
  Graph() : m_vertexMap{}, m_edgeMap{}, m_implementation{}
  {
    m_vertexMap.max_load_factor(0.7F);
    m_edgeMap.max_load_factor(0.7F);
  }

  /*!
   * \brief Adds an edge to this `Graph`.
   * \param source The source vertex.
   * \param target The target vertex.
   * \param data The data associated with the edge.
   * \return true if the edge could be inserted; otherwise false.
   **/
  [[nodiscard]] bool addEdge(
    VertexIdentifier source,
    VertexIdentifier target,
    const EdgeData&  data)
  {
    if (!hasVertex(source) || !hasVertex(target)) { return false; }

    const auto [it, wasInserted]
      = m_edgeMap.emplace(Edge{source, target}, data);
    (void)it;
    m_implementation.addTo(source, target);

    if constexpr (directionality == Directionality::Undirected) {
      if (!wasInserted) { return false; }

      // cyclic edge
      if (source == target) { return wasInserted; }

      const auto [iter, wasItInserted]
        = m_edgeMap.emplace(Edge{target, source}, data);
      m_implementation.addTo(target, source);

      return wasItInserted;
    }

    return wasInserted;
  }

  /*!
   * \brief Adds a vertex to this `Graph`.
   * \param vertex The vertex to add.
   * \param data The data associated with `vertex`.
   **/
  [[nodiscard]] bool addVertex(VertexIdentifier vertex, const VertexData& data)
  {
    const auto [it, wasInserted] = m_vertexMap.emplace(vertex, data);
    (void)it;
    return wasInserted;
  }

  /*!
   * \brief Clears this `Graph`, making it empty.
   **/
  void clear()
  {
    m_vertexMap.clear();
    m_edgeMap.clear();
    m_implementation.clear();
  }

  /*!
   * \brief Fetches all the vertices that are directly reachable from `vertex`.
   * \param vertex The vertex to get the directly reachable vertices of.
   * \return The vertices directly reachable from `vertex`.
   **/
  [[nodiscard]] std::vector<VertexIdentifier> directlyReachables(
    VertexIdentifier vertex) const
  {
    return m_implementation.directlyReachables(vertex);
  }

  /*!
   * \brief Determines the count of edges in this `Graph`.
   * \return The count of edges.
   **/
  [[nodiscard]] std::size_t edgeCount() const
  {
    if constexpr (directionality == Directionality::Directed) {
      return m_edgeMap.size();
    }

    G_ASSERT(
      m_edgeMap.size() % 2 == 0, "m_edgeMap.size() was: %z", m_edgeMap.size());

    return m_edgeMap.size() / 2;
  }

  /*!
   * \brief Returns all the edges of this `Graph`.
   * \return All edges of this `Graph`.
   **/
  [[nodiscard]] std::vector<Edge> edges() const
  {
    if constexpr (directionality == Directionality::Directed) {
      return makeEdgeVector();
    }

    std::vector<Edge> result{};
    G_ASSERT(
      (m_edgeMap.size() % 2) == 0, "m_edgeMap.size(): %zu", m_edgeMap.size());

    auto contains{[&result](const Edge& edge) {
      const auto it{std::find(result.begin(), result.end(), edge)};

      return it != result.end();
    }};

    for (const std::pair<const Edge, EdgeData>& pair : m_edgeMap) {
      const Edge edge{pair.first};

      g::VertexIdentifier lower{0xDEADC0DE};
      g::VertexIdentifier higher{0xDEADC0DE};

      if (edge.source() <= edge.target()) {
        lower  = edge.source();
        higher = edge.target();
      }
      else {
        lower  = edge.target();
        higher = edge.source();
      }

      if (const Edge normalizedEdge{lower, higher}; !contains(normalizedEdge)) {
        result.push_back(normalizedEdge);
      }
    }

    G_ASSERT(
      result.size() == m_edgeMap.size() / 2,
      "result.size(): %zu, m_edgeMap.size(): %zu",
      result.size(),
      m_edgeMap.size());

    return result;
  }

  [[nodiscard]] std::vector<Edge> edgesUndirectedWithReverseEdges() const
  {
    return makeEdgeVector();
  }

  /*!
   * \brief Fetches the data associated with the edge given.
   * \param source The source vertex.
   * \param target The target vertex.
   * \return An optional containing the `EdgeData` for the edge given, or
   *         nullopt if there is no such edge.
   **/
  [[nodiscard]] std::optional<EdgeData> fetchEdgeData(
    VertexIdentifier source,
    VertexIdentifier target) const
  {
    if (const const_edge_iterator edgeIterator{
          fetchEdgeIterator(source, target)};
        isEdgeIteratorValid(edgeIterator)) {
      return edgeIterator->second;
    }

    return std::nullopt;
  }

  /*!
   * \brief Fetches the edge iterator for the edge given.
   * \param source The source vertex.
   * \param target The target vertex.
   * \return The edge iterator for the iterator given.
   * \note The validity of the iterator can be checked using
   *       `isEdgeIteratorValid`.
   * \sa isEdgeIteratorValid
   **/
  [[nodiscard]] edge_iterator fetchEdgeIterator(
    VertexIdentifier source,
    VertexIdentifier target)
  {
    const edge_iterator it{m_edgeMap.find(Edge{source, target})};

    if constexpr (directionality == Directionality::Undirected) {
      if (!isEdgeIteratorValid(it)) {
        return m_edgeMap.find(Edge{target, source});
      }
    }

    return it;
  }

  /*!
   * \brief Fetches the edge iterator for the edge given.
   * \param source The source vertex.
   * \param target The target vertex.
   * \return The edge iterator for the iterator given.
   * \note The validity of the iterator can be checked using
   *       `isEdgeIteratorValid`.
   * \sa isEdgeIteratorValid
   **/
  [[nodiscard]] const_edge_iterator fetchEdgeIterator(
    VertexIdentifier source,
    VertexIdentifier target) const
  {
    return const_cast<this_type*>(this)->fetchEdgeIterator(source, target);
  }

  /*!
   * \brief Fetches the data associated with `vertex`.
   * \param vertex The vertex to fetch the data of.
   * \return An optional that contains the `VertexData` for `vertex`, or nullopt
   *         if there is no such vertex in the graph.
   **/
  [[nodiscard]] std::optional<VertexData> fetchVertexData(
    VertexIdentifier vertex) const
  {
    if (const const_vertex_iterator vertexIterator{fetchVertexIterator(vertex)};
        isVertexIteratorValid(vertexIterator)) {
      return vertexIterator->second;
    }

    return std::nullopt;
  }

  /*!
   * \brief Fetches the iterator for the given vertex.
   * \param vertex The vertex to get the iterator of.
   * \return The vertex iterator of `vertex`.
   * \note The validity of the iterator can be checked using
   *       `isVertexIteratorValid`.
   * \sa isVertexIteratorValid
   **/
  [[nodiscard]] vertex_iterator fetchVertexIterator(VertexIdentifier vertex)
  {
    return m_vertexMap.find(vertex);
  }

  /*!
   * \brief Fetches the iterator for the given vertex.
   * \param vertex The vertex to get the iterator of.
   * \return The vertex iterator of `vertex`.
   * \note The validity of the iterator can be checked using
   *       `isVertexIteratorValid`.
   * \sa isVertexIteratorValid
   **/
  [[nodiscard]] const_vertex_iterator fetchVertexIterator(
    VertexIdentifier vertex) const
  {
    return const_cast<this_type*>(this)->fetchVertexIterator(vertex);
  }

  /*!
   * \brief Determines whether this `Graph` has the given edge.
   * \param source The source vertex.
   * \param target The target vertex.
   * \return true if the `Graph` has the given edge; otherwise false.
   **/
  [[nodiscard]] bool hasEdge(VertexIdentifier source, VertexIdentifier target)
    const
  {
    return isEdgeIteratorValid(fetchEdgeIterator(source, target));
  }

  /*!
   * \brief Determines if this `Graph` has any edges.
   * \return true if this `Graph` has at least one edge; otherwise false.
   **/
  [[nodiscard]] bool hasEdges() const { return !m_edgeMap.empty(); }

  /*!
   * \brief Determines whether this `Graph` has the given vertex.
   * \param vertex The vertex to look for.
   * \return true if this `Graph` contains `vertex`; otherwise false.
   **/
  [[nodiscard]] bool hasVertex(VertexIdentifier vertex) const
  {
    return isVertexIteratorValid(fetchVertexIterator(vertex));
  }

  /*!
   * \brief Checks if this `Graph` has any vertices.
   * \return true if this `Graph` has at least one vertex; otherwise false.
   **/
  [[nodiscard]] bool hasVertices() const { return !m_vertexMap.empty(); }

  /*!
   * \brief Determines whether `target` is directly reachable from `source`.
   * \param source The source vertex.
   * \param target The target vertex.
   * \return true if `target` is directly reachable from `source`; otherwise
   *         false.
   **/
  [[nodiscard]] bool isDirectlyReachable(
    VertexIdentifier source,
    VertexIdentifier target) const
  {
    return m_implementation.isDirectlyReachable(source, target);
  }

  /*!
   * \brief Checks whether the given edge iterator is valid.
   * \param edgeIterator The edge iterator to check.
   * \return true if `edgeIterator` is valid; otherwise false.
   **/
  [[nodiscard]] bool isEdgeIteratorValid(const_edge_iterator edgeIterator) const
  {
    return edgeIterator != m_edgeMap.end();
  }

  /*!
   * \brief Checks whether the given vertex iterator is valid.
   * \param vertexIterator The vertex iterator to check.
   * \return true if `vertexIterator` is valid; otherwise false.
   **/
  [[nodiscard]] bool isVertexIteratorValid(
    const_vertex_iterator vertexIterator) const
  {
    return vertexIterator != m_vertexMap.end();
  }

  /*!
   * \brief Removes an edge from the graph.
   * \param source The source vertex.
   * \param target The target vertex.
   * \return true if the edge could be removed; otherwise false.
   **/
  [[nodiscard]] bool removeEdge(
    VertexIdentifier source,
    VertexIdentifier target)
  {
    const edge_iterator it{fetchEdgeIterator(source, target)};
    m_implementation.removeAdjacentFrom(source, target);

    if constexpr (directionality == Directionality::Directed) {
      if (!isEdgeIteratorValid(it)) { return false; }

      m_edgeMap.erase(it);

      return true;
    }
    else { // Undirected
      m_implementation.removeAdjacentFrom(target, source);

      if (isEdgeIteratorValid(it)) {
        m_edgeMap.erase(it);

        if (const edge_iterator reverseIt{fetchEdgeIterator(target, source)};
            isEdgeIteratorValid(reverseIt)) {
          m_edgeMap.erase(reverseIt);
        }

        return true;
      }
      else {
        const edge_iterator otherIt{fetchEdgeIterator(target, source)};

        if (!isEdgeIteratorValid(otherIt)) { return false; }

        m_edgeMap.erase(otherIt);

        return true;
      }
    }
  }

  /*!
   * \brief Removes a vertex from the graph.
   * \param vertex The vertex to remove.
   * \return true if the vertex could be remove; otherwise false.
   **/
  [[nodiscard]] bool removeVertex(VertexIdentifier vertex)
  {
    const auto it{fetchVertexIterator(vertex)};

    if (!isVertexIteratorValid(it)) { return false; }

    m_vertexMap.erase(it);
    m_implementation.removeVertex(vertex);
    mapEraseIf(
      m_edgeMap, [vertex](const std::pair<const Edge, EdgeData>& pair) {
        const Edge& key{pair.first};
        return key.source() == vertex || key.target() == vertex;
      });
    return true;
  }

  /*!
   * \brief Swaps this `Graph` with `other`.
   * \param other The other `Graph` to swap with.
   **/
  void swap(this_type& other) noexcept
  {
    m_vertexMap.swap(other.m_vertexMap);
    m_edgeMap.swap(other.m_edgeMap);
    m_implementation.swap(other.m_implementation);
  }

  /*!
   * \brief Returns the count of vertices in the graph.
   * \return The count of vertices in the graph.
   **/
  [[nodiscard]] std::size_t vertexCount() const { return m_vertexMap.size(); }

  /*!
   * \brief Returns the vertices of this `Graph`.
   * \return The vertices of this `Graph`.
   **/
  [[nodiscard]] std::vector<VertexIdentifier> vertices() const
  {
    std::vector<VertexIdentifier> resultBuffer(m_vertexMap.size());
    std::transform(
      m_vertexMap.begin(),
      m_vertexMap.end(),
      resultBuffer.begin(),
      [](const auto& pair) { return pair.first; });
    G_ASSERT(
      resultBuffer.size() == m_vertexMap.size(),
      "resultBuffer.size(): %zu, m_vertexMap: %zu",
      resultBuffer.size(),
      m_vertexMap.size());
    return resultBuffer;
  }

  /* Should be private */
public:
  /*!
   * \brief Erases all elements from an unordered_map that satisfy the
   *        predicate given.
   * \tparam Key The key type of the unordered_map.
   * \tparam Value The value type of the unordered_map.
   * \tparam UnaryPredicate The type of the predicate.
   * \param map The unordered_map to erase from.
   * \param unaryPredicate The predicate which indicates whether an element
   *                       should be erased.
   **/
  template<typename Key, typename Value, typename UnaryPredicate>
  static void mapEraseIf(
    std::unordered_map<Key, Value>& map,
    UnaryPredicate                  unaryPredicate)
  {
    const auto end{map.end()};

    for (auto it{map.begin()}; it != end;) {
      if (unaryPredicate(*it)) { map.erase(it++); }
      else {
        ++it;
      }
    }
  }

  [[nodiscard]] std::vector<Edge> makeEdgeVector() const
  {
    std::vector<Edge> result{};
    std::transform(
      m_edgeMap.begin(),
      m_edgeMap.end(),
      std::back_inserter(result),
      [](const std::pair<const Edge, EdgeData>& pair) { return pair.first; });
    G_ASSERT(
      result.size() == m_edgeMap.size(),
      "result.size(): %zu, m_edgeMap.size(): %zu",
      result.size(),
      m_edgeMap.size());
    return result;
  }

  vertex_map m_vertexMap; /*!< Maps vertices to their data */
  edge_map   m_edgeMap;   /*!< Maps edges to their data */
  Implementation
    m_implementation; /*!< Encodes the adjacency relations of the vertices */
};

/*!
 * \brief Swaps two graphs.
 * \tparam TheDirectionality The directionality of the edges in the graphs.
 * \tparam Implementation The underlying implementation type of the graphs.
 * \tparam VertexData The data that the vertices in the graphs are associated
 *                    with.
 * \tparam EdgeData The data that the edges in the graphs are associated
 *                  with.
 * \param lhs The left hand side operand.
 * \param rhs The right hand side operand.
 **/
template<
  Directionality TheDirectionality,
  typename Implementation,
  typename VertexData,
  typename EdgeData>
void swap(
  Graph<TheDirectionality, Implementation, VertexData, EdgeData>& lhs,
  Graph<TheDirectionality, Implementation, VertexData, EdgeData>& rhs) noexcept
{
  lhs.swap(rhs);
}
} // namespace g
#endif // INCG_G_GRAPH_HPP
