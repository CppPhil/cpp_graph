#include <ostream>

#include "directionality.hpp"

namespace g {
std::ostream& operator<<(std::ostream& os, Directionality d)
{
  switch (d) {
  case Directionality::Directed: os << "Directed"; break;
  case Directionality::Undirected: os << "Undirected"; break;
  }

  return os;
}
} // namespace g
