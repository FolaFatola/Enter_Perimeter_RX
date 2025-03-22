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

TEST_F(TimeTest, BCD_ConversionSuccess) {
    TimeManager t1{seconds_input, minutes_input, hours_input, week_day_input, date_day_input, month_input, year_input, use_military_time_input};
    EXPECT_EQ(RTC_OK, t1.convert_decimal_time_to_bcd());
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

TEST_F(TimeTest, WeekDay_Limit_Exceeded) {
    week_day_input = 8;
    TimeManager t6{seconds_input, minutes_input, hours_input, week_day_input, date_day_input, month_input, year_input, use_military_time_input};
    EXPECT_EQ(RTC_DATA_TOO_LARGE, t6.convert_decimal_time_to_bcd());
}

TEST_F(TimeTest, DateDay_Limit_Exceeded) {
    date_day_input = 32;
    TimeManager t7{seconds_input, minutes_input, hours_input, week_day_input, date_day_input, month_input, year_input, use_military_time_input};
    EXPECT_EQ(RTC_DATA_TOO_LARGE, t7.convert_decimal_time_to_bcd());
}

TEST_F(TimeTest, month_input_Limit_Exceeded) {
    month_input = 13;
    TimeManager t8{seconds_input, minutes_input, hours_input, week_day_input, date_day_input, month_input, year_input, use_military_time_input};
    EXPECT_EQ(RTC_DATA_TOO_LARGE, t8.convert_decimal_time_to_bcd());
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
