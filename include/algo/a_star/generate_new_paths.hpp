#ifndef INCG_G_ALGO_A_STAR_GENERATE_NEW_PATHS_HPP
#define INCG_G_ALGO_A_STAR_GENERATE_NEW_PATHS_HPP
#include <vector>

#include "algo/a_star/path.hpp"

namespace g::algo::a_star {
std::vector<Path> generateNewPaths(
  const Path&                        oldPath,
  const std::vector<VertexWithCost>& neighbors);
} // namespace g::algo::a_star
#endif // INCG_G_ALGO_A_STAR_GENERATE_NEW_PATHS_HPP
