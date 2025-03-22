#include "test.hpp"
#include "timeunit.hpp"
#include <stdint.h>


class TimeTest :  public testing::Test {
protected:
    TimeTest() {

    }

    TimeManager t1{30, 45, 9, 1, 1, JANUARY, 1, true};
    TimeManager t2{70, 24, 13, 4, 17, MARCH, 2025, true};
    // TimeManager t3;
};

TEST_F(TimeTest, BCD_ConversionSuccess) {
    EXPECT_EQ(RTC_OK, t1.convert_decimal_time_to_bcd());
}

TEST_F(TimeTest, LimitExceeded) {
    EXPECT_EQ(RTC_DATA_TOO_LARGE, t2.convert_decimal_time_to_bcd());
}



int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
