#ifndef OX_WRAPPER_OPTIX_MODEL_DATE_TIME_H
#define OX_WRAPPER_OPTIX_MODEL_DATE_TIME_H

#include <stdint.h>
#include <string>

namespace ox_wrapper {


class TimeSpan;


//! Implements primitive Gregorian style date and time wrapper with the origin at January 1, 1970
//! Note that this calendar can only represent the dates after Christ
class DateTime
{
public:
    struct DateOutputMask
    {
        static unsigned char const year = 0x1;
        static unsigned char const month = 0x2;
        static unsigned char const day = 0x4;
        static unsigned char const hour = 0x8;
        static unsigned char const minute = 0x10;
        static unsigned char const second = 0x20;
    };

    enum class DateOutputStyle
    {
        european, american
    };

public:
    //! Initializes the date-time object to January 1, 1970 UTC.
    //! @param time_zone determines the time shift in hours from the UTC time calculated as the winter time (i.e. without taking the daylight saving into account).
    //! When @param daylight_saving_time is 'true' an extra hour is added to the time shift from UTC
    DateTime(int8_t time_zone = 0, bool daylight_saving_time = false);

    //! Initializes the date-time object to the given moment in time. The time moment supplied to constructor must be UTC time.
    //! Here @param year is the year represented by the date-time object
    //! @param month is the month of the @param year encapsulated by the date-time object. The value is clamped to 12 during initialization to enforce validity of the date.
    //! @param day is the current day of the @param month. The value is clamped to the last day of @param month in order to enforce validity of the date.
    //! @param hour is the current hour of the @param day. The value is clamped to 23 to enforce validity of the date.
    //! @param minute is the current minute of the @param hour. The value is clamped to 59 to enforce validity of the date.
    //! @param second is the current second of @param minute. The value is clamped to 59.999999999 to enforce validity of the date.
    //! @param time_zone determines the time shift in hours from the UTC time calculated as the winter time (i.e. without taking the daylight saving into account).
    //! When @param daylight_saving_time is 'true' an extra hour is added to the time shift from UTC
    DateTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, double second, int8_t time_zone = 0, bool daylight_saving_time = false);

    DateTime(DateTime const& other) = default; //! default copy constructor

                                               //! Constructs date-time object using @param nanoseconds passed since January 1, 1970, 00:00:00
    DateTime(unsigned long long nanoseconds);

    DateTime& operator= (DateTime const& other) = default;   //! assigns @param other date to this date

                                                             //! Calculates date that is later than this date by the time span determined by @param span.
                                                             //! Note that opposite to that of the time span, the date time object cannot store values that are larger than 255 in its
                                                             //! month, day, hour, and minute fields, and all of them (including the year second fields) must be non-negative. If the time span added
                                                             //! to the date is such that this requirement is violated the results are undefined
    DateTime operator+ (TimeSpan const& span) const;

    //! Calculates date that is earlier than this date by the time span determined by @param span.
    //! Note that opposite to that of the time span, the date time object cannot store values that are larger than 255 in its
    //! month, day, hour, and minute fields, and all of them (including the year and second fields) must be non-negative. If the time span subtracted from
    //! the date is such that this requirement is violated the results are undefined
    DateTime operator- (TimeSpan const& span) const;

    bool operator>=(DateTime const& other) const;    //! returns 'true' if this date is later or same than the @param other date
    bool operator<=(DateTime const& other) const;    //! returns 'true' if this date is earlier or same than the other date


    uint16_t year() const;
    uint8_t month() const;
    uint8_t day() const;
    uint8_t hour() const;
    uint8_t minute() const;
    double second() const;
    bool isLeapYear() const;

    int8_t getTimeZone() const;	//! returns time shift in hours from the UNC time, without accounting for the daylight saving shift
    bool isDTS() const;	//! returns 'true' if the time represented by the date time object is in daylight saving mode
    DateTime getUTC() const;	//! returns date and time equivalent to the date and time represented by this object converted to the UNC time zone
    DateTime getLocalTime(int8_t time_zone, bool daylight_saving) const;	//! returns date and time equivalent to the date and time represented by this object converted to requested time zone

                                                                            //! Calculates time passed since this date till the @param other date. If the @param other date is same or earlier than this date
                                                                            //! the function returns 0 time span
    TimeSpan timeSince(DateTime const& other) const;

    static DateTime now(int8_t time_zone = 0, bool daylight_saving = 0);	//! Returns date-time object encapsulating current date and time with the given time zone and DTS mode
    static DateTime convertNanosecondsToDate(unsigned long long nanoseconds, int8_t time_zone = 0, bool daylight_saving = 0);	//! Converts amount of nanoseconds passed from January 1, 1970, 00:00:00 into date-time representation

    std::string toString(unsigned char mask = 0x3F, DateOutputStyle style = DateOutputStyle::european) const;

private:
    uint16_t m_year;
    bool m_is_leap_year;
    uint8_t m_days_in_month[12];
    uint8_t m_month;
    uint8_t m_day;
    uint8_t m_hour;
    uint8_t m_minute;
    double m_second;
    int8_t m_time_shift_from_utc;    //!< total time shift to be added to the UNC time to get local time of the host
    bool m_is_dts;	//!< 'true' if the time is a daylight saving time
};


//! Time period represented by tuple of year, month, day, hour, minute and fractional second. Time spans can be negative.
//! Time span is merely a tuple, it does not define whether certain year is a leap year or a regular year nor does it define the number of days in a certain month.
//! Therefore, time spans do not support order relations. When two time spans are added their fields are added independently.
class TimeSpan
{
public:
    TimeSpan(); //! Initializes zero-length time span

                //! Initializes time span with given length of years, months, days, hours, minutes, and fractional seconds
    TimeSpan(int years, int months, int days, int hours, int minutes, double seconds);

    int years() const; //! Returns number of years in the time span
    int months() const;    //! Returns number of months in the time span
    int days() const;  //! Returns number of days in the time span
    int hours() const; //! Returns number of hours in the time span
    int minutes() const;   //! Returns number of minutes in the time span
    double seconds() const; //! Returns number of seconds in the time span

    TimeSpan& operator=(TimeSpan const& other);    //! Assigns this time span to the @param other
    TimeSpan operator+(TimeSpan const& other) const;    //! Adds @param other time span to this time span
    TimeSpan operator-(TimeSpan const& other) const;    //! Subtracts @param other time span from this time span
    TimeSpan operator-() const;  //! unary minus, reverses the sign (direction) of the time span

private:
    int m_years;
    int m_months;
    int m_days;
    int m_hours;
    int m_minutes;
    double m_seconds;
};

}

#endif
