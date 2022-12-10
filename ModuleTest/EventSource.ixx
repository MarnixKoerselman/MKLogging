export module EventSource;
import <initializer_list>;
import <memory>;
import <mutex>;
import <vector>;

export template <typename EventNotificationInterface>
class EventSource
{
public:
  EventSource() = default;

  explicit EventSource(std::initializer_list<std::shared_ptr<EventNotificationInterface>> listeners)
    : m_Listeners(listeners)
  {
  }

  virtual ~EventSource() = default;

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
  using Mutex = std::recursive_mutex;
  using Lock = std::lock_guard<std::recursive_mutex>;

protected:
  std::vector<std::shared_ptr<EventNotificationInterface>> m_Listeners;
  Mutex m_AccessListeners;
};

