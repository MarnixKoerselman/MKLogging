#include <Logger.h>
#include <LogFormatter.h>

#include <gtest/gtest.h>
#include "MockLogSink.h"
#include "FakeStringLogSink.h"

#ifdef WIN32
#define NOMINMAX
#include <Windows.h>
#endif

using testing::_;

TEST(LogFormatters, DefaultLogFormatter)
{
  LogFormatter formatter;

  auto function = __FUNCTION__;
  auto line = __LINE__;

  MockLogSink mockSink;
  EXPECT_CALL(mockSink, OutputRecord(_)).WillOnce([function, line](const LogRecord& record)
    {
      EXPECT_EQ(ELogLevel::Info, record.LogLevel);
      EXPECT_STREQ(function, record.Function);
      EXPECT_STREQ(__FILE__, record.File);
      EXPECT_EQ(line, record.LineNumber);
      //EXPECT_EQ(_, record.Time);
      EXPECT_EQ(std::this_thread::get_id(), record.ThreadId);
      EXPECT_STREQ("test", record.GetLogMessage().c_str());
    });

  {
    LogRecordAutoSink(&mockSink, ELogLevel::Info, function, __FILE__, line).Get() << "test";
  }
}

TEST(LogFormatters, MessageOnlyLogFormatter_LogRecordWithSink)
{
  // unless explicitly configured, a log sink does not have a formatter
  std::shared_ptr<FakeStringLogSink> buffer = std::make_shared<FakeStringLogSink>();
  {
    LogRecordAutoSink(buffer.get(), ELogLevel::Info, __FUNCTION__, __FILE__, __LINE__).Get() << "test";
  }
  EXPECT_STREQ("test", buffer->Buffer.c_str());
  buffer->Buffer.clear();
}

TEST(LogFormatters, MessageOnlyLogFormatter_LogCentral)
{
  // Logger does have a default formatter, which must be explicitly replaced
  Logger logger;
  logger.SetFormatter(nullptr);
  logger.SetMinimumLogLevel(ELogLevel::All);
  std::shared_ptr<FakeStringLogSink> buffer = std::make_shared<FakeStringLogSink>();
  logger.AddListener(buffer);
  MKL_LOGD(&logger, "test");
  EXPECT_STREQ("test\n", buffer->Buffer.c_str());
}

TEST(LogFormatters, DerivedLogFormatter)
{
  class DerivedFormatter : public LogFormatter
  {
  public:
    virtual void OutputRecordWithFormatting(std::ostream& os, const LogRecord& record) override
    {
      os << "DerivedFormatter: " << record.GetLogMessage();
    }
  };

  Logger logger;
  logger.SetFormatter(std::make_shared<DerivedFormatter>());
  logger.SetMinimumLogLevel(ELogLevel::All);
  std::shared_ptr<FakeStringLogSink> buffer = std::make_shared<FakeStringLogSink>();
  logger.AddListener(buffer);
  MKL_LOGD(&logger, "test");
  EXPECT_STREQ("DerivedFormatter: test\n", buffer->Buffer.c_str());
}

TEST(LogFormatters, DerivedLogFormatterPartials)
{
    class DerivedFormatter : public LogFormatter
    {
    public:
        virtual void OutputLogLevel(std::ostream& os, ELogLevel /*logLevel*/) override
        {
            os << "loglevel";
        }
        virtual void OutputTime(std::ostream& os, const std::chrono::system_clock::time_point& /*time*/) override
        {
            os << "time";
        }
        virtual void OutputThreadId(std::ostream& os, std::thread::id /*threadId*/) override
        {
            os << "threadid";
        }
        virtual void OutputMessage(std::ostream& os, const LogRecord& /*record*/) override
        {
            os << "message";
        }
    };

    Logger logger;
    logger.SetFormatter(std::make_shared<DerivedFormatter>());
    logger.SetMinimumLogLevel(ELogLevel::All);
    std::shared_ptr<FakeStringLogSink> buffer = std::make_shared<FakeStringLogSink>();
    logger.AddListener(buffer);
    MKL_LOGD(&logger, "test");
    EXPECT_NE(std::string::npos, buffer->Buffer.find("time threadid loglevel message"));
}
