#include "classiclib/Events.hpp"
#include "purecomplib/Events.hpp"
#include "templatecastlib/Events.hpp"

#include <benchmark/benchmark.h>

#include <fstream>
#include <memory>


void BM_classic(benchmark::State & state)
{
    std::ofstream out("temp.txt", std::ios::out | std::ios::trunc);
    if (!out)
    {
        throw std::runtime_error("Failed to open output file");
    }

    for (auto _ : state)
    {
        auto sessionStartEvent = std::unique_ptr<classiclib::EventBase>(new classiclib::SessionStartEvent{ 9876, std::chrono::system_clock::now(), "session123", 42 });
        classiclib::handleEvent(sessionStartEvent.get(), out);

        auto authLoginEvent = std::unique_ptr<classiclib::EventBase>(new classiclib::AuthLoginEvent{ 6789, std::chrono::system_clock::now(), "Fred", 42 });
        classiclib::handleEvent(authLoginEvent.get(), out);
    }

    out.close();
}
BENCHMARK(BM_classic);

void BM_purecomp(benchmark::State & state)
{
    std::ofstream out("temp.txt", std::ios::out | std::ios::trunc);
    if (!out)
    {
        throw std::runtime_error("Failed to open output file");
    }

    for (auto _ : state)
    {
        auto sessionStartEvent = std::make_unique<purecomplib::Event>(purecomplib::SessionEvent{purecomplib::SessionStartEvent{9876, std::chrono::system_clock::now(), "session123", 42}});
        purecomplib::handleEvent(sessionStartEvent.get(), out);

        auto authLoginEvent = std::make_unique<purecomplib::Event>(purecomplib::AuthEvent{purecomplib::AuthLoginEvent{6789, std::chrono::system_clock::now(), "Fred", 42}});
        purecomplib::handleEvent(authLoginEvent.get(), out);
    }

    out.close();
}
BENCHMARK(BM_purecomp);

void BM_templatecast(benchmark::State & state)
{
    std::ofstream out("temp.txt", std::ios::out | std::ios::trunc);
    if (!out)
    {
        throw std::runtime_error("Failed to open output file");
    }

    for (auto _ : state)
    {
        auto sessionStartEvent = std::unique_ptr<templatecastlib::IEvent>(new templatecastlib::SessionStartEvent{9876, std::chrono::system_clock::now(), "session123", 42});
        templatecastlib::handleEvent(sessionStartEvent.get(), out);

        auto authLoginEvent = std::unique_ptr<templatecastlib::IEvent>(new templatecastlib::AuthLoginEvent{6789, std::chrono::system_clock::now(), "Fred", 42});
        templatecastlib::handleEvent(authLoginEvent.get(), out);
    }

    out.close();
}
BENCHMARK(BM_templatecast);


BENCHMARK_MAIN();
