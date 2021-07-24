
// use the main() function included in gmock
// #include "gmock/../../src/src/gmock_main.cc"

#include <gtest/gtest.h>
#include <Logger.h>
#include <Windows.h>
#include <LogDebugOutputSink.h>

class TestEnvironment : public testing::Environment
{
public:
    void SetUp() override
    {
        LogCentral().AddListener(std::make_shared<CLogDebugOutputSink>());
    }
    void TearDown() override
    {
    }
};

// Custome main function to register our custom TestEnvironment
int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    // gtest takes ownership of the TestEnvironment ptr - we don't delete it.
    ::testing::AddGlobalTestEnvironment(new TestEnvironment);
    return RUN_ALL_TESTS();
}
