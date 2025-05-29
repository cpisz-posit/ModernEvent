#include "classiclib/Events.hpp"
#include "purecomplib/Events.hpp"

#include <iostream>
#include <memory>


int main(int argc, char ** argv)
{
    std::cout << "------------- Classic Event Typing Example -------------\n";
    auto sessionStartEvent = std::unique_ptr<classiclib::EventBase>(new classiclib::SessionStartEvent{ 1234, std::chrono::system_clock::now(), "session123" });
    classiclib::handleEvent(sessionStartEvent.get());

    // Ain't this the ugliest constructor you've ever seen?
    std::cout << "------------- Pure Composition Event Typing Example -------------\n";
    purecomplib::AuthLoginEvent authLoginEvent{ {{9876, std::chrono::system_clock::now()} , "fred"} };
    purecomplib::handleEvent(authLoginEvent);

    return 0;
}
