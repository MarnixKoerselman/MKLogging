// what we're testing:
#include "ErrorMessages.h"

// what we need for for testing
#include <gtest/gtest.h>

TEST(UnitTest, GetLastWindowsErrorMessage)
{
    std::wstring sLastErrorMessage = GetLastWindowsErrorMessage();
    EXPECT_STREQ(L"(0x00000000) The operation completed successfully.", sLastErrorMessage.c_str());
}
