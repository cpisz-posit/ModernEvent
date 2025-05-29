#include "hellolib/Events.hpp"

#include <iostream>


int main(int argc, char ** argv)
{
    hellolib::Event authLoginEvent{
        9876,
        std::chrono::system_clock::now(),
        hellolib::AuthLoginEvent{"1234"}
    };
    
    hellolib::handleEvent(authLoginEvent);

    return 0;
}
