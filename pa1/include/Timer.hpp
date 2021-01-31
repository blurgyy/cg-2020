#pragma once

#include "global.hpp"

#include <chrono>

class Timer {
    using clk = std::chrono::steady_clock;

  private:
    std::chrono::time_point<clk> start_time;
    std::chrono::time_point<clk> end_time;

  public:
    Timer();

    // Start the timer.
    void start();
    // Pause the timer
    void end();
    // Get elapsed time in miliseconds.
    double elapsedms();
};

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Dec 18 2020, 23:39 [CST]
