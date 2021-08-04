#include <algorithm>

#include "algo/a_star/generate_new_paths.hpp"

namespace g::algo::a_star {
std::vector<Path> generateNewPaths(
  const Path&                        oldPath,
  const std::vector<VertexWithCost>& neighbors)
{
  std::vector<Path> result(neighbors.size());
  std::transform(
    neighbors.begin(),
    neighbors.end(),
    result.begin(),
    [&oldPath](const VertexWithCost& vertexWithCost) {
      Path copy{oldPath};
      copy.append(vertexWithCost);
      return copy;
    });
  return result;
}
} // namespace g::algo::a_star
