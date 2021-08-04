#include <algorithm>

#include "algo/dijkstra/insert.hpp"

namespace g::algo::dijkstra {
void insert(std::deque<VertexWithPriority>& queue, VertexWithPriority toInsert)
{
  const auto insertionPoint{std::lower_bound(
    queue.begin(),
    queue.end(),
    toInsert,
    [](const VertexWithPriority& lhs, const VertexWithPriority& rhs) {
      return lhs.priority() < rhs.priority();
    })};
  queue.insert(insertionPoint, toInsert);
}
} // namespace g::algo::dijkstra
