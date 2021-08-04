#include <algorithm>

#include "algo/hierholzer.hpp"

namespace g::algo::detail {
void insertInto(
  std::vector<VertexIdentifier>&       eulerCircuit,
  const std::vector<VertexIdentifier>& subCircuit)
{
  if (subCircuit.empty()) { return; }

  const VertexIdentifier firstVertexOfSubCircuit{subCircuit.front()};
  const std::vector<VertexIdentifier>::const_iterator position{std::find(
    eulerCircuit.begin(), eulerCircuit.end(), firstVertexOfSubCircuit)};
  G_ASSERT(
    position != eulerCircuit.end(),
    "Could not find %zu in the euler circuit!",
    firstVertexOfSubCircuit);

  eulerCircuit.insert(
    /* insert after position */ position + 1,
    /* don't place the start vertex of the circuit in there a second time */
    subCircuit.begin() + 1,
    subCircuit.end());
}
} // namespace g::algo::detail
