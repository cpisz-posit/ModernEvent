#include "purecomplib/Events.hpp"
#include <iomanip>
#include <stdexcept>


namespace purecomplib
{

SessionStartEvent::SessionStartEvent(short pid, std::chrono::system_clock::time_point timestamp, const std::string& sessionId, int someSpecificData)
    : sessionBaseData_{ {pid, timestamp}, sessionId }
    , someSpecificData_(someSpecificData)
{}

SessionEndEvent::SessionEndEvent(short pid, std::chrono::system_clock::time_point timestamp, const std::string& sessionId, int someSpecificData)
    : sessionBaseData_{ {pid, timestamp}, sessionId }
    , someSpecificData_(someSpecificData)
{}

AuthLoginEvent::AuthLoginEvent(short pid, std::chrono::system_clock::time_point timestamp, const std::string& userId, int someSpecificData)
    : authBaseData_{ {pid, timestamp}, userId }
    , someSpecificData_(someSpecificData)
{}

AuthLogoutEvent::AuthLogoutEvent(short pid, std::chrono::system_clock::time_point timestamp, const std::string& userId, int specificData)
    : authBaseData_{ {pid, timestamp}, userId }
    , someSpecificData_(specificData)
{}

void handleAuthEvent(const AuthEvent * authEvent, std::ostream & out)
{
    if (!authEvent)
    {
        throw std::invalid_argument("authEvent pointer is null");
    }

    if(!out)
    {
        throw std::invalid_argument("Output stream is not valid");
    }

    std::visit([&out](auto && concreteEvent) 
    {
        using T = std::decay_t<decltype(concreteEvent)>;

        // Handle event base data
        out << "PID is " << concreteEvent.authBaseData_.eventBaseData_.pid_ << "\n";

        std::time_t time = std::chrono::system_clock::to_time_t(concreteEvent.authBaseData_.eventBaseData_.timestamp_);
        std::tm tm = *std::localtime(&time); // Convert to local time
        out << "Timestamp is " << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << std::endl;

        // Handle auth event base data
        out << "User is " << concreteEvent.authBaseData_.userId_ << "\n";

        if constexpr (std::is_same_v<T, AuthLoginEvent>)
        {
            out << "Some specific data for AuthLoginEvent: " << concreteEvent.someSpecificData_ << "\n";
        }
        else if constexpr (std::is_same_v<T, AuthLogoutEvent>)
        {
            out << "Some specific data for AuthLogoutEvent: " << concreteEvent.someSpecificData_ << "\n";
        }
        else
        {
            throw std::runtime_error("Unknown AuthEvent type");
        }
    }, *authEvent);
}

void handleSessionEvent(const SessionEvent * sessionEvent, std::ostream & out)
{
    if (!sessionEvent)
    {
        throw std::invalid_argument("sessionEvent pointer is null");
    }

    if(!out)
    {
        throw std::invalid_argument("Output stream is not valid");
    }
    
    std::visit([&out](auto&& concreteEvent)
    {
        using T = std::decay_t<decltype(concreteEvent)>;

        // Handle event base data
        // concreteEvent.sessionBaseData_.eventBaseData_.pid_  is really inconvenient
        out << "PID is " << concreteEvent.sessionBaseData_.eventBaseData_.pid_ << "\n";

        std::time_t time = std::chrono::system_clock::to_time_t(concreteEvent.sessionBaseData_.eventBaseData_.timestamp_);
        std::tm tm = *std::localtime(&time); // Convert to local time
        out << "Timestamp is " << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << std::endl;

        // Handle session event base data
        out << "Session id is " << concreteEvent.sessionBaseData_.sessionId_ << "\n";

        if constexpr (std::is_same_v<T, SessionStartEvent>)
        {
            out << "Some specific data for SessionStartEvent: " << concreteEvent.someSpecificData_ << "\n";
        }
        else if constexpr (std::is_same_v<T, SessionEndEvent>)
        {
            out << "Some specific data for SessionEndEvent: " << concreteEvent.someSpecificData_ << "\n";
        }
        else
        {
            throw std::runtime_error("Unknown SessionEvent type");
        }
    }, *sessionEvent);
}

void handleEvent(const Event * event, std::ostream & out)
{
    if(!event)
    {
        throw std::invalid_argument("event pointer is null");
    }

    if(!out)
    {
        throw std::invalid_argument("Output stream is not valid");
    }

    std::visit([&](auto&& subtype) {
        using T = std::decay_t<decltype(subtype)>;
        if constexpr (std::is_same_v<T, AuthEvent>)
        {
            handleAuthEvent(&subtype, out);
        } 
        else if constexpr (std::is_same_v<T, SessionEvent>)
        {
            handleSessionEvent(&subtype, out);
        }
    }, *event);
}

} // namespace purecomplib