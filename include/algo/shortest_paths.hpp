#ifndef INCG_G_ALGO_SHORTEST_PATHS_HPP
#define INCG_G_ALGO_SHORTEST_PATHS_HPP
#include <cstdint>

#include <optional>
#include <unordered_map>
#include <vector>

#include "graph_exception.hpp"
#include "vertex_identifier.hpp"

namespace g::algo {
class ShortestPaths {
public:
  ShortestPaths(
    VertexIdentifier source,
    const std::unordered_map<VertexIdentifier, std::optional<VertexIdentifier>>&
                                                              prev,
    const std::unordered_map<VertexIdentifier, std::int64_t>& dist);

  [[nodiscard]] VertexIdentifier source() const;

  [[nodiscard]] std::vector<VertexIdentifier> shortestPathTo(
    VertexIdentifier target) const;

  [[nodiscard]] std::int64_t distanceTo(VertexIdentifier target) const;

private:
  VertexIdentifier m_source;
  std::unordered_map<VertexIdentifier, std::optional<VertexIdentifier>> m_prev;
  std::unordered_map<VertexIdentifier, std::int64_t>                    m_dist;
};
} // namespace g::algo
#endif // INCG_G_ALGO_SHORTEST_PATHS_HPP
