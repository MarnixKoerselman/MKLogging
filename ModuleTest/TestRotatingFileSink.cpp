// what we're testing:
import LogRotatingFileSink;

// what we need for for testing
#include <gtest/gtest.h>

using namespace testing;

// make (protected) implementation methods public, so they can be tested
class TestableLogRotatingFileSink : public LogRotatingFileSink
{
public:
  TestableLogRotatingFileSink(const std::wstring& sDirectory)
    : LogRotatingFileSink(sDirectory)
  {
  }
  std::filesystem::path GetNextFileName() const
  {
    return LogRotatingFileSink::GetNextFileName();
  }
  std::wstring GenerateFileName(const std::chrono::system_clock::time_point& now = std::chrono::system_clock::now()) const
  {
    return LogRotatingFileSink::GenerateFileName(now);
  }
};

TEST(RotatingLogFile, GenerateFileName)
{
  // we don't really care about the exact format of the generated file name, as long as it's different

  TestableLogRotatingFileSink sink(L"");

  std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
  std::chrono::system_clock::time_point later = now + std::chrono::seconds(1);

  EXPECT_NE(sink.GenerateFileName(now), sink.GenerateFileName(later));
  // also test that the file names are generated in an alphabetically sortable way, i.e. the filename generated at t=T should be lower in rank than for t=(T+1)
  EXPECT_LT(sink.GenerateFileName(now), sink.GenerateFileName(later));
  EXPECT_EQ(sink.GenerateFileName(now), sink.GenerateFileName(now));
}
