#ifndef INCG_G_ALGO_A_STAR_ALGORITHM_HPP
#define INCG_G_ALGO_A_STAR_ALGORITHM_HPP
#include <cstdint>

#include <algorithm>
#include <deque>

#include "algo/a_star/expand.hpp"
#include "algo/a_star/generate_new_paths.hpp"
#include "algo/a_star/insert.hpp"
#include "algo/a_star/path.hpp"
#include "graph.hpp"

namespace g::algo::a_star {
template<
  Directionality TheDirectionality,
  typename Implementation,
  typename VertexData,
  typename IsGoal,
  typename Heuristic>
[[nodiscard]] Path algorithm(
  const Graph<TheDirectionality, Implementation, VertexData, std::int64_t>&
                                       graph,
  const std::vector<VertexIdentifier>& startVertices,
  IsGoal                               isGoal,
  Heuristic                            heuristic,
  bool                                 useClosedList = true)
{
  std::vector<VertexIdentifier> closedList{};
  std::deque<Path>              openList{};

  for (VertexIdentifier vertex : startVertices) {
    Path defaultPath{};
    defaultPath.append(VertexWithCost{vertex, 0});
    insert(heuristic, &openList, defaultPath);
  }

  auto contains{
    [](const std::vector<VertexIdentifier>& vec, VertexIdentifier v) {
      const std::vector<VertexIdentifier>::const_iterator it{
        std::find(vec.begin(), vec.end(), v)};
      return it != vec.end();
    }};

  while (!openList.empty()) {
    Path currentPath{openList.front()};
    openList.pop_front();

    if (!useClosedList || !contains(closedList, currentPath.back().vertex())) {
      const VertexIdentifier lastVertexOfPath{currentPath.back().vertex()};
      closedList.push_back(lastVertexOfPath);

      if (std::invoke(isGoal, lastVertexOfPath)) { return currentPath; }

      const std::vector<VertexWithCost> neighbors{
        expand(lastVertexOfPath, graph)};

      for (const std::vector<Path> newPaths{
             generateNewPaths(currentPath, neighbors)};
           const Path& path : newPaths) {
        insert(heuristic, &openList, path);
      }
    }
  }

  return Path{};
}
} // namespace g::algo::a_star
#endif // INCG_G_ALGO_A_STAR_ALGORITHM_HPP
