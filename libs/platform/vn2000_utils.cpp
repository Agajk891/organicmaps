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
  return geodesy::ENToLatLon({y, x}, tm);
}
}  // namespace vn2000_utils
