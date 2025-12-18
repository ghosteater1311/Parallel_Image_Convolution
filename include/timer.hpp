// Define timer.hpp
// High-Resolution Timer Utilities (namespace-based)
// Lightweight namespace functions and a small ScopedTimer RAII helper
// for precise benchmarking using C++11 <chrono>.
//
// Usage examples:
//   auto t0 = timer::now();
//   // ... work ...
//   double ms = timer::msBetween(t0, timer::now());
//
//   // or using RAII helper:
//   timer::ScopedTimer st; // starts immediately
//   // ... work ...
//   double ms = st.elapsedMs();

#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>

namespace timer 
{
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;

    // Return current high-resolution time point
    inline TimePoint now() 
    {
        return Clock::now();
    }

    // Milliseconds between two time points (double, ms with fractional)
    inline double msBetween(const TimePoint& start, const TimePoint& end) 
    {
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        return duration.count() / 1000.0;
    }

    // Milliseconds since start
    inline double msSince(const TimePoint& start) 
    {
        return msBetween(start, now());
    }

    // Seconds between two time points
    inline double secondsBetween(const TimePoint& start, const TimePoint& end) 
    {
        return msBetween(start, end) / 1000.0;
    }

    // Microseconds between two time points
    inline double usBetween(const TimePoint& start, const TimePoint& end) 
    {
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        return duration.count() / 1000.0; // ns -> us
    }

    // Lightweight RAII helper that records start time on construction
    // and provides elapsed time queries. No automatic logging/destruction
    // side-effects to keep it simple and safe for benchmarking.
    struct ScopedTimer 
    {
        TimePoint start;
        ScopedTimer() : start(now()) {}
        double elapsedMs() const { return msSince(start); }
        double elapsedUs() const { return usBetween(start, now()); }
        double elapsedSeconds() const { return secondsBetween(start, now()); }
    };
} 

#endif 

