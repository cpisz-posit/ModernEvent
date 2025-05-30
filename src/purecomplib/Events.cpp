#include "purecomplib/Events.hpp"
#include <iomanip>
#include <stdexcept>


namespace purecomplib 
{

void handleAuthEvent(const AuthEvent & authEvent, std::ostream & out)
{
    if(!out)
    {
        throw std::runtime_error("Output stream is not valid");
    }

    std::visit([&out](auto&& concreteEvent) 
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
            // Do things specific to AuthLoginEvent here
        }
        else if constexpr (std::is_same_v<T, AuthLogoutEvent>)
        {
            // Do things specific to AuthLogoutEvent here
        }
        else
        {
            throw std::runtime_error("Unknown AuthEvent type");
        }
    }, authEvent);
}

void handleSessionEvent(const SessionEvent & sessionEvent, std::ostream & out)
{
    if(!out)
    {
        throw std::runtime_error("Output stream is not valid");
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
            // Do things specific to SessionStartEvent here
        }
        else if constexpr (std::is_same_v<T, SessionEndEvent>)
        {
            // Do things specific to SessionEndEvent here
        }
        else
        {
            throw std::runtime_error("Unknown SessionEvent type");
        }
    }, sessionEvent);
}

void handleEvent(const Event & event, std::ostream & out)
{
    std::visit([&](auto&& subtype) {
        using T = std::decay_t<decltype(subtype)>;
        if constexpr (std::is_same_v<T, AuthEvent>)
        {
            handleAuthEvent(subtype, out);
        } 
        else if constexpr (std::is_same_v<T, SessionEvent>)
        {
            handleSessionEvent(subtype, out);
        }
    }, event);
}

} // namespace purecomplib