#include <unordered_map>

#include "romania/city.hpp"
#include "romania/heuristic.hpp"

namespace g::romania {
const std::unordered_map<City, std::int64_t> straightLineToBucharestMap(
  {{City::Arad, 366},    {City::Bucharest, 0},   {City::Craiova, 160},
   {City::Dobreta, 242}, {City::Eforie, 161},    {City::Fagaras, 176},
   {City::Giurgiu, 77},  {City::Hirsova, 151},   {City::Iasi, 226},
   {City::Lugoj, 244},   {City::Mehadia, 241},   {City::Neamt, 234},
   {City::Oradea, 380},  {City::Pitesti, 100},   {City::RimnicuVilcea, 193},
   {City::Sibiu, 253},   {City::Timisoara, 329}, {City::Urziceni, 80},
   {City::Vaslui, 199},  {City::Zerind, 374}});

std::int64_t heuristic(VertexIdentifier city)
{
  const auto it{straightLineToBucharestMap.find(static_cast<City>(city))};

  if (it == straightLineToBucharestMap.end()) { return INT64_MAX; }

  return it->second;
}
} // namespace g::romania
