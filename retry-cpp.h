#ifndef RETRY_CPP_H
#define RETRY_CPP_H

#include <iostream>
#include <chrono>
#include <thread>
#include <stdexcept>
#include <future>
#include <random>

struct retry_parameter {
    bool    stop_after_attempt = false;     // Flag to enable the feature of stoping the retry after certain attempt.
    bool    stop_after_delay = false;       // Flag to enable the feature of stoping the retry after certain amout of time.
    bool    stop_after_success = false;     // Flag to enable the feature of stoping the retry only after the execution is success.
    bool    exponential_delay = false;      // Flag to enable the feature of stoping the retry after certain attempt.
    int     stop_after_attempt_count = 3;   // No of times the retry has to run if stop_after_attempt is true. 
    long    stop_after_delay_count = 3;     // How long the retry has to execute in seconds if the stop_after_delay is true.
    float   wait_before_retry = 1;          // Time difference between each retry.
    float   exponential_delay_factor = 2;   // Exponential factor on the wait_time of each delay when exponential_delay is enabled.
};

template<typename Func, typename... Args>
auto retry(Func func, Args... args) {
    retry_parameter retry_value; 
    return retry(func, retry_value, args...);
}

template<typename Func, typename... Args>
auto retry(Func func, retry_parameter& retry_value, Args... args) {
    int attempt = 0;
    auto start_time = std::chrono::steady_clock::now();
    if (!(retry_value.stop_after_attempt || retry_value.stop_after_delay || retry_value.stop_after_success)) {
        retry_value.stop_after_attempt = true;
        }
    while (true) {
        try {
            return func(args...);
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            ++attempt;
            if (retry_value.stop_after_attempt && attempt >= retry_value.stop_after_attempt_count) {
                break;
            }
            if (retry_value.stop_after_delay && std::chrono::steady_clock::now() >= start_time + std::chrono::seconds(retry_value.stop_after_delay_count)) {
                break;
            }
            
            if (retry_value.wait_before_retry > 0) {
                int time  = retry_value.wait_before_retry * 1000;
                if (retry_value.exponential_delay) {
                    time = retry_value.wait_before_retry * (1 << attempt) * 1000; // Exponential backoff
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(time));
            }
        }
    }
    throw std::runtime_error("Retry limit exceeded");
}

template<typename Func, typename... Args>
auto retry_async(Func func, Args... args) {
    retry_parameter retry_value;
    return retry_async(func, retry_value, args...);
}

template<typename Func, typename... Args>
auto retry_async(Func func, retry_parameter& retry_value, Args... args) {
    auto fut = std::async(std::launch::async, [func, &retry_value, args...] {
        return retry(func, retry_value, args...);
    });
    return fut;
}

#endif // RETRY_CPP_H
