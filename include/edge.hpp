#ifndef INCG_G_EDGE_HPP
#define INCG_G_EDGE_HPP
#include <algorithm>
#include <functional>
#include <iosfwd>
#include <tuple>

#include "vertex_identifier.hpp"

namespace g {
class Edge;
} // namespace g

namespace std {
template<>
struct hash<::g::Edge>;
} // namespace std

namespace g {
/*!
 * \brief Represents an `Edge` in a `Graph`.
 **/
class Edge {
public:
  friend ::std::hash<Edge>;

  /*!
   * \brief Prints an `Edge` to an ostream.
   * \param os The ostream to print to.
   * \param edge The `Edge` to print.
   * \return `os`.
   **/
  friend std::ostream& operator<<(std::ostream& os, const Edge& edge);

#ifdef CPP20
  /*!
   * \brief Three-way comparison operator.
   **/
  friend auto operator<=>(const Edge& lhs, const Edge& rhs) = default;
#else
  friend bool operator==(const Edge& lhs, const Edge& rhs)
  {
    return lhs.source() == rhs.source() && lhs.target() == rhs.target();
  }

  friend bool operator<(const Edge& lhs, const Edge& rhs)
  {
    return std::tie(lhs.m_source, lhs.m_target)
           < std::tie(rhs.m_source, rhs.m_target);
  }
#endif

  /*!
   * \brief Creates an `Edge`.
   * \param source The source vertex.
   * \param target The target vertex.
   **/
  Edge(VertexIdentifier source, VertexIdentifier target);

  /*!
   * \brief Read accessor for the source vertex.
   * \return The source vertex.
   **/
  [[nodiscard]] VertexIdentifier source() const;

  /*!
   * \brief Read accessor for the target vertex.
   * \return The target vertex.
   **/
  [[nodiscard]] VertexIdentifier target() const;

private:
  VertexIdentifier m_source;
  VertexIdentifier m_target;
};
} // namespace g

namespace std {
template<>
struct hash<::g::Edge> {
  size_t operator()(const ::g::Edge& edge) const noexcept
  {
    const hash<::g::VertexIdentifier> hasher{};
    size_t                            hashVal{};
    constexpr auto                    goldenRatio{0x9E3779B9};
    hashVal
      ^= hasher(edge.source()) + goldenRatio + (hashVal << 6) + (hashVal >> 2);
    hashVal
      ^= hasher(edge.target()) + goldenRatio + (hashVal << 6) + (hashVal >> 2);

    return hashVal;
  }
};
} // namespace std
#endif // INCG_G_EDGE_HPP
