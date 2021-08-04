#ifndef INCG_G_ROMANIA_CITY_HPP
#define INCG_G_ROMANIA_CITY_HPP
#include <iosfwd>

#include "vertex_identifier.hpp"

namespace g::romania {
#define G_ROMANIA_CITY_X_MACRO \
  X(Arad)                      \
  X(Bucharest)                 \
  X(Craiova)                   \
  X(Dobreta)                   \
  X(Eforie)                    \
  X(Fagaras)                   \
  X(Giurgiu)                   \
  X(Hirsova)                   \
  X(Iasi)                      \
  X(Lugoj)                     \
  X(Mehadia)                   \
  X(Neamt)                     \
  X(Oradea)                    \
  X(Pitesti)                   \
  X(RimnicuVilcea)             \
  X(Sibiu)                     \
  X(Timisoara)                 \
  X(Urziceni)                  \
  X(Vaslui)                    \
  X(Zerind)

enum City : VertexIdentifier {
#define X(CityName) CityName,
  G_ROMANIA_CITY_X_MACRO
#undef X
};

std::ostream& operator<<(std::ostream& os, City city);
} // namespace g::romania
#endif // INCG_G_ROMANIA_CITY_HPP
