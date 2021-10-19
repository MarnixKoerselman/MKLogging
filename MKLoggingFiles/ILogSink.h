#pragma once

//#include <string>
//#include "LogRecord.h"

#include <memory>
#include <string>

struct LogRecord;
class LogFormatter;

__interface ILogSink
{
	void OutputRecord(const LogRecord& record);
};

// Copy/paste:
//public: // ILogSink
//void OutputRecord(const LogRecord& record) override;

class LogSinkWithFormatter : public ILogSink
{
public: // ILogSink
	void OutputRecord(const LogRecord& record) override = 0;
public:
	void SetFormatter(const std::shared_ptr<LogFormatter>& formatter);
	void OutputFormattedRecord(std::ostream& os, const LogRecord& record);
private:
	static std::shared_ptr<LogFormatter> DefaultFormatter;
protected:
	std::shared_ptr<LogFormatter> m_Formatter = DefaultFormatter;
};
