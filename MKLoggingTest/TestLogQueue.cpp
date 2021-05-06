// what we're testing:
#include "LogQueue.h"

// what we need for for testing
#include "FakeFileLogSink.h"
#include "FakeStringLogSink.h"
#include "MockLogSink.h"

using namespace testing;

TEST(UnitTest, LogQueue)
{
    // Initialise the test environment
    StrictMock<MockLogSink> mockSink;
    // use trivial destructors so that the mock variables on the stack are not deleted by the shared_ptr cleanup
    std::shared_ptr<ILogSink> pMockSink(&mockSink, [] (auto)
        {});
    std::shared_ptr<CLogQueue> pBufferedSink = std::make_shared<CLogQueue>(pMockSink);

    // Set expectations
    std::string sTestString = "This is a test";
    EXPECT_CALL(mockSink, OutputString(sTestString))
        .RetiresOnSaturation();

    // Run the test
    pBufferedSink->OutputString(sTestString);
    // Waiting for things to happen in a multi-threaded environment is always a little tricky, particularly because adding timers (sleep) to the test would make the tests unnecessarily slow
    while (pBufferedSink->GetMessageQueueSize() > 0)
    {
        std::this_thread::yield();
    }
}
