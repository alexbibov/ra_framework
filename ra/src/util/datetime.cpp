#include "datetime.h"

#include <chrono>
#include <cassert>
#include <algorithm>
#include <sstream>


using namespace ra::util;


namespace
{

// Helper: transforms provided day to the given time zone
void adjust_time_shift(uint16_t year, uint8_t const days_in_month[], uint8_t month, uint8_t day, uint8_t hour, int8_t time_shift_from_utc,
    uint16_t& adjusted_year, uint8_t& adjusted_month, uint8_t& adjusted_day, uint8_t& adjusted_hour)
{
    int8_t raw_hour = static_cast<int8_t>(hour) + time_shift_from_utc;
    int8_t day_shift = raw_hour / 24; raw_hour %= 24;
    if (raw_hour < 0) --day_shift;

    int8_t raw_day = static_cast<int8_t>(day) - 1 + day_shift;
    int8_t month_shift = raw_day / days_in_month[month - 1]; raw_day %= days_in_month[month - 1];
    if (raw_day < 0) --month_shift;

    int8_t raw_month = static_cast<int8_t>(month) - 1 + month_shift;
    int8_t year_shift = raw_month / 12; raw_month %= 12;
    if (raw_month < 0) --year_shift;

    adjusted_year = year + year_shift;
    adjusted_month = raw_month < 0 ? 12 : raw_month + 1;
    adjusted_day = raw_day < 0 ? days_in_month[adjusted_month - 1] : raw_day + 1;
    adjusted_hour = (raw_hour < 0 ? 24 : 0) + raw_hour;
}
}


