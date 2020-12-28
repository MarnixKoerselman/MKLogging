
#include "Logger.h"
#include "LogDebugOutputSink.h"

// Global instance; automatically instantiated for each application that references this static library.
#ifdef _DEBUG
// Every debug logger uses OutputDebugString
CLogCentral g_LogCentral({ std::make_shared<CLogDebugOutputSink>() });
#else // _DEBUG
CLogCentral g_LogCentral({});
#endif // _DEBUG

// This notation would give the application more control over the initialisation of the global logger
//std::shared_ptr<CLogCentral> g_LogCentral = std::make_shared<CLogCentral>(std::list<std::shared_ptr<ILogSink>>({ std::make_shared<CLogDebugOutputSink>() }));
