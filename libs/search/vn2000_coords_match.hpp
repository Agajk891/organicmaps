#pragma once

#include "geometry/latlon.hpp"

#include <optional>
#include <string>

namespace search
{
// VN-2000 national grid, written as "VN2000 <central meridian> <X> <Y>", e.g. "VN2000 105 2326237 588788"
// (X = northing, Y = easting, decimal point for fractions; "VN-2000" is also accepted). The prefix makes
// the query self-identifying, so this is a strong match: a hit suppresses the normal search, like UTM.
// Returns nullopt unless the input has exactly this form and the values are in range (see vn2000_utils).
std::optional<ms::LatLon> MatchVN2000Coords(std::string const & query);
}  // namespace search
