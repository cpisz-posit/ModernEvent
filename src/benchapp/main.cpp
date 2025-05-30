#include "classiclib/Events.hpp"
#include "purecomplib/Events.hpp"

#include <benchmark/benchmark.h>

#include <fstream>
#include <memory>


void BM_classic(benchmark::State & state)
{
    for (auto _ : state)
    {
        std::ofstream out("temp.txt", std::ios::out | std::ios::trunc);
        if (!out)
        {
            throw std::runtime_error("Failed to open output file");
        }

        auto sessionStartEvent = std::unique_ptr<classiclib::EventBase>(new classiclib::SessionStartEvent{ 1234, std::chrono::system_clock::now(), "session123" });
        classiclib::handleEvent(sessionStartEvent.get(), out);

        auto authLoginEvent = std::unique_ptr<classiclib::EventBase>(new classiclib::AuthLoginEvent{ 1234, std::chrono::system_clock::now(), "Fred" });
        classiclib::handleEvent(authLoginEvent.get(), out);

        out.close();
    }
}
BENCHMARK(BM_classic);

void BM_purecomp(benchmark::State & state)
{
    for (auto _ : state)
    {
        std::ofstream out("temp.txt", std::ios::out | std::ios::trunc);
        if (!out)
        {
            throw std::runtime_error("Failed to open output file");
        }

        purecomplib::SessionStartEvent sessionStartEvent{ {{9876, std::chrono::system_clock::now()} , "session123"} };
        purecomplib::handleEvent(sessionStartEvent, out);

        purecomplib::AuthLoginEvent authLoginEvent{ {{9876, std::chrono::system_clock::now()} , "fred"} };
        purecomplib::handleEvent(authLoginEvent, out);

        out.close();
    }
}
BENCHMARK(BM_purecomp);


BENCHMARK_MAIN();
