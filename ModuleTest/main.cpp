#include <gtest/gtest.h>

// Custome main function to register our custom TestEnvironment
int main(int argc, char* argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