DateTime::DateTime(int8_t time_zone /* = 0 */, bool daylight_saving_time /* = false */) :
    m_year{ 1970 },
    m_is_leap_year{ false },
    m_days_in_month{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
    m_month{ 1 },
    m_day{ 1 },
    m_hour{ 0 },
    m_minute{ 0 },
    m_second{ 0.0 },
    m_time_shift_from_utc{ daylight_saving_time + time_zone },
    m_is_dts{ daylight_saving_time }
{
    adjust_time_shift(m_year, m_days_in_month, m_month, m_day, m_hour, m_time_shift_from_utc,
        m_year, m_month, m_day, m_hour);
}

DateTime::DateTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, double second,
    int8_t time_zone /* = 0 */, bool daylight_saving_time /* = false */) :
    m_year{ year },
    m_is_leap_year{ m_year % 400 == 0 || m_year % 4 == 0 && m_year % 100 != 0 },
    m_days_in_month{ 31, static_cast<uint8_t>(28 + m_is_leap_year), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
    m_month{ month <= 12 ? month : 12U },
    m_day{ day <= m_days_in_month[month - 1] ? day : m_days_in_month[month - 1] },
    m_hour{ hour <= 23 ? hour : 23U },
    m_minute{ minute <= 59 ? minute : 59U },
    m_second{ second < 60 ? second : 59.999999999 },
    m_time_shift_from_utc{ daylight_saving_time + time_zone },
    m_is_dts{ daylight_saving_time }
{
    assert(second >= 0);

    if (time_zone || daylight_saving_time)
        adjust_time_shift(m_year, m_days_in_month, m_month, m_day, m_hour, m_time_shift_from_utc,
            m_year, m_month, m_day, m_hour);
}

DateTime::DateTime(unsigned long long nanoseconds)
{
    *this = DateTime::convertNanosecondsToDate(nanoseconds);
}

uint16_t DateTime::year() const { return m_year; }

uint8_t DateTime::month() const { return m_month; }

uint8_t DateTime::day() const { return m_day; }

uint8_t DateTime::hour() const { return m_hour; }

uint8_t DateTime::minute() const { return m_minute; }

double DateTime::second() const { return m_second; }

bool DateTime::isLeapYear() const { return m_is_leap_year; }

int8_t DateTime::getTimeZone() const { return m_time_shift_from_utc - m_is_dts; }

bool DateTime::isDTS() const { return m_is_dts; }

DateTime DateTime::getUTC() const
{
    uint16_t utc_year;
    uint8_t utc_month, utc_day, utc_hour;
    adjust_time_shift(m_year, m_days_in_month, m_month, m_day, m_hour, -m_time_shift_from_utc,
        utc_year, utc_month, utc_day, utc_hour);

    return DateTime{ utc_year, utc_month, utc_day, utc_hour, m_minute, m_second, 0, false };
}

DateTime DateTime::getLocalTime(int8_t time_zone, bool daylight_saving) const
{
    DateTime aux = getUTC();
    return DateTime{ aux.m_year, aux.m_month, aux.m_day, aux.m_hour, aux.m_minute, aux.m_second, time_zone, daylight_saving };
}

/*DateTime& DateTime::operator=(DateTime const& other)
{
if (this == &other) return *this;

m_year = other.m_year;
m_month = other.m_month;
m_day = other.m_day;
m_hour = other.m_hour;
m_minute = other.m_minute;
m_second = other.m_second;
m_is_leap_year = other.m_is_leap_year;
m_unc_time_shift = other.m_unc_time_shift;
m_is_dts = other.m_is_dts;

return *this;
}*/

DateTime DateTime::operator+(TimeSpan const& span) const
{
    int year = static_cast<int>(m_year) + span.years();


    int month = static_cast<int>(m_month) + span.months();
    auto align_months = [&month, &year]()->void
    {
        year += (month - 1) / 12U;
        month = (month - 1) % 12U + 1;
    };
    align_months();


    int day = static_cast<int>(m_day) + span.days();
    auto align_days = [&day, &month, this]()->void
    {
        while (day > m_days_in_month[month - 1]) day -= m_days_in_month[month++ - 1];
    };
    align_days();
    align_months();


    int hour = static_cast<int>(m_hour) + span.hours();
    auto align_hours = [&hour, &day]()->void
    {
        day += hour / 24U;
        hour = hour % 24U;
    };
    align_hours();
    align_days();
    align_months();


    int minute = static_cast<int>(m_minute) + span.minutes();
    auto align_minutes = [&minute, &hour]()->void
    {
        hour += minute / 60U;
        minute = minute % 60U;
    };
    align_minutes();
    align_hours();
    align_days();
    align_months();


    double second = static_cast<double>(m_second) + span.seconds();
    auto align_seconds = [&second, &minute]()->void
    {
        minute += static_cast<int>(second / 60);
        second -= static_cast<int>(second / 60) * 60;
    };
    align_seconds();
    align_minutes();
    align_hours();
    align_days();
    align_months();

    assert(year >= 0 && year <= 65535 &&
        month >= 0 && month <= 255 &&
        day >= 0 && day <= 255 &&
        hour >= 0 && hour <= 255 &&
        minute >= 0 && minute <= 255 &&
        second >= 0);
    return DateTime{ static_cast<uint16_t>(year), static_cast<uint8_t>(month), static_cast<uint8_t>(day),
        static_cast<uint8_t>(hour), static_cast<uint8_t>(minute), second };
}

DateTime DateTime::operator-(TimeSpan const& span) const
{
    return *this + (-span);
}

bool DateTime::operator>=(DateTime const& other) const
{
    if (m_year != other.m_year) return m_year > other.m_year;
    if (m_month != other.m_month) return m_month > other.m_month;
    if (m_day != other.m_day) return m_day > other.m_day;
    if (m_hour != other.m_hour) return m_hour > other.m_hour;
    if (m_minute != other.m_minute) return m_minute > other.m_minute;
    if (m_second != other.m_second) return m_second > other.m_second;
    return true;
}

bool DateTime::operator<=(DateTime const& other) const
{
    if (m_year == other.m_year && m_month == other.m_month && m_day == other.m_day &&
        m_hour == other.m_hour && m_minute == other.m_minute && m_second == other.m_second)
        return true;

    return !((*this) >= other);
}

TimeSpan DateTime::timeSince(DateTime const& other) const
{
    if (other <= *this) return TimeSpan{};

    uint8_t months_till_next_year = 12U - m_month;
    uint8_t days_till_next_month = m_days_in_month[m_month - 1] - m_day;
    uint8_t hours_till_next_day = 23 - m_hour;
    uint8_t minutes_till_next_hour = 59 - m_minute;
    double seconds_till_next_minute = std::max(59.999999999 - m_second, 0.0);

    if (m_year != other.m_year)
        return TimeSpan{ other.m_year - m_year, months_till_next_year, days_till_next_month,
        hours_till_next_day, minutes_till_next_hour, seconds_till_next_minute };

    if (m_month != other.m_month)
        return TimeSpan{ 0, other.m_month - m_month, days_till_next_month,
        hours_till_next_day, minutes_till_next_hour, seconds_till_next_minute };

    if (m_day != other.m_day)
        return TimeSpan{ 0, 0, other.m_day - m_day, hours_till_next_day, minutes_till_next_hour, seconds_till_next_minute };

    if (m_hour != other.m_hour)
        return TimeSpan{ 0, 0, 0, other.m_hour - m_hour, minutes_till_next_hour, seconds_till_next_minute };

    if (m_minute != other.m_minute)
        return TimeSpan{ 0, 0, 0, 0, other.m_minute - m_minute, seconds_till_next_minute };

    return TimeSpan{ 0, 0, 0, 0, 0, std::max(other.m_second - m_second, 0.0) };
}



namespace
{
//Retrieves number of integers in the range [s, e] with common multiplier m
inline uint32_t getNumberOfMultiples(uint32_t s, uint32_t e, uint32_t m)
{
    uint32_t rv = static_cast<uint32_t>(std::floor(static_cast<double>(e) / m) - std::ceil(static_cast<double>(s) / m)) + 1;
    return rv;
}


// Computes number of days in time period [start_year, end_year]
uint32_t getNumberOfDaysInPeriod(uint16_t start_year, uint16_t end_year)
{
    uint8_t num_leap_century_years = getNumberOfMultiples(start_year, end_year, 400);
    uint8_t num_century_years = getNumberOfMultiples(start_year, end_year, 100);
    uint16_t num_leap_years = getNumberOfMultiples(start_year, end_year, 4);

    uint32_t num_days = 365 * (end_year - start_year + 1) + num_leap_years - num_century_years + num_leap_century_years;

    return num_days;
}
}


using days = std::chrono::duration<int, std::ratio<86400, 1>>;

DateTime DateTime::now(int8_t time_zone /* = 0 */, bool daylight_saving /* = 0 */)
{
    static bool init_call = true;	// this is needed for technical optimization, since usually after the previous call it is enough to count seconds, or minutes, not months or years
    static DateTime last_date;	// date-time object obtain on the last invocation of this function
    static std::chrono::system_clock::duration last_time;	// the time of the last invocation

    std::chrono::system_clock::duration current_time = std::chrono::system_clock::now().time_since_epoch();
    auto delta_ns = current_time - last_time;

    DateTime rv;
    if (init_call || std::chrono::duration_cast<days>(delta_ns).count())
    {
        rv = convertNanosecondsToDate(static_cast<unsigned long long>(std::chrono::duration_cast<std::chrono::nanoseconds>(current_time).count()), time_zone, daylight_saving);
        init_call = false;
    }
    else
    {
        rv = DateTime{ last_date.year(), last_date.month(), last_date.day(),
            static_cast<uint8_t>(last_date.hour() + std::chrono::duration_cast<std::chrono::hours>(delta_ns).count()),
            static_cast<uint8_t>(last_date.minute() + std::chrono::duration_cast<std::chrono::minutes>(delta_ns).count()),
            last_date.second() + std::chrono::duration_cast<std::chrono::nanoseconds>(delta_ns).count() / 1e9, time_zone, daylight_saving };
    }

    last_date = rv.getUTC();
    last_time = current_time;
    return rv;
}

DateTime DateTime::convertNanosecondsToDate(unsigned long long nanoseconds, int8_t time_zone /* = 0 */, bool daylight_saving /* = 0 */)
{
    uint16_t year = 1970;
    uint8_t month = 0;
    uint8_t day = 0;
    uint8_t hour = 0;
    uint8_t minute = 0;
    double second = 0.0;

    std::chrono::nanoseconds duration_since_epoch{ static_cast<std::chrono::nanoseconds::rep>(nanoseconds) };
    uint32_t days_since_epoch = std::chrono::duration_cast<days>(duration_since_epoch).count();

    uint16_t const year1970_year2001_days = 11323U;	// hard-coded number of days between January 1, 1970, 00:00:00 and January 1, 2001, 00:00:00
    uint16_t const year1970_year1973_days = 1096;	// hard-coded number of days between January 1, 1970, 00:00:00 and January 1, 1973, 00:00:00
    uint32_t const num_days_in_400year_period = 146097U;	//number of days in 400-year period between January 1, 1, 00:00:00 and January 1, 400, 00:00:00
    uint32_t const num_days_in_100year_period = 36524U;	//number of days in 100-year period between January 1, 1, 00:00:00 and January 1, 100, 00:00:00
    uint32_t const num_days_in_4year_period = 1461U;	//number of days in 4-year period between January 1, 1, 00:00:00 and January 1, 4, 00:00:00
    int days_since_jan1;

    if (days_since_epoch <= year1970_year1973_days)
    {
        uint8_t years_since_epoch = days_since_epoch / 365;
        if (years_since_epoch == 3 && days_since_epoch == 1095) years_since_epoch = 2;	// account for the leap in 1972

        days_since_jan1 = years_since_epoch <= 2 ? days_since_epoch - years_since_epoch * 365 : 0;
        year += years_since_epoch;
    }
    else if (days_since_epoch <= year1970_year2001_days)
    {
        uint8_t years_since_epoch = 3;
        uint32_t days_since_1973 = days_since_epoch - year1970_year1973_days;

        uint16_t num_full_4year_periods = days_since_1973 / num_days_in_4year_period;
        years_since_epoch += num_full_4year_periods * 4;
        uint8_t years_since_last_leap = (days_since_1973 - num_full_4year_periods * num_days_in_4year_period) / 365;
        if (years_since_last_leap == 4) years_since_last_leap = 3;	// this is only possible when the current date is a leap day
        years_since_epoch += years_since_last_leap;

        days_since_jan1 = days_since_1973 - num_full_4year_periods * num_days_in_4year_period - years_since_last_leap * 365;
        year += years_since_epoch;
    }
    else
    {
        // here we start counting from January 1, 2001, 00:00:00
        uint8_t years_since_epoch = 31;
        uint32_t days_since_2001 = days_since_epoch - year1970_year2001_days;


        uint16_t num_full_400_periods = days_since_2001 / num_days_in_400year_period;
        uint32_t days_since_last_400year_period = days_since_2001 - num_full_400_periods * num_days_in_400year_period;
        years_since_epoch += num_full_400_periods * 400;

        uint16_t num_full_100_periods = days_since_last_400year_period / num_days_in_100year_period;
        uint32_t days_since_last_100year_period = days_since_last_400year_period - num_full_100_periods * num_days_in_100year_period;
        years_since_epoch += num_full_100_periods * 100;

        uint16_t num_full_4_year_periods = days_since_last_100year_period / num_days_in_4year_period;
        uint32_t days_since_last_4year_period = days_since_last_100year_period - num_full_4_year_periods * num_days_in_4year_period;
        years_since_epoch += num_full_4_year_periods * 4;

        uint8_t years_since_last_leap = days_since_last_4year_period / 365;
        if (years_since_last_leap == 4) years_since_last_leap = 3;	// this is only possible if the current date is a leap day
        years_since_epoch += years_since_last_leap;

        days_since_jan1 = days_since_last_4year_period - years_since_last_leap * 365;
        year += years_since_epoch;
    }


    bool is_leap_year = year % 400 == 0 || year % 4 == 0 && year % 100 != 0;
    uint8_t days_in_month[] = { 31, static_cast<uint8_t>(28 + is_leap_year), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    for (uint32_t i = 0; i <= 11; ++i)
    {
        ++month;
        day = days_since_jan1 + 1;

        days_since_jan1 -= days_in_month[i];
        if (i == 1 && is_leap_year) --days_since_jan1;

        if (days_since_jan1 < 0) break;
    }


    auto check1 = std::chrono::duration_cast<std::chrono::hours>(duration_since_epoch).count();
    auto check2 = std::chrono::duration_cast<days>(duration_since_epoch).count();
    hour = std::chrono::duration_cast<std::chrono::hours>(duration_since_epoch).count()
        - std::chrono::duration_cast<days>(duration_since_epoch).count() * 24;

    minute = std::chrono::duration_cast<std::chrono::minutes>(duration_since_epoch).count()
        - std::chrono::duration_cast<std::chrono::hours>(duration_since_epoch).count() * 60;

    unsigned long long const minutes_to_nanoseconds = 60000000000ULL;
    second = (std::chrono::duration_cast<std::chrono::nanoseconds>(duration_since_epoch).count()
        - std::chrono::duration_cast<std::chrono::minutes>(duration_since_epoch).count() * minutes_to_nanoseconds) / 1e9;

    return DateTime{ year, month, day, hour, minute, second, time_zone, daylight_saving };
}

std::string DateTime::toString(unsigned char mask, DateOutputStyle style) const
{
    std::string month_name[] = { "January", "February",
        "March", "April", "May",
        "June", "July", "August",
        "September", "October", "November",
        "December" };

    std::ostringstream output_string_stream{};
    bool preceeding_date_token_exists = false;

    switch (style)
    {
    case DateOutputStyle::european:
        if (mask & DateOutputMask::day)
        {
            output_string_stream << std::to_string(m_day);
            preceeding_date_token_exists = true;
        }

        if (mask & DateOutputMask::month)
        {
            if (preceeding_date_token_exists) output_string_stream << "/";

            output_string_stream << month_name[m_month - 1];
            preceeding_date_token_exists = true;
        }

        break;

    case DateOutputStyle::american:
        if (mask & DateOutputMask::month)
        {
            output_string_stream << month_name[m_month - 1];
            preceeding_date_token_exists = true;
        }

        if (mask & DateOutputMask::day)
        {
            if (preceeding_date_token_exists) output_string_stream << "/";

            output_string_stream << std::to_string(m_day);
            preceeding_date_token_exists = true;
        }

        break;
    }

    if (mask & DateOutputMask::year)
    {
        if (preceeding_date_token_exists) output_string_stream << "/";
        output_string_stream << std::to_string(m_year);
        preceeding_date_token_exists = true;
    }


    bool preceding_time_token_exists = false;
    if (mask & DateOutputMask::hour)
    {
        if (preceeding_date_token_exists) output_string_stream << "/ (";
        output_string_stream << std::to_string(m_hour);
        preceding_time_token_exists = true;
    }

    if (mask & DateOutputMask::minute)
    {
        if (preceeding_date_token_exists && !preceding_time_token_exists)
            output_string_stream << "/ (";
        else if (preceding_time_token_exists)
            output_string_stream << ":";
        if (m_minute < 10) output_string_stream << "0";
        output_string_stream << std::to_string(m_minute);
        preceding_time_token_exists = true;
    }

    if (mask & DateOutputMask::second)
    {
        if (preceeding_date_token_exists && !preceding_time_token_exists)
            output_string_stream << "/ (";
        else if (preceding_time_token_exists)
            output_string_stream << ":";
        if (m_second < 10) output_string_stream << "0";
        output_string_stream << std::to_string(m_second) << ")";
    }

    return output_string_stream.str();
}


TimeSpan::TimeSpan() : m_years{ 0 }, m_months{ 0 }, m_days{ 0 }, m_hours{ 0 }, m_minutes{ 0 }, m_seconds{ 0 }
{

}

TimeSpan::TimeSpan(int years, int months, int days, int hours, int minutes, double seconds) :
    m_years{ years }, m_months{ months }, m_days{ days }, m_hours{ hours }, m_minutes{ minutes }, m_seconds{ seconds }
{

}

int TimeSpan::years() const { return m_years; }

int TimeSpan::months() const { return m_months; }

int TimeSpan::days() const { return m_days; }

int TimeSpan::hours() const { return m_hours; }

int TimeSpan::minutes() const { return m_minutes; }

double TimeSpan::seconds() const { return m_seconds; }

TimeSpan& TimeSpan::operator=(TimeSpan const& other)
{
    if (this == &other) return *this;

    m_years = other.m_years;
    m_months = other.m_months;
    m_days = other.m_days;
    m_hours = other.m_hours;
    m_minutes = other.m_minutes;
    m_seconds = other.m_seconds;

    return *this;
}

TimeSpan TimeSpan::operator+(TimeSpan const& other) const
{
    return TimeSpan{ m_years + other.m_years, m_months + other.m_months, m_days + other.m_days,
        m_hours + other.m_hours, m_minutes + other.m_minutes, m_seconds + other.m_seconds };
}

TimeSpan TimeSpan::operator-(TimeSpan const& other) const
{
    return TimeSpan{ m_years - other.m_years, m_months - other.m_months, m_days - other.m_days,
        m_hours - other.m_hours, m_minutes - other.m_minutes, m_seconds - other.m_seconds };
}

TimeSpan TimeSpan::operator-() const
{
    return TimeSpan{ -m_years, -m_months, -m_days, -m_hours, -m_minutes, -m_seconds };
}