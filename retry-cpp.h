#ifndef RETRY_CPP_H
#define RETRY_CPP_H

#include <iostream>
#include <chrono>
#include <thread>
#include <stdexcept>
#include <future>
#include <random>

struct retry_parameter {
    bool stop_after_attempt = false;
    bool stop_after_delay = false;
    bool stop_after_success = false;
    bool exponential_delay = false;
    int stop_after_attempt_count = 10;
    long stop_after_delay_count = 10;
    int wait_before_retry = 3;
    int exponential_delay_factor = 2;
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
    while (true) {
        try {
            return func(args...);
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            if (retry_value.wait_before_retry > 0) {
                if (retry_value.exponential_delay) {
                    int wait_time = retry_value.wait_before_retry * (1 << attempt); // Exponential backoff
                    std::this_thread::sleep_for(std::chrono::seconds(wait_time));
                } else {
                    std::this_thread::sleep_for(std::chrono::seconds(retry_value.wait_before_retry));
                }
            }
        }
        if (!(retry_value.stop_after_attempt || retry_value.stop_after_delay || retry_value.stop_after_success)) {
            retry_value.stop_after_attempt = true;
        }
        if (retry_value.stop_after_success || (retry_value.stop_after_attempt && attempt >= retry_value.stop_after_attempt_count)) {
            break;
        }
        if (retry_value.stop_after_delay && std::chrono::steady_clock::now() >= start_time + std::chrono::seconds(retry_value.stop_after_delay_count)) {
            break;
        }
        ++attempt;
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
