#include "LogQueue.h"
#include <Windows.h> // OutputDebugString

CLogQueue::CLogQueue(const std::shared_ptr<ILogSink>& pLogDelegate)
    : m_pDelegate(pLogDelegate)
    , m_WorkerThread(&CLogQueue::ConsumerThread, this)
{
}

CLogQueue::~CLogQueue()
{
    // trigger thread EOL
    m_bStopProcessing = true;
    m_QueueChanged.notify_one();

    // wait for the thread to complete
    if (m_WorkerThread.joinable())
    {
        m_WorkerThread.join();
    }
}

void CLogQueue::Drain()
{
    bool bQueueEmpty = false;
    while (!bQueueEmpty)
    {
        std::unique_lock<std::mutex> lock(m_AccessQueue);
        bQueueEmpty = m_MessageQueue.empty();
        if (!bQueueEmpty)
        {
            lock.unlock();
            std::this_thread::yield();
        }
    }
}

size_t CLogQueue::GetMessageQueueSize()
{
    return m_MessageQueue.size();
}

void CLogQueue::OutputString(const std::wstring& text)
{
    // make the change while locked, and unlock before signalling the changed condition
    std::unique_lock<std::mutex> lock(m_AccessQueue);
    m_MessageQueue.push(text);
    lock.unlock();
    // signal new message
    m_QueueChanged.notify_one();
}

void CLogQueue::ConsumerThread()
{
    while (true)
    {
        // unlock as soon as possible, before dealing with the potentially very slow Delegate, to allow the producer access to the queue
        std::unique_lock<std::mutex> lock(m_AccessQueue);
        m_QueueChanged.wait(lock, [this] { return m_bStopProcessing || !m_MessageQueue.empty(); });
        // check the 'abort' condition
        if (m_bStopProcessing)
        {
            OutputDebugStringW(__FUNCTIONW__ L": Stop Processing\n");
            return;
        }
        if (!m_MessageQueue.empty())
        {
            std::wstring sMessage = m_MessageQueue.front();
            m_MessageQueue.pop();
            lock.unlock(); // release the queue for access by log producers
            m_pDelegate->OutputString(sMessage);
        }
    }
}
