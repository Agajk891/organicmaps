#include "platform/vn2000_utils.hpp"

#include "geometry/geodesy.hpp"

#include "base/math.hpp"
#include "base/string_utils.hpp"

#include <string_view>

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

// Official central meridians of the 34 provinces and cities after the merger.
struct Province
{
  char const * m_name;
  double m_centralMeridianDeg;
};

Province constexpr kProvinces[] = {
    {"An Giang", 104.75},   {"Bắc Ninh", 107.0},     {"Cà Mau", 104.5},       {"Cao Bằng", 105.75},
    {"Đắk Lắk", 108.5},     {"Điện Biên", 103.0},    {"Đồng Nai", 107.75},    {"Đồng Tháp", 105.0},
    {"Gia Lai", 108.25},    {"Hà Tĩnh", 105.5},      {"Hưng Yên", 105.5},     {"Khánh Hòa", 108.25},
    {"Lai Châu", 104.75},   {"Lạng Sơn", 107.25},    {"Lào Cai", 104.75},     {"Lâm Đồng", 107.75},
    {"Nghệ An", 104.75},    {"Ninh Bình", 105.0},    {"Phú Thọ", 104.75},     {"Quảng Ngãi", 108.0},
    {"Quảng Ninh", 107.75}, {"Quảng Trị", 106.0},    {"Sơn La", 104.0},       {"Tây Ninh", 105.75},
    {"Thái Nguyên", 106.5}, {"Thanh Hóa", 105.0},    {"Cần Thơ", 105.0},      {"Đà Nẵng", 107.75},
    {"Hà Nội", 105.0},      {"Hải Phòng", 105.75},   {"Hồ Chí Minh", 105.75}, {"Huế", 107.0},
    {"Tuyên Quang", 106.0}, {"Vĩnh Long", 105.5},
};

// Comparison key: lower case, no diacritics, no "TP"/"Thành phố" prefix. Normalize does not map "đ", so it
// is replaced first.
std::string ProvinceKey(std::string name)
{
  while (strings::ReplaceFirst(name, "đ", "d"))
  {
  }
  while (strings::ReplaceFirst(name, "Đ", "D"))
  {
  }
  std::string key = strings::MakeLowerCase(strings::Normalize(name));
  strings::Trim(key);
  for (std::string_view const prefix : {"thanh pho ", "tp. ", "tp "})
  {
    if (key.starts_with(prefix))
    {
      key.erase(0, prefix.size());
      break;
    }
  }
  return key;
}
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

std::optional<double> CentralMeridianOfProvince(std::string const & name)
{
  auto const key = ProvinceKey(name);
  for (auto const & p : kProvinces)
    if (ProvinceKey(p.m_name) == key)
      return p.m_centralMeridianDeg;
  return {};
}
}  // namespace vn2000_utils
