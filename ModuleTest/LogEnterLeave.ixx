export module LogEnterLeave;
import ELogLevel;
import Logger;

export class LogEnterLeave
{
public:
  LogEnterLeave(Logger* logger, ELogLevel logLevel, const char* function, const char* file, int line);
  virtual ~LogEnterLeave();

private:
  Logger* m_Logger;
  const ELogLevel m_LogLevel;
  const char* m_Function;
  const char* m_File;
  const long m_Line;
};
