#include <gtest/gtest.h>

#include "gdumper/pidutils.hpp"

TEST(PidUtilsTest, InvalidPid) {
    auto res = pidUtils::validatePid(-5);
    EXPECT_FALSE(res.has_value());
    EXPECT_EQ(res.error(), "Invalid PID (must be positive)");

    auto res2 = pidUtils::validatePid(0);
    EXPECT_FALSE(res2.has_value());
    EXPECT_EQ(res2.error(), "Invalid PID (must be positive)");
}

TEST(PidUtilsTest, NonExistingPid) {
    auto res = pidUtils::validatePid(999999);
    EXPECT_FALSE(res.has_value());
    EXPECT_TRUE(res.error().rfind("No process with PID", 0) == 0);
}

TEST(PidUtilsTest, CurrentPidExists) {
    pid_t self = getpid();
    auto res = pidUtils::validatePid(self);
    ASSERT_TRUE(res.has_value());
    EXPECT_EQ(res.value(), self);
}
