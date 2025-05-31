
#include "templatecastlib/Events.hpp"

#include <iomanip>
#include <stdexcept>


namespace templatecastlib
{

Event::Event(EventType type, short pid, std::chrono::system_clock::time_point timestamp)
    : type_(type)
    , pid_(pid)
    , timestamp_(timestamp)
{}

SessionStartEvent::SessionStartEvent(short pid, std::chrono::system_clock::time_point timestamp, const std::string & sessionId, int specificData)
    : SessionEventBase(Event::SESSION_START, pid, timestamp, sessionId)
    , specificData_(specificData)
{}

SessionEndEvent::SessionEndEvent(short pid, std::chrono::system_clock::time_point timestamp, const std::string & sessionId, int specificData)
    : SessionEventBase(Event::SESSION_END, pid, timestamp, sessionId)
    , specificData_(specificData)
{}

AuthLoginEvent::AuthLoginEvent(short pid, std::chrono::system_clock::time_point timestamp, const std::string & userId, int specificData)
    : AuthEventBase(Event::AUTH_LOGIN, pid, timestamp, userId)
    , specificData_(specificData)
{}

AuthLogoutEvent::AuthLogoutEvent(short pid, std::chrono::system_clock::time_point timestamp, const std::string & userId, int specificData)
    : AuthEventBase(Event::AUTH_LOGOUT, pid, timestamp, userId)
    , specificData_(specificData)
{}

void handleSessionEvent(const SessionEventBase * sessionEvent, std::ostream & out)
{
    if(!sessionEvent)
    {
        throw std::invalid_argument("Session event pointer is null");
    }

    if(!out)
    {
        throw std::invalid_argument("Output stream is not valid");
    }

    out << "Session id is " << sessionEvent->sessionId_ << "\n";

    if(auto sessionStartEvent = sessionEvent->cast<SessionStartEvent>())
    {
        out << "Specific data for session start event: " << sessionStartEvent->specificData_ << "\n";
    }
    else if(auto sessionEndEvent = sessionEvent->cast<SessionEndEvent>())
    {
        out << "Specific data for session end event: " << sessionEndEvent->specificData_ << "\n";
    }
    else
    {
        throw std::invalid_argument("Unknown session event type encountered");
    }
}

void handleAuthEvent(const AuthEventBase * authEvent, std::ostream & out)
{
    if(!authEvent)
    {
        throw std::invalid_argument("Auth event pointer is null");
    }

    if(!out)
    {
        throw std::invalid_argument("Output stream is not valid");
    }

    out << "User is " << authEvent->userId_ << "\n";

    if(auto loginEvent = authEvent->cast<AuthLoginEvent>())
    {
        out << "Specific data for auth login event: " << loginEvent->specificData_ << "\n";
    }
    else if(auto logoutEvent = authEvent->cast<AuthLogoutEvent>())
    {
        out << "Specific data for auth logout event: " << logoutEvent->specificData_ << "\n";
    }
    else
    {
        throw std::invalid_argument("Unknown auth event type encountered");
    }
}

void handleEvent(const Event * event, std::ostream & out)
{
    if(!event)
    {
        throw std::invalid_argument("Event pointer is null");
    }
    if(!out)
    {
        throw std::invalid_argument("Output stream is not valid");
    }

    out << "PID is " << event->pid_ << std::endl;out << "PID is " << event->pid_ << std::endl;

    std::time_t time = std::chrono::system_clock::to_time_t(event->timestamp_);
    std::tm tm = *std::localtime(&time); // Convert to local time
    out << "Timestamp is " << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << std::endl;

    if(auto sessionEvent = event->cast<SessionEventBase>())
    {
        handleSessionEvent(sessionEvent, out);
    }
    else if(auto authEvent = event->cast<AuthEventBase>())
    {
        handleAuthEvent(authEvent, out);
    }
    else
    {
        throw std::invalid_argument("Unknown event type encountered");
    }
}

} // end namespace templatecastlib