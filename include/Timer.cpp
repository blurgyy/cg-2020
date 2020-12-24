#include "Timer.hpp"

Timer::Timer() : start_time{std::chrono::time_point<clk>()} {}

void Timer::start() { this->start_time = clk::now(); }
void Timer::end() { this->end_time = clk::now(); }

double Timer::elapsedms() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
               this->end_time - this->start_time)
        .count();
}

// Author: Blurgy <gy@blurgy.xyz>
// Date:   Dec 18 2020, 23:39 [CST]
