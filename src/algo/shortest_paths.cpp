#include "algo/shortest_paths.hpp"
#include "throw.hpp"

namespace g::algo {
ShortestPaths::ShortestPaths(
  VertexIdentifier source,
  const std::unordered_map<VertexIdentifier, std::optional<VertexIdentifier>>&
                                                            prev,
  const std::unordered_map<VertexIdentifier, std::int64_t>& dist)
  : m_source{source}, m_prev{prev}, m_dist{dist}
{
}

VertexIdentifier ShortestPaths::source() const { return m_source; }

std::vector<VertexIdentifier> ShortestPaths::shortestPathTo(
  VertexIdentifier target) const
{
  std::vector<VertexIdentifier> stack{};

  std::optional<VertexIdentifier> u{target};

  const auto it{m_prev.find(*u)};

  if (it == m_prev.end()) {
    G_THROW(GraphException, "m_prev did not contain target (%zu)", target);
  }

  if (const std::optional<VertexIdentifier>& previous{it->second};
      previous.has_value() || *u == m_source) {
    while (u.has_value()) {
      stack.insert(stack.begin(), *u);

      const auto iterator{m_prev.find(*u)};

      if (iterator == m_prev.end()) {
        G_THROW(GraphException, "m_prev did not contain \"u\" (%zu)", *u);
      }

      u = iterator->second;
    }
  }

  return stack;
}

std::int64_t ShortestPaths::distanceTo(VertexIdentifier target) const
{
  const auto it{m_dist.find(target)};

  if (it == m_dist.end()) {
    G_THROW(GraphException, "m_dist did not contain target (%zu)", target);
  }

  return it->second;
}
} // namespace g::algo
