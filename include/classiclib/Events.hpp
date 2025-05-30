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
    virtual ~EventBase() = default;                      // Virtual destructor for proper cleanup
};

struct SessionEventBase : public EventBase
{
    std::string sessionId_;                         // Unique identifier for the session

    SessionEventBase(EventType type, short pid, std::chrono::system_clock::time_point timestamp, const std::string & sessionId);
    virtual ~SessionEventBase() = default;          // Virtual destructor for proper cleanup
};

struct SessionStartEvent : public SessionEventBase
{
    // Additional data specific to session start can be added here

    SessionStartEvent(short pid, std::chrono::system_clock::time_point timestamp, const std::string & sessionId);
};

struct SessionEndEvent : public SessionEventBase
{
    // Additional data specific to session end can be added here

    SessionEndEvent(short pid, std::chrono::system_clock::time_point timestamp, const std::string & sessionId);
};

struct AuthEventBase : public EventBase
{
    std::string userId_;                            // Unique identifier for the user

    AuthEventBase(EventType type, short pid, std::chrono::system_clock::time_point timestamp, const std::string & userId);
    virtual ~AuthEventBase() = default;              // Virtual destructor for proper cleanup
};

struct AuthLoginEvent : public AuthEventBase
{
    // Additional data specific to auth login can be added here

    AuthLoginEvent(short pid, std::chrono::system_clock::time_point timestamp, const std::string & userId);
};

struct AuthLogoutEvent : public AuthEventBase
{
    // Additional data specific to auth logout can be added here

    AuthLogoutEvent(short pid, std::chrono::system_clock::time_point timestamp, const std::string & userId);
};


/* Dispatcher for top level event into subtype handlers */
void handleEvent(const EventBase * event, std::ostream & out);

} // end namespace eventlib

#endif // CLASSICLIB_EVENTS_HPP