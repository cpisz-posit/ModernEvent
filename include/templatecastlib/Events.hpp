#ifndef TEMPLATECASTLIB_EVENTS_HPP
#define TEMPLATECASTLIB_EVENTS_HPP

#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <variant>

namespace templatecastlib
{

class SessionEventBase;
class AuthEventBase;

class IEventID
{
    template <class T, int ID>
    friend class EventID;

public:
    int eventType() const { return m_eventType; }
    bool isSessionEvent() const { return m_isSessionEvent; }
    bool isAuthEvent() const { return m_isAuthEvent; }

protected:
    IEventID() 
        : m_eventType(-1)
        , m_isSessionEvent{false}
        , m_isAuthEvent{false}
    {}

private:
    int m_eventType;
    bool m_isSessionEvent;
    bool m_isAuthEvent;
};

template <class THIS, int TYPE_ID>
class EventID : virtual public IEventID
{
public:
    enum { TypeId = TYPE_ID };

    template <typename SE>
    static std::shared_ptr<THIS> castShared(std::shared_ptr<SE>& event)
    {
        if (event->eventType() == TypeId)
        {
            return std::static_pointer_cast<THIS>(event);
        }
        return nullptr;
    }

protected:
    EventID()
    {
        m_eventType = TYPE_ID;
        m_isSessionEvent = std::is_base_of<SessionEventBase, THIS>::value;
        m_isAuthEvent = std::is_base_of<AuthEventBase, THIS>::value;
    }
};

class Event : virtual public IEventID
{
public:
    enum EventType
    {
        UNKONWN = 0,
        SESSION_START,
        SESSION_END,
        AUTH_LOGIN,
        AUTH_LOGOUT
    };

    const EventType type_;                               // Type of the event
    short pid_;                                          // Process ID reporting the event
    std::chrono::system_clock::time_point timestamp_;    // Timestamp the event occured

    virtual ~Event() = default;

    template<typename DESTINATIONTYPE>
    const DESTINATIONTYPE* cast() const
    {
        if(eventType() == DESTINATIONTYPE::TypeId)
        {
            return static_cast<const DESTINATIONTYPE*>(this);
        }
        return nullptr;
    }

    template<typename DESTINATIONTYPE>
    DESTINATIONTYPE* cast()
    {
        if(eventType() == DESTINATIONTYPE::TypeId)
        {
            return static_cast<DESTINATIONTYPE*>(this);
        }
        return nullptr;
    }

    template<typename DESTINATIONTYPE, typename SOURCETYPE>
    static std::shared_ptr<DESTINATIONTYPE> castShared(std::shared_ptr<SOURCETYPE>& event)
    {
        if (event->eventType() == DESTINATIONTYPE::TypeId)
        {
            return std::static_pointer_cast<DESTINATIONTYPE>(event);
        }
        return nullptr;
    }

    template<typename DESTINATIONTYPE, typename SOURCETYPE>
    static std::unique_ptr<DESTINATIONTYPE> static_unique_ptr_cast(std::unique_ptr<SOURCETYPE>&& event) 
    {
        if (event->eventType() == DESTINATIONTYPE::TypeId)
        {
            return std::unique_ptr<DESTINATIONTYPE>(static_cast<DESTINATIONTYPE*>(event.release()));
        }
        return nullptr;
    }

protected:
    Event(EventType type, short pid, std::chrono::system_clock::time_point timestamp);
};

class SessionEventBase : public Event
{
public:
    std::string sessionId_;                         // Unique identifier for the session

    ~SessionEventBase() override = default;

    template<typename SOURCETYPE>
    static std::shared_ptr<SessionEventBase> castShared(std::shared_ptr<SOURCETYPE>& event)
    {
        if (event->isSessionEvent() )
        {
            return std::static_pointer_cast<SessionEventBase>(event);
        }
        return nullptr;
    }

    template<typename SOURCETYPE>
    static std::unique_ptr<SessionEventBase> static_unique_ptr_cast(std::unique_ptr<SOURCETYPE>&& event) 
    {
        if (event->isSessionEvent() )
        {
            return std::unique_ptr<SessionEventBase>(static_cast<SessionEventBase*>(event.release()));
        }
        return nullptr;
    }

protected:
    SessionEventBase(Event::EventType type, short pid, std::chrono::system_clock::time_point timestamp, const std::string & sessionId)
        : Event(type, pid, timestamp)
        , sessionId_(sessionId)
    {}
};

template <>
inline const SessionEventBase* Event::cast<SessionEventBase>() const
{
    if (isSessionEvent())
    {
        return static_cast<const SessionEventBase*>(this);
    }
    return nullptr;
}

template <>
inline SessionEventBase* Event::cast<SessionEventBase>()
{
    if (isSessionEvent())
    {
        return static_cast<SessionEventBase*>(this);
    }
    return nullptr;
}


struct SessionStartEvent : public SessionEventBase, public EventID<SessionStartEvent, Event::SESSION_START>
{
    int specificData_;                              // Placeholder for additional specific data

    SessionStartEvent(short pid, std::chrono::system_clock::time_point timestamp, const std::string & sessionId, int specificData);
};

struct SessionEndEvent : public SessionEventBase, public EventID<SessionEndEvent, Event::SESSION_END>
{
    int specificData_;                              // Placeholder for additional specific data

    SessionEndEvent(short pid, std::chrono::system_clock::time_point timestamp, const std::string & sessionId, int specificData);
};


class AuthEventBase : public Event
{
public:
    std::string userId_;                         // Unique identifier for the session

    ~AuthEventBase() override = default;

    template<typename SOURCETYPE>
    static std::shared_ptr<AuthEventBase> castShared(std::shared_ptr<SOURCETYPE>& event)
    {
        if (event->isAuthEvent() )
        {
            return std::static_pointer_cast<AuthEventBase>(event);
        }
        return nullptr;
    }

    template<typename SOURCETYPE>
    static std::unique_ptr<AuthEventBase> static_unique_ptr_cast(std::unique_ptr<SOURCETYPE>&& event) 
    {
        if (event->isAuthEvent() )
        {
            return std::unique_ptr<AuthEventBase>(static_cast<AuthEventBase*>(event.release()));
        }
        return nullptr;
    }

protected:
    AuthEventBase(Event::EventType type, short pid, std::chrono::system_clock::time_point timestamp, const std::string & userId)
        : Event(type, pid, timestamp)
        , userId_(userId)
    {}
};

template <>
inline const AuthEventBase* Event::cast<AuthEventBase>() const
{
    if (isAuthEvent())
    {
        return static_cast<const AuthEventBase*>(this);
    }
    return nullptr;
}

template <>
inline AuthEventBase* Event::cast<AuthEventBase>()
{
    if (isAuthEvent())
    {
        return static_cast<AuthEventBase*>(this);
    }
    return nullptr;
}

struct AuthLoginEvent : public AuthEventBase, public EventID<AuthLoginEvent, Event::AUTH_LOGIN>
{
    int specificData_;                              // Placeholder for additional specific data

    AuthLoginEvent(short pid, std::chrono::system_clock::time_point timestamp, const std::string & userId, int specificData);
};

struct AuthLogoutEvent : public AuthEventBase, public EventID<AuthLogoutEvent, Event::AUTH_LOGOUT>
{
    int specificData_;                              // Placeholder for additional specific data

    AuthLogoutEvent(short pid, std::chrono::system_clock::time_point timestamp, const std::string & userId, int speceificData);
};

// Dispatcher for top level event into subtype handlers
void handleEvent(const Event * event, std::ostream & out);


} // end namespace templatecastlib

#endif // TEMPLATECASTLIB_EVENTS_HPP