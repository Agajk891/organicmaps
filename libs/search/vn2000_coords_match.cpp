#include "search/vn2000_coords_match.hpp"

#include "base/string_utils.hpp"

#include "platform/vn2000_utils.hpp"

#include <sstream>

namespace search
{
namespace
{
// Digits, sign and decimal point only, so "nan" and "inf" never reach the grid code.
bool IsPlainNumber(std::string const & s)
{
  if (s.empty())
    return false;
  for (char c : s)
    if (!((c >= '0' && c <= '9') || c == '.' || c == '-'))
      return false;
  return true;
}
}  // namespace

std::optional<ms::LatLon> MatchVN2000Coords(std::string const & query)
{
  std::istringstream iss(query);
  std::string prefix;
  if (!(iss >> prefix))
    return {};
  strings::AsciiToLower(prefix);
  if (prefix != "vn2000" && prefix != "vn-2000")
    return {};

  std::string cmStr, xStr, yStr, extra;
  if (!(iss >> cmStr >> xStr >> yStr) || (iss >> extra))
    return {};

  double cm, x, y;
  if (!IsPlainNumber(cmStr) || !IsPlainNumber(xStr) || !IsPlainNumber(yStr))
    return {};
  if (!strings::to_double(cmStr, cm) || !strings::to_double(xStr, x) || !strings::to_double(yStr, y))
    return {};

  return vn2000_utils::VN2000ToLatLon(x, y, cm);
}
}  // namespace search
