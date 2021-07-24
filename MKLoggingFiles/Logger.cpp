
#include "Logger.h"

// Global instance; automatically instantiated for each application that references this static library.
CLogCentral& LogCentral()
{
    static CLogCentral g_LogCentral({});
    return g_LogCentral;
}