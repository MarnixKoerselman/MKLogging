#include "LogConsoleSink.h"
#include <iostream>

void LogConsoleSink::OutputRecord(const LogRecord& record)
{
	OutputFormattedRecord(std::cout, record);
}
