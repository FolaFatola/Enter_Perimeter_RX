/*
 * test.cpp
 *
 *  Created on: Mar 22, 2025
 *      Author: folafatola
 */

#include "test.hpp"
#include "timeunit.hpp"
#include <stdint.h>

class TimeTest : public testing::Test {
protected:
    TimeTest() {

    }

    uint8_t seconds_input = 30;
    uint8_t minutes_input = 24;
    uint8_t hours_input = 13;
    uint8_t week_day_input = 4;
    uint8_t date_day_input = 17;
    uint8_t month_input = MARCH;
    int year_input = 2025;
    bool use_military_time_input = true;
};

TEST_F(TimeTest, decimal_to_bcd_test) {
    TimeManager t1{seconds_input, minutes_input, hours_input, week_day_input, date_day_input, month_input, year_input, use_military_time_input};
    EXPECT_EQ(RTC_OK, t1.convert_decimal_time_to_bcd());
}

TEST_F(TimeTest, bcd_to_decimal_test) {
    TimeManager time{seconds_input, minutes_input, hours_input, week_day_input, date_day_input, month_input, year_input, use_military_time_input};
    EXPECT_EQ(RTC_OK, time.convert_decimal_time_to_bcd());
    EXPECT_EQ(RTC_OK, time.convert_bcd_time_to_decimal());

    TimeSpan unit_of_time = time.getTimeSpan();

    uint8_t seconds = unit_of_time.seconds_.time_value;
    uint8_t minutes = unit_of_time.minutes_.time_value;
    int year = 2000 + unit_of_time.year_.time_value;

    EXPECT_EQ(seconds_input, seconds);
    EXPECT_EQ(minutes_input, minutes);
    EXPECT_EQ(year_input, year);
}

TEST_F(TimeTest, Second_Limit_Exceeded) {
    seconds_input = 70;
    TimeManager t3{seconds_input, minutes_input, hours_input, week_day_input, date_day_input, month_input, year_input, use_military_time_input};
    EXPECT_EQ(RTC_DATA_TOO_LARGE, t3.convert_decimal_time_to_bcd());
}

TEST_F(TimeTest, minutes_input_Limit_Exceeded) {
    minutes_input = 70;
    TimeManager t4{seconds_input, minutes_input, hours_input, week_day_input, date_day_input, month_input, year_input, use_military_time_input};
    EXPECT_EQ(RTC_DATA_TOO_LARGE, t4.convert_decimal_time_to_bcd());
}

TEST_F(TimeTest, hours_input_Limit_Exceeded_Military_Time) {
    hours_input = 25;
    TimeManager t5{seconds_input, minutes_input, hours_input, week_day_input, date_day_input, month_input, year_input, use_military_time_input};
    EXPECT_EQ(RTC_DATA_TOO_LARGE, t5.convert_decimal_time_to_bcd());
}

TEST_F(TimeTest, hours_input_Limit_Exceeded_No_Military_Time) {
    hours_input = 13;
    use_military_time_input = false;
    TimeManager exceed_w_no_military_time{seconds_input, minutes_input, hours_input, week_day_input, date_day_input, month_input, year_input, use_military_time_input};
    EXPECT_EQ(RTC_DATA_TOO_LARGE, exceed_w_no_military_time.convert_decimal_time_to_bcd());
}

TEST_F(TimeTest, WeekDay_Limit_Exceeded) {
    week_day_input = 8;
    TimeManager t6{seconds_input, minutes_input, hours_input, week_day_input, date_day_input, month_input, year_input, use_military_time_input};
    EXPECT_EQ(RTC_DATA_TOO_LARGE, t6.convert_decimal_time_to_bcd());
}

TEST_F(TimeTest, DateDay_Limit_Exceeded) {
    date_day_input = 32;
    month_input = MARCH;
    TimeManager exceed_day_limit_march{seconds_input, minutes_input, hours_input, week_day_input, date_day_input, month_input, year_input, use_military_time_input};

    date_day_input = 30;
    month_input = FEBRUARY;
    TimeManager exceed_day_limit_feb{seconds_input, minutes_input, hours_input, week_day_input, date_day_input, month_input, year_input, use_military_time_input};

    date_day_input = 31;
    month_input = APRIL;
    TimeManager exceed_day_limit_april{seconds_input, minutes_input, hours_input, week_day_input, date_day_input, month_input, year_input, use_military_time_input};

    EXPECT_EQ(RTC_DATA_TOO_LARGE, exceed_day_limit_march.convert_decimal_time_to_bcd());
    EXPECT_EQ(RTC_DATA_TOO_LARGE, exceed_day_limit_feb.convert_decimal_time_to_bcd());
    EXPECT_EQ(RTC_DATA_TOO_LARGE, exceed_day_limit_april.convert_decimal_time_to_bcd());
}

TEST_F(TimeTest, month_input_Limit_Exceeded) {
    month_input = 13;
    TimeManager t8{seconds_input, minutes_input, hours_input, week_day_input, date_day_input, month_input, year_input, use_military_time_input};
    EXPECT_EQ(RTC_DATA_TOO_LARGE, t8.convert_decimal_time_to_bcd());
}

TEST_F(TimeTest, test_decimal_to_bcd_conversion) {
    seconds_input = 40;
    TimeManager time_1{seconds_input, minutes_input, hours_input, week_day_input, date_day_input, month_input, year_input, use_military_time_input};
    time_1.convert_decimal_time_to_bcd();
    TimeSpan unit1 = time_1.getTimeSpan();
    uint8_t seconds_bcd = unit1.seconds_.time_value;
    uint8_t tens_digit = (seconds_bcd & 0xF0) >> 4;
    uint8_t ones_digit = (seconds_bcd & 0x0F);
    EXPECT_EQ(tens_digit, 4);
    EXPECT_EQ(ones_digit, 0);


    minutes_input = 9;
    TimeManager time_2{seconds_input, minutes_input, hours_input, week_day_input, date_day_input, month_input, year_input, use_military_time_input};
    time_2.convert_decimal_time_to_bcd();
    TimeSpan unit2 = time_2.getTimeSpan();
    uint8_t minutes_bcd = unit2.minutes_.time_value;
    tens_digit = (minutes_bcd & 0xF0) >> 4;
    ones_digit = (minutes_bcd & 0x0F);
    EXPECT_EQ(tens_digit, 0);
    EXPECT_EQ(ones_digit, 9);

    hours_input = 10;
    TimeManager time_3{seconds_input, minutes_input, hours_input, week_day_input, date_day_input, month_input, year_input, use_military_time_input};
    time_3.convert_decimal_time_to_bcd();
    TimeSpan unit3 = time_3.getTimeSpan();
    uint8_t hours_bcd = unit3.hours_.time_value;
    tens_digit = (hours_bcd & 0xF0) >> 4;
    ones_digit = (hours_bcd & 0x0F);
    EXPECT_EQ(tens_digit, 1);
    EXPECT_EQ(ones_digit, 0);

    year_input = 2025;
    TimeManager time_4{seconds_input, minutes_input, hours_input, week_day_input, date_day_input, month_input, year_input, use_military_time_input};
    time_4.convert_decimal_time_to_bcd();
    TimeSpan unit4 = time_4.getTimeSpan();
    uint8_t years_bcd = unit4.year_.time_value;
    uint8_t tens_year = (years_bcd & 0xF0) >> 4;
    uint8_t ones_year = (years_bcd & 0x0F);
    EXPECT_EQ(tens_year, 2);
    EXPECT_EQ(ones_year, 5);
}



int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
