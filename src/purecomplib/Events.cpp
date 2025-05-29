#include "purecomplib/Events.hpp"
#include <iomanip>


namespace purecomplib 
{

void handleAuthEvent(const AuthEvent & authEvent)
{
    std::visit([](auto&& concreteEvent) 
    {
        using T = std::decay_t<decltype(concreteEvent)>;

        // Handle event base data
        std::cout << "PID is " << concreteEvent.authBaseData_.eventBaseData_.pid_ << "\n";

        std::time_t time = std::chrono::system_clock::to_time_t(concreteEvent.authBaseData_.eventBaseData_.timestamp_);
        std::tm tm = *std::localtime(&time); // Convert to local time
        std::cout << "Timestamp is " << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << std::endl;

        // Handle auth event base data
        std::cout << "User is " << concreteEvent.authBaseData_.userId_ << "\n";

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
            std::cerr << "Unknown AuthEvent type\n";
        }
    }, authEvent);
}

void handleSessionEvent(const SessionEvent & sessionEvent)
{
    std::visit([](auto&& concreteEvent)
    {
        using T = std::decay_t<decltype(concreteEvent)>;

        // Handle event base data
        // concreteEvent.sessionBaseData_.eventBaseData_.pid_  is really inconvenient
        std::cout << "PID is " << concreteEvent.sessionBaseData_.eventBaseData_.pid_ << "\n";

        std::time_t time = std::chrono::system_clock::to_time_t(concreteEvent.sessionBaseData_.eventBaseData_.timestamp_);
        std::tm tm = *std::localtime(&time); // Convert to local time
        std::cout << "Timestamp is " << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << std::endl;

        // Handle session event base data
        std::cout << "Session id is " << concreteEvent.sessionBaseData_.sessionId_ << "\n";

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
            std::cerr << "Unknown SessionEvent type\n";
        }
    }, sessionEvent);
}

void handleEvent(const Event & event)
{
    std::visit([&](auto&& subtype) {
        using T = std::decay_t<decltype(subtype)>;
        if constexpr (std::is_same_v<T, AuthEvent>)
        {
            handleAuthEvent(subtype);
        } 
        else if constexpr (std::is_same_v<T, SessionEvent>)
        {
            handleSessionEvent(subtype);
        }
    }, event);
}

} // namespace purecomplib