#include "LogConsoleSink.h"
#include <iostream>

void CLogConsoleSink::OutputRecord(const LogRecord& record)
{
	OutputFormattedRecord(std::cout, record);
}
