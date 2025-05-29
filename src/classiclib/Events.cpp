#include "classiclib/Events.hpp"
#include <iomanip>


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

SessionStartEvent::SessionStartEvent(short pid, std::chrono::system_clock::time_point timestamp, const std::string & sessionId)
    : SessionEventBase(SESSION_START, pid, timestamp, sessionId)
{}

SessionEndEvent::SessionEndEvent(short pid, std::chrono::system_clock::time_point timestamp, const std::string & sessionId)
    : SessionEventBase(SESSION_END, pid, timestamp, sessionId)
{}

AuthEventBase::AuthEventBase(EventType type, short pid, std::chrono::system_clock::time_point timestamp, const std::string & userId)
    : EventBase(type, pid, timestamp)
    , userId_(userId) 
{}

AuthLoginEvent::AuthLoginEvent(short pid, std::chrono::system_clock::time_point timestamp, const std::string & userId)
    :
    AuthEventBase(AUTH_LOGIN, pid, timestamp, userId)
{}

AuthLogoutEvent::AuthLogoutEvent(short pid, std::chrono::system_clock::time_point timestamp, const std::string & userId)
    :
    AuthEventBase(AUTH_LOGOUT, pid, timestamp, userId)
{}

void handleAuthEvent(const AuthEventBase * authEvent)
{
    std::cout << "PID is " << authEvent->pid_ << std::endl;

    std::time_t time = std::chrono::system_clock::to_time_t(authEvent->timestamp_);
    std::tm tm = *std::localtime(&time); // Convert to local time
    std::cout << "Timestamp is " << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << std::endl;

    std::cout << "User is " << authEvent->userId_ << "\n";

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

            // Do specific things with the login event
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

void handleSessionEvent(const SessionEventBase * sessionEvent)
{
    std::cout << "PID is " << sessionEvent->pid_ << std::endl;

    std::time_t time = std::chrono::system_clock::to_time_t(sessionEvent->timestamp_);
    std::tm tm = *std::localtime(&time); // Convert to local time
    std::cout << "Timestamp is " << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << std::endl;

    std::cout << "Session id is " << sessionEvent->sessionId_ << std::endl;

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

            // Do specific things with the start event
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

            // Do specific things with the end event
            break;
        }
        default:
        {
            throw std::runtime_error("Unknown session event type encountered");
            break;
        }
    }
}

void handleEvent(const EventBase * event)
{
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

            handleSessionEvent(sessionEvent);
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

            handleAuthEvent(authEvent);
            break;
        }
        default:
            // Handle unknown event type
            throw std::runtime_error("Unknown event type encountered");
            break;
    }
}

} // end namespace classiclib