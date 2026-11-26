#include "platform/vn2000_utils.hpp"

#include "geometry/geodesy.hpp"

#include "base/math.hpp"

namespace vn2000_utils
{
namespace
{
double constexpr kMinCentralMeridianDeg = 100.0;
double constexpr kMaxCentralMeridianDeg = 112.0;
double constexpr kMaxX = 3000000.0;
double constexpr kMaxY = 1000000.0;

// Decision 05/2007/QĐ-BTNMT, VN-2000 -> WGS84 (position-vector convention, as in geodesy). Rotations are
// arc-seconds and the scale is in ppm.
geodesy::HelmertParams constexpr kVn2000ToWgs84{
    -191.90441429, -39.30318279, -111.45032835, geodesy::SecToRad(-0.00928836), geodesy::SecToRad(0.01975479),
    geodesy::SecToRad(-0.00427372), 0.252906278e-6};
}  // namespace

std::optional<ms::LatLon> VN2000ToLatLon(double x, double y, double centralMeridianDeg)
{
  // Written as negated comparisons so NaN is rejected too.
  if (!(centralMeridianDeg >= kMinCentralMeridianDeg && centralMeridianDeg <= kMaxCentralMeridianDeg))
    return {};
  if (!(x >= 0.0 && x <= kMaxX && y >= 0.0 && y <= kMaxY))
    return {};

  geodesy::TMParams const tm{geodesy::kWgs84, 0.9999, 0.0, math::DegToRad(centralMeridianDeg), 500000.0, 0.0};
  // The grid's northing is X and its easting is Y.
  auto const onVn2000 = geodesy::ENToLatLon({y, x}, tm);
  return geodesy::ShiftDatum(onVn2000, geodesy::kWgs84, kVn2000ToWgs84, geodesy::kWgs84);
}
}  // namespace vn2000_utils
