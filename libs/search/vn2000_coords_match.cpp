#include "search/vn2000_coords_match.hpp"

#include "base/string_utils.hpp"

#include "platform/vn2000_utils.hpp"

#include <sstream>
#include <vector>

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

  std::vector<std::string> tokens;
  for (std::string token; iss >> token;)
    tokens.push_back(token);
  // At least "<place> <X> <Y>"; the place can be several words.
  if (tokens.size() < 3)
    return {};

  size_t const n = tokens.size();
  std::string const & xStr = tokens[n - 2];
  std::string const & yStr = tokens[n - 1];
  double x, y;
  if (!IsPlainNumber(xStr) || !IsPlainNumber(yStr) || !strings::to_double(xStr, x) || !strings::to_double(yStr, y))
    return {};

  std::string place;
  for (size_t i = 0; i + 2 < n; ++i)
  {
    if (i > 0)
      place += ' ';
    place += tokens[i];
  }

  // The place is either a numeric central meridian or a province/city name from the official table.
  double cm;
  if (IsPlainNumber(place))
  {
    if (!strings::to_double(place, cm))
      return {};
  }
  else
  {
    auto const meridian = vn2000_utils::CentralMeridianOfProvince(place);
    if (!meridian)
      return {};
    cm = *meridian;
  }

  return vn2000_utils::VN2000ToLatLon(x, y, cm);
}
}  // namespace search
