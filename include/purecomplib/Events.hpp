#ifndef PURECOMPLIB_EVENTS_HPP
#define PURECOMPLIB_EVENTS_HPP

#include <chrono>
#include <iostream>
#include <string>
#include <variant>

namespace purecomplib 
{

struct EventBaseData 
{
    // Data in common to all events
    short pid_;                                          // Process ID reporting the event
    std::chrono::system_clock::time_point timestamp_;    // Timestamp the event occured
};

struct SessionEventBaseData
{
    EventBaseData eventBaseData_;

    // Additional data in common for all session events
    std::string sessionId_;                         // Unique identifier for the session
};

struct SessionStartEvent
{
    SessionEventBaseData sessionBaseData_;
    int someSpecificData_;                  // Placeholder for additional specific data

    SessionStartEvent(short pid, std::chrono::system_clock::time_point timestamp, const std::string& sessionId, int someSpecificData);
};

struct SessionEndEvent
{
    SessionEventBaseData sessionBaseData_;
    int someSpecificData_;                  // Placeholder for additional specific data

    SessionEndEvent(short pid, std::chrono::system_clock::time_point timestamp, const std::string& sessionId, int someSpecificData);
};

// This must be updated when creating a new session event type
using SessionEvent = std::variant<SessionStartEvent, SessionEndEvent>;


struct AuthEventBaseData
{
    EventBaseData eventBaseData_;

    // Additional data in common for all auth events
    std::string userId_;                            // Unique identifier for the user
};

struct AuthLoginEvent
{
    AuthEventBaseData authBaseData_;
    int someSpecificData_;                  // Placeholder for additional specific data

    AuthLoginEvent(short pid, std::chrono::system_clock::time_point timestamp, const std::string& userId, int someSpecificData);
};

struct AuthLogoutEvent
{
    AuthEventBaseData authBaseData_;
    int someSpecificData_;                  // Placeholder for additional specific data

    AuthLogoutEvent(short pid, std::chrono::system_clock::time_point timestamp, const std::string& userId, int specificData);
};

// This must be updated when creating a new auth event type
using AuthEvent = std::variant<AuthLoginEvent, AuthLogoutEvent>;

// Top level union of event sub types
using Event = std::variant<SessionEvent, AuthEvent>;


/* Dispatcher for top level event into subtype handlers */
void handleEvent(const Event * event, std::ostream & out);

} // end namespace eventlib

#endif // PURECOMPLIB_EVENTS_HPP