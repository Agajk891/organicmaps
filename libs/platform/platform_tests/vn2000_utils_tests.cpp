#include "testing/testing.hpp"

#include "platform/vn2000_utils.hpp"

#include "geometry/latlon.hpp"

using namespace vn2000_utils;

namespace
{
// Expected values come from PROJ (pyproj 3.8, +proj=tmerc +lat_0=0 +lon_0=<CM> +k=0.9999 +x_0=500000
// +y_0=0 +ellps=GRS80), with X = northing and Y = easting. The tolerance 1e-6 deg is about 0.1 m.
double constexpr kTolDeg = 1e-6;
}  // namespace

UNIT_TEST(VN2000_GoldenValues_CM105)
{
  auto const hanoi = VN2000ToLatLon(2326237.1932, 588788.2265, 105.0);
  TEST(hanoi, ());
  TEST(ms::AlmostEqualAbs(*hanoi, ms::LatLon(21.0285, 105.8542), kTolDeg), (*hanoi));

  auto const hcmc = VN2000ToLatLon(1191696.7058, 685718.0053, 105.0);
  TEST(hcmc, ());
  TEST(ms::AlmostEqualAbs(*hcmc, ms::LatLon(10.7725, 106.698), kTolDeg), (*hcmc));

  auto const danang = VN2000ToLatLon(1779652.0608, 844740.2017, 105.0);
  TEST(danang, ());
  TEST(ms::AlmostEqualAbs(*danang, ms::LatLon(16.0678, 108.2208), kTolDeg), (*danang));
}

UNIT_TEST(VN2000_GoldenValues_CM108)
{
  auto const hanoi = VN2000ToLatLon(2327499.0885, 276926.0117, 108.0);
  TEST(hanoi, ());
  TEST(ms::AlmostEqualAbs(*hanoi, ms::LatLon(21.0285, 105.8542), kTolDeg), (*hanoi));

  auto const hcmc = VN2000ToLatLon(1191484.6758, 357602.3652, 108.0);
  TEST(hcmc, ());
  TEST(ms::AlmostEqualAbs(*hcmc, ms::LatLon(10.7725, 106.698), kTolDeg), (*hcmc));
}

UNIT_TEST(VN2000_OriginOfZone)
{
  // The zone origin (X = 0, Y = 500 000) lies on the equator at the central meridian.
  auto const origin = VN2000ToLatLon(0.0, 500000.0, 105.0);
  TEST(origin, ());
  TEST(ms::AlmostEqualAbs(*origin, ms::LatLon(0.0, 105.0), kTolDeg), (*origin));
}

UNIT_TEST(VN2000_InvalidInput)
{
  // Central meridian outside the plausible Vietnamese range.
  TEST(!VN2000ToLatLon(2326237.0, 588788.0, 0.0), ());
  TEST(!VN2000ToLatLon(2326237.0, 588788.0, 99.9), ());
  TEST(!VN2000ToLatLon(2326237.0, 588788.0, 112.1), ());
  // Coordinates outside the allowed ranges, including negatives and NaN.
  TEST(!VN2000ToLatLon(-1.0, 588788.0, 105.0), ());
  TEST(!VN2000ToLatLon(3000001.0, 588788.0, 105.0), ());
  TEST(!VN2000ToLatLon(2326237.0, -1.0, 105.0), ());
  TEST(!VN2000ToLatLon(2326237.0, 1000001.0, 105.0), ());
  TEST(!VN2000ToLatLon(2326237.0, 588788.0, std::nan("")), ());
  TEST(!VN2000ToLatLon(std::nan(""), 588788.0, 105.0), ());
}
