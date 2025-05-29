#include "hellolib/Events.hpp"
#include <iomanip>


namespace hellolib {

void handleAuthEvent(const AuthEvent & authEvent) {
    std::visit([](auto&& concreteEvent) {
        using T = std::decay_t<decltype(concreteEvent)>;
        std::cout << "User is " << concreteEvent.authMetaData_.userId_ << "\n";

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

void handleSessionEvent(const SessionEvent & sessionEvent) {
    std::visit([](auto&& concreteEvent) {
        using T = std::decay_t<decltype(concreteEvent)>;
        std::cout << "Session id is " << concreteEvent.sessionMetaData_.sessionId_ << "\n";

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

void handleEvent(const Event & event) {
    std::cout << "PID is " << event.pid_<< "\n";

    std::time_t time = std::chrono::system_clock::to_time_t(event.timestamp_);
    std::tm tm = *std::localtime(&time); // Convert to local time
    std::cout << "Timestamp is " << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << std::endl;

    std::visit([&](auto&& subtype) {
        using T = std::decay_t<decltype(subtype)>;
        if constexpr (std::is_same_v<T, AuthEvent>) {
            handleAuthEvent(subtype);
        } else if constexpr (std::is_same_v<T, SessionEvent>) {
            handleSessionEvent(subtype);
        }
    }, event.payload_);
}

}