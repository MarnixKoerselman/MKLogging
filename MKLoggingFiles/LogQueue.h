#pragma once

#include "Logger.h"
#include <queue>

class CLogQueue : public ILogSink
{
public:
    CLogQueue(const std::shared_ptr<ILogSink>& pLogDelegate);
    virtual ~CLogQueue();
    void Drain();
    size_t GetMessageQueueSize();
public: // ILogSink
    void OutputString(const std::wstring& text) override;
private:
    void ConsumerThread();
private:
    std::shared_ptr<ILogSink> m_pDelegate;
    std::queue<std::wstring> m_MessageQueue; // FIFO queue
    std::mutex m_AccessQueue;
    std::condition_variable m_QueueChanged;
    std::thread m_WorkerThread;
    bool m_bStopProcessing = false;
};
