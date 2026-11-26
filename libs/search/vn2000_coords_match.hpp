#pragma once

#include "geometry/latlon.hpp"

#include <optional>
#include <string>

namespace search
{
// VN-2000 national grid, written as "VN2000 <place> <X> <Y>", e.g. "VN2000 Hà Nội 2326345 588592" or
// "VN2000 105 2326345 588592" (a numeric central meridian for zones not in the table). X = northing,
// Y = easting, decimal point for fractions; "VN-2000" is also accepted, and the place name is matched without
// case or diacritics. The prefix makes the query self-identifying, so this is a strong match: a hit
// suppresses the normal search, like UTM. Returns nullopt unless the input has this form and the values are
// in range (see vn2000_utils).
std::optional<ms::LatLon> MatchVN2000Coords(std::string const & query);
}  // namespace search
