#pragma once

#include "geometry/latlon.hpp"

#include <optional>
#include <string>

namespace vn2000_utils
{
/* VN-2000 national grid of Vietnam: transverse Mercator on the WGS84/GRS80 ellipsoid, scale 0.9999,
 * false easting 500 000 m, false northing 0. VN-2000 is split into 3-degree zones, each with its own
 * central meridian (e.g. 105, 106, 108 degrees). The numeric pair does not carry the zone, so the caller
 * must supply the central meridian.
 *
 * Axis order follows Vietnamese cadastral practice: X = northing (Bắc), Y = easting (Đông).
 *
 * Datum: VN-2000 -> WGS84 uses the official 7-parameter Helmert set of Decision 05/2007/QĐ-BTNMT
 * (translations about -192, -39, -111 m, so the shift is far from negligible). The values were read from
 * gosco.vn, which quotes the decision.
 */

// Returns nullopt unless the central meridian is a plausible Vietnamese zone and X, Y are plausible
// VN-2000 values (X in [0, 3 000 000] m, Y in [0, 1 000 000] m).
std::optional<ms::LatLon> VN2000ToLatLon(double x, double y, double centralMeridianDeg);

// Official VN-2000 central meridian of a province or city (34 units after the merger). The name is matched
// without case, diacritics or a "TP"/"Thành phố" prefix, so "Hà Nội", "ha noi" and "TP Hà Nội" all match.
// Returns nullopt for an unknown name.
std::optional<double> CentralMeridianOfProvince(std::string const & name);
}  // namespace vn2000_utils
