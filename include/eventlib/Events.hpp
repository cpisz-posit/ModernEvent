#ifndef EVENTLIB_EVENTS_HPP
#define EVENTLIB_EVENTS_HPP

#include <chrono>
#include <iostream>
#include <string>
#include <variant>

namespace eventlib {

struct SessionEventMetaData
{
   // Data in common for all session events
   std::string sessionId_;                         // Unique identifier for the session
};

struct SessionStartEvent
{
    SessionEventMetaData sessionMetaData_;
    // additional data specific to session start can be added here
};

struct SessionEndEvent
{
    SessionEventMetaData sessionMetaData_;
    // additional data specific to session end can be added here
};

using SessionEvent = std::variant<SessionStartEvent, SessionEndEvent>;

struct AuthEventMetaData
{
   // Data in common for all authentication events
   std::string userId_;                            // Unique identifier for the user
};

struct AuthLoginEvent
{
    AuthEventMetaData authMetaData_;
    // additional data specific to auth login can be added here
};

struct AuthLogoutEvent
{
    AuthEventMetaData authMetaData_;
    // additional data specific to auth logout can be added here
};

using AuthEvent = std::variant<AuthLoginEvent, AuthLogoutEvent>;

// Top level union of event sub types
using EventPayload = std::variant<SessionEvent, AuthEvent>;


struct Event {
   // Data in common for all events
   short pid_;                                          // Process ID reporting the event
   std::chrono::system_clock::time_point timestamp_;    // Timestamp the event occured

   // Data specific to the event type
   EventPayload payload_;                               // The event payload is a variant of event sub types   
};

/* Subtype handlers called from top level event handeler */
void handleAuthEvent(const AuthEvent & authEvent);
void handleSessionEvent(const SessionEvent & sessionEvent);

/* Dispatcher for top level event into subtype handlers */
void handleEvent(const Event & e);

} // end namespace eventlib

#endif // _EVENTLIB_EVENTS_HPP