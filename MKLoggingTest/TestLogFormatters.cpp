#include <Logger.h>
//#include <LogFileSink.h>
//#include <LogQueue.h>
//#include <LogDebugOutputSink.h>
//#include <LogUnbufferedFileSink.h>
#include <LogFormatter.h>

//#include <string>
//#include <filesystem>

#include <gtest/gtest.h>
//#include "TestUtils.h"
//#include "StringUtils.h"
#include "MockLogSink.h"
#include "FakeStringLogSink.h"

using testing::_;

TEST(LogFormatters, DefaultLogFormatter)
{
	LogFormatter formatter;

	auto function = __FUNCTION__;
	auto line = __LINE__;

	MockLogSink mockSink;
	EXPECT_CALL(mockSink, OutputRecord(_)).WillOnce([function, line] (const LogRecord& record)
													{
														EXPECT_EQ(ELogLevel::Info, record.LogLevel);
														EXPECT_STREQ(function, record.Function.c_str());
														EXPECT_STREQ(__FILE__, record.File.c_str());
														EXPECT_EQ(line, record.LineNumber);
														//EXPECT_EQ(_, record.Time);
														EXPECT_EQ(std::this_thread::get_id(), record.ThreadId);
														//EXPECT_EQ(GetProcessId(), record.ProcessId);
														EXPECT_STREQ("test", record.GetMessage().c_str());
													});

	{
		LogRecord(&mockSink, ELogLevel::Info, function, __FILE__, line).Get() << "test";
	}
}

TEST(LogFormatters, MessageOnlyLogFormatter)
{
	MessageOnlyLogFormatter formatter;
	std::shared_ptr<FakeStringLogSink> buffer = std::make_shared<FakeStringLogSink>();
	{
		LogRecord(buffer.get(), ELogLevel::Info, __FUNCTION__, __FILE__, __LINE__).Get() << "test";
	}
	EXPECT_STREQ("test", buffer->Buffer.c_str());
	buffer->Buffer.clear();

	CLogCentral logger;
	logger.AddListener(buffer);
	MKL_LOGD(logger, "test");
	EXPECT_STREQ("test\n", buffer->Buffer.c_str());
}

