#pragma once

#include "Logger.h"
#include <queue>

class CLogQueue : public ILogSink
{
public:
    CLogQueue(const std::shared_ptr<ILogSink>& logDelegate);
    virtual ~CLogQueue();

    void Drain();
    size_t GetMessageQueueSize();

public: // ILogSink
    void OutputString(const std::string& text) override;

protected:
    void ConsumerThread();

protected:
    std::shared_ptr<ILogSink> m_Delegate;
    std::queue<std::string> m_MessageQueue; // FIFO queue
    std::mutex m_AccessQueue;
    std::condition_variable m_QueueChanged;
    std::thread m_WorkerThread;
    bool m_IsProcessingStopped;
};
