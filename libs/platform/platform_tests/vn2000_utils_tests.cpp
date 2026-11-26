#include "testing/testing.hpp"

#include "platform/vn2000_utils.hpp"

#include "geometry/latlon.hpp"

using namespace vn2000_utils;

namespace
{
// Expected values come from PROJ (pyproj 3.8, +proj=tmerc +lat_0=0 +lon_0=<CM> +k=0.9999 +x_0=500000
// +y_0=0 +ellps=GRS80 +towgs84=-191.90441429,-39.30318279,-111.45032835,-0.00928836,0.01975479,
// -0.00427372,0.252906278), with X = northing and Y = easting. The tolerance 1e-6 deg is about 0.1 m.
double constexpr kTolDeg = 1e-6;
}  // namespace

UNIT_TEST(VN2000_GoldenValues_CM105)
{
  auto const hanoi = VN2000ToLatLon(2326345.5343, 588592.6647, 105.0);
  TEST(hanoi, ());
  TEST(ms::AlmostEqualAbs(*hanoi, ms::LatLon(21.0285, 105.8542), kTolDeg), (*hanoi));

  auto const hcmc = VN2000ToLatLon(1191808.5184, 685522.4680, 105.0);
  TEST(hcmc, ());
  TEST(ms::AlmostEqualAbs(*hcmc, ms::LatLon(10.7725, 106.698), kTolDeg), (*hcmc));

  auto const danang = VN2000ToLatLon(1779762.6290, 844543.9303, 105.0);
  TEST(danang, ());
  TEST(ms::AlmostEqualAbs(*danang, ms::LatLon(16.0678, 108.2208), kTolDeg), (*danang));
}

UNIT_TEST(VN2000_GoldenValues_CM108)
{
  auto const hanoi = VN2000ToLatLon(2327611.1438, 276732.4203, 108.0);
  TEST(hanoi, ());
  TEST(ms::AlmostEqualAbs(*hanoi, ms::LatLon(21.0285, 105.8542), kTolDeg), (*hanoi));

  auto const hcmc = VN2000ToLatLon(1191598.3772, 357407.9658, 108.0);
  TEST(hcmc, ());
  TEST(ms::AlmostEqualAbs(*hcmc, ms::LatLon(10.7725, 106.698), kTolDeg), (*hcmc));
}

UNIT_TEST(VN2000_OriginOfZone)
{
  // The zone's false origin (X = 0, Y = 500 000) is the equator at the central meridian on the VN-2000
  // datum; after the datum shift it lands a few hundred metres away. PROJ gives X = 111.5587, Y = 499804.6135.
  auto const origin = VN2000ToLatLon(111.5587, 499804.6135, 105.0);
  TEST(origin, ());
  TEST(ms::AlmostEqualAbs(*origin, ms::LatLon(0.0, 105.0), kTolDeg), (*origin));
}

UNIT_TEST(VN2000_InvalidInput)
{
  // Central meridian outside the plausible Vietnamese range.
  TEST(!VN2000ToLatLon(2326237.0, 588788.0, 0.0), ());
  TEST(!VN2000ToLatLon(2326237.0, 588788.0, 99.9), ());
  TEST(!VN2000ToLatLon(2326237.0, 588788.0, 112.1), ());
  // Coordinates outside the allowed ranges, including negatives. NaN is not tested: the build uses
  // -ffast-math, so NaN comparisons are not reliable here; the matcher rejects non-numeric text instead.
  TEST(!VN2000ToLatLon(-1.0, 588788.0, 105.0), ());
  TEST(!VN2000ToLatLon(3000001.0, 588788.0, 105.0), ());
  TEST(!VN2000ToLatLon(2326237.0, -1.0, 105.0), ());
  TEST(!VN2000ToLatLon(2326237.0, 1000001.0, 105.0), ());
}
