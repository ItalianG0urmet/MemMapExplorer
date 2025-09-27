#include <gtest/gtest.h>

#include "gdumper/argsmanager.hpp"

static char* makeArg(const char* s) { return const_cast<char*>(s); }

TEST(ArgsManagerTest, MissingPid) {
    const char* argv[] = {"prog"};
    auto res = Args::parse(1, const_cast<char**>(argv));
    EXPECT_FALSE(res.has_value());
    EXPECT_EQ(res.error(), "Use: -p <PID> [-f <filter>] [-a]");
}

TEST(ArgsManagerTest, InvalidPidNotANumber) {
    const char* argv[] = {"prog", "-p", "abc"};
    auto res = Args::parse(3, const_cast<char**>(argv));
    EXPECT_FALSE(res.has_value());
    EXPECT_TRUE(res.error().rfind("Invalid PID (not a number)", 0) == 0);
}

TEST(ArgsManagerTest, PidOutOfRange) {
    const char* argv[] = {"prog", "-p", "999999999999999999999999"};
    auto res = Args::parse(3, const_cast<char**>(argv));
    EXPECT_FALSE(res.has_value());
    EXPECT_TRUE(res.error().rfind("PID out of range", 0) == 0);
}

TEST(ArgsManagerTest, ValidPidOnly) {
    const char* argv[] = {"prog", "-p", "1234"};
    auto res = Args::parse(3, const_cast<char**>(argv));
    ASSERT_TRUE(res.has_value());
    EXPECT_EQ(res->pid, 1234);
    EXPECT_FALSE(res->showFullPath);
    EXPECT_TRUE(res->onlyFindString.empty());
}

TEST(ArgsManagerTest, ValidPidWithFilterAndFullPath) {
    const char* argv[] = {"prog", "-p", "4321", "-f", "needle", "-a"};
    auto res = Args::parse(6, const_cast<char**>(argv));
    ASSERT_TRUE(res.has_value());
    EXPECT_EQ(res->pid, 4321);
    EXPECT_EQ(res->onlyFindString, "needle");
    EXPECT_TRUE(res->showFullPath);
}

