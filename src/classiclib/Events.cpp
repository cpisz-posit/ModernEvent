#include "classiclib/Events.hpp"
#include <iomanip>
#include <stdexcept>


namespace classiclib {

EventBase::EventBase(EventType type, short pid, std::chrono::system_clock::time_point timestamp)
    : type_(type)
    , pid_(pid)
    , timestamp_(timestamp) 
{}

SessionEventBase::SessionEventBase(EventType type, short pid, std::chrono::system_clock::time_point timestamp, const std::string & sessionId)
    : EventBase(type, pid, timestamp)
    , sessionId_(sessionId)
{};

SessionStartEvent::SessionStartEvent(short pid, std::chrono::system_clock::time_point timestamp, const std::string & sessionId, int someSpecificData)
    : SessionEventBase(SESSION_START, pid, timestamp, sessionId)
    , someSpecificData_(someSpecificData)
{}

SessionEndEvent::SessionEndEvent(short pid, std::chrono::system_clock::time_point timestamp, const std::string & sessionId, int someSpecificData)
    : SessionEventBase(SESSION_END, pid, timestamp, sessionId)
    , someSpecificData_(someSpecificData)
{}

AuthEventBase::AuthEventBase(EventType type, short pid, std::chrono::system_clock::time_point timestamp, const std::string & userId)
    : EventBase(type, pid, timestamp)
    , userId_(userId) 
{}

AuthLoginEvent::AuthLoginEvent(short pid, std::chrono::system_clock::time_point timestamp, const std::string & userId, int someSpecificData)
    :
    AuthEventBase(AUTH_LOGIN, pid, timestamp, userId)
    , someSpecificData_(someSpecificData)
{}

AuthLogoutEvent::AuthLogoutEvent(short pid, std::chrono::system_clock::time_point timestamp, const std::string & userId, int someSpecificData)
    :
    AuthEventBase(AUTH_LOGOUT, pid, timestamp, userId)
    , someSpecificData_(someSpecificData)
{}

void handleAuthEvent(const AuthEventBase * authEvent, std::ostream & out)
{
    if (!authEvent)
    {
        throw std::invalid_argument("Auth event is not valid");
    }

    if(!out)
    {
        throw std::invalid_argument("Output stream is not valid");
    }

    out << "User is " << authEvent->userId_ << "\n";

    switch(authEvent->type_)
    {
        case EventType::AUTH_LOGIN:
        {
            /* If we want to avoid dynamic_cast and hope no programmer makes mistakes with the type enum
               then we could static_cast, but if they do mislabel then we run the risk of UB
            auto loginEvent = static_cast<const AuthLoginEvent *>(authEvent);
            */
            auto loginEvent = dynamic_cast<const AuthLoginEvent *>(authEvent);
            if(!loginEvent) 
            { 
                throw std::runtime_error("Cast to auth login event type failed");
            }

            out << "Some specific data for auth login: " << loginEvent->someSpecificData_ << std::endl;
            break;
        }
        case EventType::AUTH_LOGOUT:
        {
            /* If we want to avoid dynamic_cast and hope no programmer makes mistakes with the type enum
               then we could static_cast, but if they do mislabel then we run the risk of UB
            auto logoutEvent = static_cast<const AuthLogoutEvent *>(authEvent);
            */
            auto logoutEvent = dynamic_cast<const AuthLogoutEvent *>(authEvent);
            if(!logoutEvent) 
            { 
                throw std::runtime_error("Cast to auth logout event type failed");
            }

            out << "Some specific data for auth logout: " << logoutEvent->someSpecificData_ << std::endl;
            break;
        }
        default:
        {
            throw std::invalid_argument("Unknown auth event type encountered");
            break;
        }
    }
}

void handleSessionEvent(const SessionEventBase * sessionEvent, std::ostream & out)
{
    if (!sessionEvent)
    {
        throw std::invalid_argument("Session event is not valid");
    }
    
    if(!out)
    {
        throw std::invalid_argument("Output stream is not valid");
    }

    out << "Session id is " << sessionEvent->sessionId_ << std::endl;

    switch(sessionEvent->type_)
    {
        case EventType::SESSION_START:
        {
            /* If we want to avoid dynamic_cast and hope no programmer makes mistakes with the type enum
               then we could static_cast, but if they do mislabel then we run the risk of UB
            auto startEvent = static_cast<const SessionStartEvent *>(sessionEvent);
            */
            auto startEvent = dynamic_cast<const SessionStartEvent *>(sessionEvent);
            if(!startEvent)
            { 
                throw std::runtime_error("Cast to session start event type failed");
            }

            out << "Some specific data for session start: " << startEvent->someSpecificData_ << std::endl;
            break;
        }
        case EventType::SESSION_END:
        {
            /* If we want to avoid dynamic_cast and hope no programmer makes mistakes with the type enum
               then we could static_cast, but if they do mislabel then we run the risk of UB
            auto endEvent = static_cast<const SessionEndEvent *>(sessionEvent);
            */
            auto endEvent = dynamic_cast<const SessionEndEvent *>(sessionEvent);
            if(!endEvent)
            { 
                throw std::runtime_error("Cast to session end event type failed");
            }

            out << "Some specific data for session end: " << endEvent->someSpecificData_ << std::endl;
            break;
        }
        default:
        {
            throw std::runtime_error("Unknown session event type encountered");
            break;
        }
    }
}

void handleEvent(const EventBase * event, std::ostream & out)
{
    if(!event)
    {
        throw std::invalid_argument("Event is not valid");
    }

    if(!out)
    {
        throw std::invalid_argument("Output stream is not valid");
    }

    out << "PID is " << event->pid_ << std::endl;

    std::time_t time = std::chrono::system_clock::to_time_t(event->timestamp_);
    std::tm tm = *std::localtime(&time); // Convert to local time
    out << "Timestamp is " << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << std::endl;

    switch(event->type_)
    {
        case EventType::SESSION_START:
        case EventType::SESSION_END:
        {
            /* If we want to avoid dynamic_cast and hope no programmer makes mistakes with the type enum
               then we could static_cast, but if they do mislabel then we run the risk of UB
            auto sessionEvent = static_cast<const SessionEventBase *>(event);
            */
            auto sessionEvent = dynamic_cast<const SessionEventBase *>(event);
            if(!sessionEvent) 
            { 
                throw std::runtime_error("Cast to session event type failed"); 
            }

            handleSessionEvent(sessionEvent, out);
            break;
        }
        case EventType::AUTH_LOGIN:
        case EventType::AUTH_LOGOUT:
        {
            /* If we want to avoid dynamic_cast and hope no programmer makes mistakes with the type enum
               then we could static_cast, but if they do mislabel then we run the risk of UB
            auto authEvent = static_cast<const AuthEventBase *>(event);
            */
            auto authEvent = dynamic_cast<const AuthEventBase *>(event);
            if(!authEvent)
            { 
                throw std::runtime_error("Cast to session event type failed"); 
            }

            handleAuthEvent(authEvent, out);
            break;
        }
        default:
            // Handle unknown event type
            throw std::invalid_argument("Unknown event type encountered");
            break;
    }
}

} // end namespace classiclib