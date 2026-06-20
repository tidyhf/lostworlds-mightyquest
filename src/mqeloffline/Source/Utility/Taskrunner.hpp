/*
    Initial author: Convery (tcn@hedgehogscience.com)
    Started: 09-01-2018
    License: MIT
    Notes:
        Provides a simple task wrapper.
*/

#pragma once
#include "../Stdinclude.hpp"

// Create a task that will run once or until termination.
inline void Createonetimetask(std::function<void()> Callback, uint32_t DelayMS)
{
    auto Lambda = [=]() -> void
    {
        std::this_thread::sleep_until(std::chrono::steady_clock::now() + std::chrono::milliseconds(DelayMS));
        Callback();
    };

    std::thread(Lambda).detach();
}
inline void Createrecurringtask(std::function<void()> Callback, uint32_t DelayMS)
{
    auto Lambda = [=]() -> void
    {
        while (true)
        {
            std::this_thread::sleep_until(std::chrono::steady_clock::now() + std::chrono::milliseconds(DelayMS));
            Callback();
        }
    };

    std::thread(Lambda).detach();
}
