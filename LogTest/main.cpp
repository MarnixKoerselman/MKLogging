
// use the main() function included in gmock
#include "gmock/../../src/src/gmock_main.cc"

// add a dummy test case that proves it works

#include "../LogShared/Logger.h"
TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);

  Logger foo;
}
