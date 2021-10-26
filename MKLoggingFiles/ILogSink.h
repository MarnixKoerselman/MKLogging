#pragma once

struct LogRecord;
class LogFormatter;

__interface ILogSink
{
	void OutputRecord(const LogRecord& record);
};

// Copy/paste:
//public: // ILogSink
//void OutputRecord(const LogRecord& record) override;

