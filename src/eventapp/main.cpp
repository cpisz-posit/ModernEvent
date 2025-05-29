#include "purecomplib/Events.hpp"

#include <iostream>


int main(int argc, char ** argv)
{
    // Ain't this the ugliest constructor you've ever seen?
    purecomplib::AuthLoginEvent authLoginEvent{ {{9876, std::chrono::system_clock::now()} , "fred"} };
    purecomplib::handleEvent(authLoginEvent);

    return 0;
}
