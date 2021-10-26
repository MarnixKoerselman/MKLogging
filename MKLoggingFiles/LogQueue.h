#pragma once

#include "ILogSink.h"
#include "LogRecord.h"
#include <queue>
#include <mutex>
#include <memory>

class LogQueue : public ILogSink
{
public:
	LogQueue(const std::shared_ptr<ILogSink>& logDelegate);
	virtual ~LogQueue();

	void Drain();
	size_t GetMessageQueueSize();

public: // ILogSink
	void OutputRecord(const LogRecord& record) override;

protected:
	void ConsumerThread();

protected:
	std::shared_ptr<ILogSink> m_Delegate;
	std::queue<LogRecord> m_MessageQueue; // FIFO queue
	std::mutex m_AccessQueue;
	std::condition_variable m_QueueChanged;
	std::thread m_WorkerThread;
	bool m_IsProcessingStopped;
};
