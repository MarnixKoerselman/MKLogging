
#include "Logger.h"
#include "LogDebugOutputSink.h"

//// Global instance; automatically instantiated for each application that references this static library.
//#ifdef _DEBUG
//// Every debug logger uses OutputDebugString
//CLogCentral g_LogCentral({ std::make_shared<CLogDebugOutputSink>() });
//#else // _DEBUG
CLogCentral g_LogCentral({});
//#endif // _DEBUG
