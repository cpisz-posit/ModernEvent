#ifndef CLASSICLIB_EVENTS_HPP
#define CLASSICLIB_EVENTS_HPP

#include <chrono>
#include <iostream>
#include <string>
#include <variant>

namespace classiclib {

enum EventType 
{
    UNKONWN = 0,
    SESSION_START,
    SESSION_END,
    AUTH_LOGIN,
    AUTH_LOGOUT
};

struct EventBase
{
    const EventType type_;                               // Type of the event
    short pid_;                                          // Process ID reporting the event
    std::chrono::system_clock::time_point timestamp_;    // Timestamp the event occured

    EventBase(EventType type, short pid, std::chrono::system_clock::time_point timestamp);
    virtual ~EventBase() = default;
};

struct SessionEventBase : public EventBase
{
    std::string sessionId_;                         // Unique identifier for the session

    SessionEventBase(EventType type, short pid, std::chrono::system_clock::time_point timestamp, const std::string & sessionId);
    virtual ~SessionEventBase() = default;
};

struct SessionStartEvent : public SessionEventBase
{
    int someSpecificData_;                  // Placeholder for additional specific data

    SessionStartEvent(short pid, std::chrono::system_clock::time_point timestamp, const std::string & sessionId, int someSpecificData);
};

struct SessionEndEvent : public SessionEventBase
{
    int someSpecificData_;                  // Placeholder for additional specific data

    SessionEndEvent(short pid, std::chrono::system_clock::time_point timestamp, const std::string & sessionId, int someSpecificData);
};

struct AuthEventBase : public EventBase
{
    std::string userId_;                            // Unique identifier for the user

    AuthEventBase(EventType type, short pid, std::chrono::system_clock::time_point timestamp, const std::string & userId);
    virtual ~AuthEventBase() = default;
};

struct AuthLoginEvent : public AuthEventBase
{
    int someSpecificData_;                  // Placeholder for additional specific data

    AuthLoginEvent(short pid, std::chrono::system_clock::time_point timestamp, const std::string & userId, int someSpecificData);
};

struct AuthLogoutEvent : public AuthEventBase
{
    int someSpecificData_;                  // Placeholder for additional specific data

    AuthLogoutEvent(short pid, std::chrono::system_clock::time_point timestamp, const std::string & userId, int someSpecificData);
};


/* Dispatcher for top level event into subtype handlers */
void handleEvent(const EventBase * event, std::ostream & out);

} // end namespace eventlib

#endif // CLASSICLIB_EVENTS_HPP