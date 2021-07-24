#pragma once

#include <mutex>
#include <vector>
#include <initializer_list>

template <typename EventNotificationInterface>
class CEventSource
{
protected:
    using Mutex = std::recursive_mutex;
    using Lock = std::lock_guard<std::recursive_mutex>;

public:
    CEventSource() = default;
    
    explicit CEventSource(const std::initializer_list<std::shared_ptr<EventNotificationInterface>>& listeners)
        : m_Listeners(listeners)
    {
    }
    
    virtual ~CEventSource() = default;

    void AddListener(const std::shared_ptr<EventNotificationInterface>& pListener)
    {
        Lock lock(m_AccessListeners);
        m_Listeners.push_back(pListener);
    }
    
    void RemoveListener(const std::shared_ptr<EventNotificationInterface>& pListener)
    {
        Lock lock(m_AccessListeners);
        auto pos = std::find(m_Listeners.begin(), m_Listeners.end(), pListener);
        if (pos != m_Listeners.end())
        {
            m_Listeners.erase(pos);
        }
    }

    void RemoveAllListeners()
    {
        Lock lock(m_AccessListeners);
        m_Listeners.clear();
    }

    bool HasListeners() const
    {
        return !m_Listeners.empty();
    }

protected:
    std::vector<std::shared_ptr<EventNotificationInterface>> m_Listeners;
    Mutex m_AccessListeners;
};

