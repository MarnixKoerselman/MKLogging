#include "LogQueue.h"

LogQueue::LogQueue(const std::shared_ptr<ILogSink>& logDelegate)
  : m_Delegate(logDelegate)
  , m_IsProcessingStopped(false)
  , m_WorkerThread(&LogQueue::ConsumerThread, this)
{
}

LogQueue::~LogQueue()
{
  Drain();
  // trigger thread EOL
  m_IsProcessingStopped = true;
  m_QueueChanged.notify_one();

  // wait for the thread to complete
  m_WorkerThread.join();
}

void LogQueue::Drain()
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

size_t LogQueue::GetMessageQueueSize()
{
  return m_MessageQueue.size();
}

void LogQueue::OutputRecord(const LogRecord& record)
{
  // make the change while locked, and unlock before signalling the changed condition
  std::unique_lock<std::mutex> lock(m_AccessQueue);
  m_MessageQueue.push(record);
  lock.unlock();
  // signal new message
  m_QueueChanged.notify_one();
}

void LogQueue::ConsumerThread()
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
      return;
    }
    if (!m_MessageQueue.empty())
    {
      LogRecord record = m_MessageQueue.front();
      lock.unlock(); // release the queue for access by log producers
      if (m_Delegate)
      {
        m_Delegate->OutputRecord(record);
      }
      lock.lock();
      m_MessageQueue.pop();
      lock.unlock(); // release the queue for access by log producers
    }
  }
}
