// what we're testing:
#include "LogRotatingFileSink.h"

// what we need for for testing
#include <gtest/gtest.h>

using namespace testing;

class TestableLogRotatingFileSink : public CLogRotatingFileSink
{
public:
    TestableLogRotatingFileSink(const std::wstring& sDirectory)
        : CLogRotatingFileSink(sDirectory)
    {
    }
    std::filesystem::path GetNextFileName() const
    {
        return CLogRotatingFileSink::GetNextFileName();
    }
    std::wstring GenerateFileName(time_t now = time(nullptr)) const
    {
        return CLogRotatingFileSink::GenerateFileName(now);
    }
};

TEST(UnitTest, RollingLogFile_GenerateFileName)
{
    // we don't really care about the exact format of the generated file name, as long as it's different

    TestableLogRotatingFileSink sink(L"");

    time_t now;
    time(&now);

    std::wstring sFileName1 = sink.GenerateFileName(now);
    now++;
    std::wstring sFileName2 = sink.GenerateFileName(now);

    EXPECT_NE(sFileName1, sFileName2);

    // also test that the file names are generated in an alphabetically sortable way, i.e. the filename generated at t=T should be lower in rank than for t=(T+1)
    EXPECT_LT(sFileName1, sFileName2);
}
