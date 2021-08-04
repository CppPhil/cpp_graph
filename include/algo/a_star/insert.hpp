#ifndef INCG_G_ALGO_A_STAR_INSERT_HPP
#define INCG_G_ALGO_A_STAR_INSERT_HPP
#include <algorithm>
#include <deque>
#include <functional>

#include "path.hpp"

namespace g::algo::a_star {
template<typename Heuristic>
void insert(
  Heuristic         heuristic,
  std::deque<Path>* openList,
  const Path&       pathToInsert)
{
  const std::deque<Path>::const_iterator insertionPoint{std::lower_bound(
    openList->begin(),
    openList->end(),
    pathToInsert,
    [&heuristic](const Path& lhs, const Path& rhs) {
      const std::int64_t lhsTotalG{lhs.g()};
      const std::int64_t lhsH{std::invoke(heuristic, lhs.back().vertex())};

      const std::int64_t rhsTotalG{rhs.g()};
      const std::int64_t rhsH{std::invoke(heuristic, rhs.back().vertex())};

      return (lhsTotalG + lhsH) < (rhsTotalG + rhsH);
    })};
  openList->insert(insertionPoint, pathToInsert);
}
} // namespace a::algo::a_star
#endif // INCG_G_ALGO_A_STAR_INSERT_HPP
