
#include "templatecastlib/Events.hpp"

#include <iomanip>
#include <stdexcept>


namespace templatecastlib
{

IEvent::IEvent(EventType type, short pid, std::chrono::system_clock::time_point timestamp)
    : type_(type)
    , pid_(pid)
    , timestamp_(timestamp)
{}

IEvent::~IEvent()
{}


SessionStartEvent::SessionStartEvent(short pid, std::chrono::system_clock::time_point timestamp, const std::string & sessionId)
    : SessionEventBase<SessionStartEvent, IEvent::SESSION_START>(IEvent::SESSION_START, pid, timestamp, sessionId)
{}

SessionEndEvent::SessionEndEvent(short pid, std::chrono::system_clock::time_point timestamp, const std::string & sessionId)
    : SessionEventBase<SessionEndEvent, IEvent::SESSION_END>(IEvent::SESSION_END, pid, timestamp, sessionId)
{}

AuthLoginEvent::AuthLoginEvent(short pid, std::chrono::system_clock::time_point timestamp, const std::string & userId)
    : AuthEventBase<AuthLoginEvent, IEvent::AUTH_LOGIN>(IEvent::AUTH_LOGIN, pid, timestamp, userId)
{}

AuthLogoutEvent::AuthLogoutEvent(short pid, std::chrono::system_clock::time_point timestamp, const std::string & userId)
    : AuthEventBase<AuthLogoutEvent, IEvent::AUTH_LOGOUT>(IEvent::AUTH_LOGOUT, pid, timestamp, userId)
{}

template <class T, int TYPE_ID>
void handleSessionEvent(const SessionEventBase<T, TYPE_ID> * sessionEvent, std::ostream & out)
{

}

template <class T, int TYPE_ID>
void handleAuthEvent(const AuthEventBase<T, TYPE_ID> * authEvent, std::ostream & out)
{
    if(!out)
    {
        throw std::runtime_error("Output stream is not valid");
    }

    out << "User is " << authEvent->userId_ << "\n";

    switch(authEvent->type_)
    {
        case IEvent::EventType::AUTH_LOGIN:
        {
            auto loginEvent = authEvent->template cast<AuthLoginEvent>();
            if(!loginEvent)
            { 
                throw std::runtime_error("Cast to auth login event type failed");
            }

            // Do specific things with the login event
            break;
        }
        case IEvent::EventType::AUTH_LOGOUT:
        {
            auto logoutEvent = authEvent->template cast<AuthLogoutEvent>();
            if(!logoutEvent)
            { 
                throw std::runtime_error("Cast to auth logout event type failed");
            }

            // Do specific things with the logout event
            break;
        }
        default:
        {
            throw std::runtime_error("Unknown auth event type encountered");
            break;
        }
    }
}

void handleEvent(const IEvent * event, std::ostream & out)
{
    if(!out)
    {
        throw std::runtime_error("Output stream is not valid");
    }

    out << "PID is " << event->pid_ << std::endl;out << "PID is " << event->pid_ << std::endl;

    std::time_t time = std::chrono::system_clock::to_time_t(event->timestamp_);
    std::tm tm = *std::localtime(&time); // Convert to local time
    out << "Timestamp is " << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << std::endl;

    switch(event->eventType())
    {
        // Unfortunately, the way we defined the hierarchy currently, we have to know the template params to cast to a 
        // type between the interface and the concrete type for each case. We could just move/repeat the handling of 
        // session data members to where we handle the data memebers of the concrete type and pass IEvent * to the subtype handler,
        // but that could be a lot of code duplication.
        case IEvent::EventType::SESSION_START:
        {
            auto sessionEvent = event->cast<SessionEventBase<SessionStartEvent, IEvent::SESSION_START>>();
            if (!sessionEvent)
            {
                throw std::runtime_error("Cast to session event type failed");
            }
            handleSessionEvent(sessionEvent, out);
            break;
        }
        case IEvent::EventType::SESSION_END:
        {
            auto sessionEvent = event->cast<SessionEventBase<SessionEndEvent, IEvent::SESSION_END>>();
            if (!sessionEvent)
            {
                throw std::runtime_error("Cast to session event type failed");
            }
            handleSessionEvent(sessionEvent, out);
            break;
        }
        case IEvent::EventType::AUTH_LOGIN:
        {
            auto authEvent = event->cast<AuthEventBase<AuthLoginEvent, IEvent::AUTH_LOGIN>>();
            if (!authEvent)
            {
                throw std::runtime_error("Cast to auth event type failed");
            }
            handleAuthEvent(authEvent, out);
            break;
        }
        case IEvent::EventType::AUTH_LOGOUT:
        {
            auto authEvent = event->cast<AuthEventBase<AuthLogoutEvent, IEvent::AUTH_LOGOUT>>();
            if (!authEvent)
            {
                throw std::runtime_error("Cast to auth event type failed");
            }
            handleAuthEvent(authEvent, out);
            break;
        }
        default:
            // Handle unknown event type
            throw std::runtime_error("Unknown event type encountered");
            break;
    }
}

} // end namespace templatecastlib