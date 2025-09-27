#include <gtest/gtest.h>

#include "gdumper/processmanager.hpp"

TEST(ProcessManagerTest, EmptyLineReturnsNull) {
    auto res = processManager::formatLine("", "", false);
    EXPECT_FALSE(res.has_value());
}

TEST(ProcessManagerTest, NoSlashInLineReturnsNull) {
    auto res = processManager::formatLine("1234 programname", "", false);
    EXPECT_FALSE(res.has_value());
}

TEST(ProcessManagerTest, ReturnFilenameOnlyWhenShowFullPathFalse) {
    auto res = processManager::formatLine("1234 /usr/bin/ls", "", false);
    ASSERT_TRUE(res.has_value());
    EXPECT_EQ(res.value(), "ls");
}

TEST(ProcessManagerTest, ReturnFullPathWhenShowFullPathTrue) {
    auto res = processManager::formatLine("1234 /usr/bin/ls", "", true);
    ASSERT_TRUE(res.has_value());
    EXPECT_EQ(res.value(), "/usr/bin/ls");
}

TEST(ProcessManagerTest, FilterMatches) {
    auto res = processManager::formatLine("1234 /usr/bin/grep", "grep", false);
    ASSERT_TRUE(res.has_value());
    EXPECT_EQ(res.value(), "grep");
}

TEST(ProcessManagerTest, FilterNoMatchReturnsNull) {
    auto res = processManager::formatLine("1234 /usr/bin/grep", "awk", false);
    EXPECT_FALSE(res.has_value());
}

