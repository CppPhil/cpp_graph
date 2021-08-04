#ifndef INCG_G_ROMANIA_HEURISTIC_HPP
#define INCG_G_ROMANIA_HEURISTIC_HPP
#include <cstdint>

#include "vertex_identifier.hpp"

namespace g::romania {
[[nodiscard]] std::int64_t heuristic(VertexIdentifier city);
} // namespace g::romania
#endif // INCG_G_ROMANIA_HEURISTIC_HPP
