#include <gtest/gtest.h>
#include "MKLogging/Logger.h"

TEST(VersionTest, GetVersion)
{
    std::string version = MKLogging::GetVersion();
    EXPECT_FALSE(version.empty());
    EXPECT_NE(version, "");
}