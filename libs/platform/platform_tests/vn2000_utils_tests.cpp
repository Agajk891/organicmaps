#include "testing/testing.hpp"

#include "platform/vn2000_utils.hpp"

#include "geometry/latlon.hpp"

using namespace vn2000_utils;

namespace
{
// Expected values come from PROJ (pyproj 3.8, +proj=tmerc +lat_0=0 +lon_0=<CM> +k=0.9999 +x_0=500000
// +y_0=0 +ellps=GRS80 +towgs84=-191.90441429,-39.30318279,-111.45032835,0.00928836,-0.01975479,
// 0.00427372,0.252906278), with X = northing and Y = easting. The tolerance 1e-6 deg is about 0.1 m.
double constexpr kTolDeg = 1e-6;
}  // namespace

UNIT_TEST(VN2000_GoldenValues_CM105)
{
  auto const hanoi = VN2000ToLatLon(2326345.3117, 588591.9440, 105.0);
  TEST(hanoi, ());
  TEST(ms::AlmostEqualAbs(*hanoi, ms::LatLon(21.0285, 105.8542), kTolDeg), (*hanoi));

  auto const hcmc = VN2000ToLatLon(1191808.3163, 685521.9613, 105.0);
  TEST(hcmc, ());
  TEST(ms::AlmostEqualAbs(*hcmc, ms::LatLon(10.7725, 106.698), kTolDeg), (*hcmc));

  auto const danang = VN2000ToLatLon(1779762.4555, 844543.3095, 105.0);
  TEST(danang, ());
  TEST(ms::AlmostEqualAbs(*danang, ms::LatLon(16.0678, 108.2208), kTolDeg), (*danang));
}

UNIT_TEST(VN2000_GoldenValues_CM108)
{
  auto const hanoi = VN2000ToLatLon(2327610.9347, 276731.6952, 108.0);
  TEST(hanoi, ());
  TEST(ms::AlmostEqualAbs(*hanoi, ms::LatLon(21.0285, 105.8542), kTolDeg), (*hanoi));

  auto const hcmc = VN2000ToLatLon(1191598.1802, 357407.4573, 108.0);
  TEST(hcmc, ());
  TEST(ms::AlmostEqualAbs(*hcmc, ms::LatLon(10.7725, 106.698), kTolDeg), (*hcmc));
}

// Control points of a Hai Phong GPS network: VN-2000 X/Y on the 105°45' meridian and their published WGS84 B/L.
// Independent of PROJ and of the Helmert sign convention: the opposite rotation sign misses them by 0.2-0.7 m.
UNIT_TEST(VN2000_SurveyControlPoints)
{
  auto const dms = [](double d, double m, double s) { return d + m / 60.0 + s / 3600.0; };
  double constexpr kTol = 5e-8;  // about 5 mm

  auto const gps2m = VN2000ToLatLon(2301633.847, 600413.541, 105.75);
  TEST(gps2m, ());
  TEST(ms::AlmostEqualAbs(*gps2m, ms::LatLon(dms(20, 48, 16.89866), dms(106, 42, 59.35167)), kTol), (*gps2m));

  auto const ht06 = VN2000ToLatLon(2301418.392, 600247.215, 105.75);
  TEST(ht06, ());
  TEST(ms::AlmostEqualAbs(*ht06, ms::LatLon(dms(20, 48, 9.92556), dms(106, 42, 53.55568)), kTol), (*ht06));
}

UNIT_TEST(VN2000_OriginOfZone)
{
  // The zone's false origin (X = 0, Y = 500 000) is the equator at the central meridian on the VN-2000
  // datum; after the datum shift it lands a few hundred metres away. PROJ gives X = 111.3201, Y = 499804.3492.
  auto const origin = VN2000ToLatLon(111.3201, 499804.3492, 105.0);
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

UNIT_TEST(VN2000_ProvinceMeridian)
{
  // Values from the official table (Ministry of Natural Resources and Environment).
  TEST(CentralMeridianOfProvince("Hà Nội") == 105.0, ("Hà Nội"));
  TEST(CentralMeridianOfProvince("Đà Nẵng") == 107.75, ("Đà Nẵng"));
  TEST(CentralMeridianOfProvince("Hồ Chí Minh") == 105.75, ("Hồ Chí Minh"));
  TEST(CentralMeridianOfProvince("Ninh Bình") == 105.0, ("Ninh Bình"));
  TEST(CentralMeridianOfProvince("Đắk Lắk") == 108.5, ("Đắk Lắk"));
  TEST(CentralMeridianOfProvince("Điện Biên") == 103.0, ("Điện Biên"));

  // Case, diacritics and the TP / Thành phố prefix do not matter.
  TEST(CentralMeridianOfProvince("ha noi") == 105.0, ("ha noi"));
  TEST(CentralMeridianOfProvince("TP Hồ Chí Minh") == 105.75, ("TP Hồ Chí Minh"));
  TEST(CentralMeridianOfProvince("Thành phố Đà Nẵng") == 107.75, ("Thành phố Đà Nẵng"));
  TEST(CentralMeridianOfProvince("dak lak") == 108.5, ("dak lak"));

  // Similar names must stay apart: Hà Tĩnh is 105.5, Hà Nội is 105.0.
  TEST(CentralMeridianOfProvince("Hà Tĩnh") == 105.5, ("Hà Tĩnh"));

  TEST(!CentralMeridianOfProvince("Atlantis"), ());
  TEST(!CentralMeridianOfProvince(""), ());
}
