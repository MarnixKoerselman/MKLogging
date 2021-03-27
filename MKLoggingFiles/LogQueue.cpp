#include "LogQueue.h"
#include <Windows.h> // OutputDebugString

CLogQueue::CLogQueue(const std::shared_ptr<ILogSink>& pLogDelegate)
    : m_pDelegate(pLogDelegate)
    , m_WorkerThread(&CLogQueue::ConsumerThread, this)
    , m_IsProcessingStopped(false)
{
}

CLogQueue::~CLogQueue()
{
    Drain();
    // trigger thread EOL
    m_IsProcessingStopped = true;
    m_QueueChanged.notify_one();

    // wait for the thread to complete
    if (m_WorkerThread.joinable())
    {
        m_WorkerThread.join();
    }
}

void CLogQueue::Drain()
{
    bool isQueueEmpty = false;
    while (!isQueueEmpty)
    {
        std::unique_lock<std::mutex> lock(m_AccessQueue);
        isQueueEmpty = m_MessageQueue.empty();
        if (!isQueueEmpty)
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
        std::unique_lock<std::mutex> lock(m_AccessQueue);
        // unlock as soon as possible, before dealing with the potentially very slow Delegate, to allow the producer access to the queue
        m_QueueChanged.wait(lock, [this]
        {
            return m_IsProcessingStopped || !m_MessageQueue.empty();
        });
        // check the 'abort' condition
        if (m_IsProcessingStopped)
        {
            OutputDebugStringW(__FUNCTIONW__ L": Stop Processing\n");
            return;
        }
        if (!m_MessageQueue.empty())
        {
            std::wstring sMessage = m_MessageQueue.front();
            m_MessageQueue.pop();
            lock.unlock(); // release the queue for access by log producers
            if (m_pDelegate)
            {
                m_pDelegate->OutputString(sMessage);
            }
        }
        //else
        //{
        //    // spurious signal detected!
        //    LOGW("Spurious signal detected!");
        //    // this should not happen, but just in case:
        //    lock.unlock();
        //}
    }
}
