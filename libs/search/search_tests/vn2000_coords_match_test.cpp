#include "testing/testing.hpp"

#include "search/vn2000_coords_match.hpp"

#include "geometry/latlon.hpp"

namespace vn2000_coords_match_test
{
using search::MatchVN2000Coords;

namespace
{
// Same reference as platform/vn2000_utils_tests: Hanoi, zone 105, PROJ-verified with the datum shift.
ms::LatLon const kHanoi{21.0285, 105.8542};
double constexpr kTolDeg = 1e-6;
}  // namespace

UNIT_TEST(MatchVN2000Coords_Positive)
{
  auto const m = MatchVN2000Coords("VN2000 105 2326345.5343 588592.6647");
  TEST(m, ());
  TEST(ms::AlmostEqualAbs(*m, kHanoi, kTolDeg), (*m));

  // Case-insensitive prefix, with the hyphenated spelling and extra spacing.
  TEST(MatchVN2000Coords("vn2000 105 2326345.5343 588592.6647"), ());
  TEST(MatchVN2000Coords("VN-2000   105   2326345.5343   588592.6647"), ());
  TEST(MatchVN2000Coords("  VN2000 105 2326345 588592  "), ());
}

UNIT_TEST(MatchVN2000Coords_NoCoordinate)
{
  // Without the prefix a bare number triple is not a VN-2000 request.
  TEST(!MatchVN2000Coords("105 2326237 588788"), ());
  // Missing or extra fields.
  TEST(!MatchVN2000Coords("VN2000 105 2326237"), ());
  TEST(!MatchVN2000Coords("VN2000 105 2326345 588592 1"), ());
  // Non-numeric fields.
  TEST(!MatchVN2000Coords("VN2000 105 X 588788"), ());
  // Ordinary queries that mention the prefix only.
  TEST(!MatchVN2000Coords("VN2000"), ());
  TEST(!MatchVN2000Coords("vn2000 bridge"), ());
  TEST(!MatchVN2000Coords(""), ());
  // Values out of the grid's range are rejected by vn2000_utils.
  TEST(!MatchVN2000Coords("VN2000 105 -5 588788"), ());
  TEST(!MatchVN2000Coords("VN2000 300 2326237 588788"), ());
}
}  // namespace vn2000_coords_match_test
