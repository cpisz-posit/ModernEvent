#include "eventlib/Events.hpp"

#include <iostream>


int main(int argc, char ** argv)
{
    eventlib::Event authLoginEvent{
        9876,
        std::chrono::system_clock::now(),
        eventlib::AuthLoginEvent{"1234"}
    };
    
    eventlib::handleEvent(authLoginEvent);

    return 0;
}
