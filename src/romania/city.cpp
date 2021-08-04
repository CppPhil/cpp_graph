#include <ostream>
#include <string_view>

#include "romania/city.hpp"

namespace g::romania {
namespace {
[[nodiscard]] std::string_view toString(City city)
{
  std::string_view sv{""};

  switch (city) {
#define X(CityName) \
  case City::CityName: sv = #CityName; break;
    G_ROMANIA_CITY_X_MACRO
#undef X
  }

  if (sv == "RimnicuVilcea") { return "Rimnicu Vilcea"; }

  return sv;
}
} // anonymous namespace

std::ostream& operator<<(std::ostream& os, City city)
{
  return os << toString(city);
}
} // namespace g::romania
