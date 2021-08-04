#include "nm/heuristic.hpp"

namespace g::nm {
std::int64_t heuristic(VertexIdentifier vertexIdentifier)
{
  switch (vertexIdentifier) {
  case 1: return 9;
  case 2: return 7;
  case 3: return 8;
  case 4: return 1;
  case 5: return 1;
  case 6: return 0;
  }

  return INT64_MAX;
}
} // namespace g::nm
