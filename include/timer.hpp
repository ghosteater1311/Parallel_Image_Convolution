// Define timer.hpp
#ifndef TIMER_HPP
#define TIMER_HPP

// Include chrono for high-precision timing (Stop-watch timer class in C++ 11)
#include <chrono>

// Timer Class
// Usage:
//   Timer timer;
//   timer.start();
//   // ... code to measure ...
//   timer.stop();
//   double elapsed = timer.elapsedMilliseconds();
//
// Features:
//   - High-resolution (nanosecond precision on most systems)
//   - Simple start/stop interface
//   - Returns time in milliseconds (double for sub-ms precision)

class Timer 
{
    private:
        // Type aliases for readability
        using Clock = std::chrono::high_resolution_clock;
        using TimePoint = std::chrono::time_point<Clock>;

        TimePoint m_start;   // Start time
        TimePoint m_end;     // End time
        bool m_running;      // Is timer currently running?

    public:
        // Constructor - initializes timer in stopped state
        Timer() : m_running(false) {}

        // start() - Begin timing
        // Records the current time as the start point.
        // Can be called multiple times to reset the start point.
        void start() 
        {
            m_start = Clock::now();
            m_running = true;
        }

        // stop() - End timing
        // Records the current time as the end point.
        // Must call start() before calling stop().
        void stop() 
        {
            m_end = Clock::now();
            m_running = false;
        }

        // elapsedMilliseconds() - Get elapsed time
        // Returns the time between start() and stop() in milliseconds.
        // If timer is still running, returns time since start().
        // Returns double for sub-millisecond precision.
        //
        // Example:
        //   123.456 means 123 ms and 456 microseconds
        double elapsedMilliseconds() const 
        {
            TimePoint endTime = m_running ? Clock::now() : m_end;

            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
                endTime - m_start
            );

            return duration.count() / 1000.0;  // Convert μs to ms
        }

        // elapsedSeconds() - Get elapsed time in seconds
        // Convenience method for longer operations.
        // Returns double for sub-second precision.
        double elapsedSeconds() const 
        {
            return elapsedMilliseconds() / 1000.0;
        }

        // elapsedMicroseconds() - Get elapsed time in microseconds
        // For very fast operations needing maximum precision.
        double elapsedMicroseconds() const 
        {
            TimePoint endTime = m_running ? Clock::now() : m_end;

            auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(
                endTime - m_start
            );

            return duration.count() / 1000.0;  // Convert ns to μs
        }
};

#endif
