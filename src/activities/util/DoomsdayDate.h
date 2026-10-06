#pragma once

#include <cstdint>

namespace doomsday {

struct Date {
  uint16_t year;
  uint8_t month;
  uint8_t day;
};

constexpr bool isLeapYear(const int year) {
  return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}

constexpr uint16_t daysInYear(const int year) { return isLeapYear(year) ? 366 : 365; }

constexpr uint8_t daysInMonth(const int year, const int month) {
  constexpr uint8_t lengths[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  return month == 2 && isLeapYear(year) ? 29 : lengths[month - 1];
}

// Sunday = 0. Gregorian calendar, matching JavaScript Date for 1600-2099.
constexpr uint8_t weekday(const Date date) {
  constexpr uint8_t offsets[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
  int year = date.year;
  if (date.month < 3) --year;
  return static_cast<uint8_t>((year + year / 4 - year / 100 + year / 400 + offsets[date.month - 1] + date.day) % 7);
}

constexpr Date dateFromDay(const uint16_t year, uint16_t dayIndex) {
  uint8_t month = 1;
  while (dayIndex >= daysInMonth(year, month)) {
    dayIndex -= daysInMonth(year, month);
    ++month;
  }
  return {year, month, static_cast<uint8_t>(dayIndex + 1)};
}

static_assert(weekday({2000, 1, 1}) == 6);
static_assert(weekday({2026, 10, 5}) == 1);
static_assert(dateFromDay(2000, 59).month == 2 && dateFromDay(2000, 59).day == 29);
static_assert(dateFromDay(1900, 59).month == 3 && dateFromDay(1900, 59).day == 1);

}  // namespace doomsday
