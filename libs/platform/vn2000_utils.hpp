#pragma once

#include "geometry/latlon.hpp"

#include <optional>

namespace vn2000_utils
{
/* VN-2000 national grid of Vietnam: transverse Mercator on the WGS84/GRS80 ellipsoid, scale 0.9999,
 * false easting 500 000 m, false northing 0. VN-2000 is split into 3-degree zones, each with its own
 * central meridian (e.g. 105, 106, 108 degrees). The numeric pair does not carry the zone, so the caller
 * must supply the central meridian.
 *
 * Axis order follows Vietnamese cadastral practice: X = northing (Bắc), Y = easting (Đông).
 *
 * Datum: VN-2000 is treated as identical to WGS84 (the GRS80 and WGS84 ellipsoids differ by well under
 * a millimetre). The published VN-2000 -> WGS84 parameters disagree with each other and are not
 * verified, so no datum shift is applied; the result may be off by a few metres.
 */

// Returns nullopt unless the central meridian is a plausible Vietnamese zone and X, Y are plausible
// VN-2000 values (X in [0, 3 000 000] m, Y in [0, 1 000 000] m).
std::optional<ms::LatLon> VN2000ToLatLon(double x, double y, double centralMeridianDeg);
}  // namespace vn2000_utils
