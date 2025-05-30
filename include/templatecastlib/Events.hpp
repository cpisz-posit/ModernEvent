#ifndef TEMPLATECASTLIB_EVENTS_HPP
#define TEMPLATECASTLIB_EVENTS_HPP

#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <variant>

namespace templatecastlib
{

struct IEvent
{
    enum EventType 
    {
        UNKONWN = 0,
        SESSION_START,
        SESSION_END,
        AUTH_LOGIN,
        AUTH_LOGOUT
    };

    // Data in common to all events
    const EventType type_;                               // Type of the event
    short pid_;                                          // Process ID reporting the event
    std::chrono::system_clock::time_point timestamp_;    // Timestamp the event occured

    IEvent(EventType type, short pid, std::chrono::system_clock::time_point timestamp);

    virtual ~IEvent() = 0;
    virtual int eventType() const = 0;

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
    static inline std::shared_ptr<DESTINATIONTYPE> castShared(std::shared_ptr<SOURCETYPE>& event)
    {
        if (event->eventType() == DESTINATIONTYPE::TypeId)
        {
            return std::static_pointer_cast<DESTINATIONTYPE>(event);
        }
        return nullptr;
    }

    template<typename DESTINATIONTYPE, typename SOURCETYPE>
    std::unique_ptr<DESTINATIONTYPE> static_unique_ptr_cast(std::unique_ptr<SOURCETYPE>&& event) 
    {
        if (event->eventType() == DESTINATIONTYPE::TypeId)
        {
            return std::unique_ptr<DESTINATIONTYPE>(static_cast<DESTINATIONTYPE*>(event.release()));
        }
        return nullptr;
    }
};

template <class T, int TYPE_ID>
struct EventBase : public IEvent
{
    enum{ TypeId = TYPE_ID };

    EventBase(IEvent::EventType type, short pid, std::chrono::system_clock::time_point timestamp)
        : IEvent(type, pid, timestamp)
    {}

    ~EventBase() override = 0;

    int eventType() const override { return type_; }

    template<typename DESTINATIONTYPE, typename SOURCETYPE>
    static inline std::shared_ptr<DESTINATIONTYPE> castShared(std::shared_ptr<SOURCETYPE>& event)
    {
        if (event->eventType() == DESTINATIONTYPE::TypeId)
        {
            return std::static_pointer_cast<DESTINATIONTYPE>(event);
        }
        return nullptr;
    }

    template<typename DESTINATIONTYPE, typename SOURCETYPE>
    std::unique_ptr<DESTINATIONTYPE> static_unique_ptr_cast(std::unique_ptr<SOURCETYPE>&& event) 
    {
        if (event->eventType() == DESTINATIONTYPE::TypeId)
        {
            return std::unique_ptr<DESTINATIONTYPE>(static_cast<DESTINATIONTYPE*>(event.release()));
        }
        return nullptr;
    }
};

template <class T, int TYPE_ID>
EventBase<T, TYPE_ID>::~EventBase()
{}

template <class T, int TYPE_ID>
struct SessionEventBase : public EventBase<T, TYPE_ID>
{
    // Additional data in common for all session events
    std::string sessionId_;                         // Unique identifier for the session


    SessionEventBase(IEvent::EventType type, short pid, std::chrono::system_clock::time_point timestamp, const std::string & sessionId)
        : EventBase<T, TYPE_ID>(type, pid, timestamp)
        , sessionId_(sessionId)
    {}

    ~SessionEventBase() override = 0;

    template<typename DESTINATIONTYPE, typename SOURCETYPE>
    static inline std::shared_ptr<DESTINATIONTYPE> castShared(std::shared_ptr<SOURCETYPE>& event)
    {
        if (event->eventType() == DESTINATIONTYPE::TypeId)
        {
            return std::static_pointer_cast<DESTINATIONTYPE>(event);
        }
        return nullptr;
    }

    template<typename DESTINATIONTYPE, typename SOURCETYPE>
    std::unique_ptr<DESTINATIONTYPE> static_unique_ptr_cast(std::unique_ptr<SOURCETYPE>&& event) 
    {
        if (event->eventType() == DESTINATIONTYPE::TypeId)
        {
            return std::unique_ptr<DESTINATIONTYPE>(static_cast<DESTINATIONTYPE*>(event.release()));
        }
        return nullptr;
    }
};

template <class T, int TYPE_ID>
SessionEventBase<T, TYPE_ID>::~SessionEventBase()
{}

struct SessionStartEvent : public SessionEventBase<SessionStartEvent, IEvent::SESSION_START>
{
    // Additional data specific to session start can be added here

    SessionStartEvent(short pid, std::chrono::system_clock::time_point timestamp, const std::string & sessionId);
};

struct SessionEndEvent : public SessionEventBase<SessionEndEvent, IEvent::SESSION_END>
{
    // Additional data specific to session start can be added here

    SessionEndEvent(short pid, std::chrono::system_clock::time_point timestamp, const std::string & sessionId);
};

template <class T, int TYPE_ID>
struct AuthEventBase : public EventBase<T, TYPE_ID>
{
    // Additional data in common for all auth events
    std::string userId_;                         // Unique identifier for the session


    AuthEventBase(IEvent::EventType type, short pid, std::chrono::system_clock::time_point timestamp, const std::string & userId)
        : EventBase<T, TYPE_ID>(type, pid, timestamp)
        , userId_(userId)
    {}

    ~AuthEventBase() override = 0;

    template<typename DESTINATIONTYPE, typename SOURCETYPE>
    static inline std::shared_ptr<DESTINATIONTYPE> castShared(std::shared_ptr<SOURCETYPE>& event)
    {
        if (event->eventType() == DESTINATIONTYPE::TypeID)
        {
            return std::static_pointer_cast<DESTINATIONTYPE>(event);
        }
        return nullptr;
    }

    template<typename DESTINATIONTYPE, typename SOURCETYPE>
    std::unique_ptr<DESTINATIONTYPE> static_unique_ptr_cast(std::unique_ptr<SOURCETYPE>&& event) 
    {
        if (event->eventType() == DESTINATIONTYPE::TypeId)
        {
            return std::unique_ptr<DESTINATIONTYPE>(static_cast<DESTINATIONTYPE*>(event.release()));
        }
        return nullptr;
    }
};

template <class T, int TYPE_ID>
AuthEventBase<T, TYPE_ID>::~AuthEventBase()
{}

struct AuthLoginEvent : public AuthEventBase<AuthLoginEvent, IEvent::AUTH_LOGIN>
{
    // Additional data specific to session start can be added here

    AuthLoginEvent(short pid, std::chrono::system_clock::time_point timestamp, const std::string & userId);
};

struct AuthLogoutEvent : public AuthEventBase<AuthLogoutEvent, IEvent::AUTH_LOGOUT>
{
    // Additional data specific to session start can be added here

    AuthLogoutEvent(short pid, std::chrono::system_clock::time_point timestamp, const std::string & userId);
};

// Dispatcher for top level event into subtype handlers
void handleEvent(const IEvent * event, std::ostream & out);


} // end namespace templatecastlib

#endif // TEMPLATECASTLIB_EVENTS_HPP