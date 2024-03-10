#include <iostream>
#include <chrono>
#include <thread>
#include <stdexcept>

// Example struct with default values
struct retry_parameter {
    bool stop_after_attempt = false;
    bool stop_after_delay = false;
    bool stop_after_success = false;
    int stop_after_attempt_count = 10;
    long stop_after_delay_count = 10;
    int wait_before_retry = 3;
};

// Example function to be passed as argument
int exampleFunction(int a, int b) {
    throw std::runtime_error("Maximum number of attempts reached");
    return a + b;
}

// Template function taking a function pointer, retry_parameter, and individual arguments
template<typename Func, typename... Args>
auto retry(Func func, retry_parameter& retry_value, Args... args) {
    int attempt = 0;
    auto start_time = std::chrono::steady_clock::now();

    while (true) {
        try {
            return func(args...);
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            if (retry_value.wait_before_retry > 0)
                std::this_thread::sleep_for(std::chrono::seconds(retry_value.wait_before_retry));
        }

        if (!(retry_value.stop_after_attempt) && !(retry_value.stop_after_delay) && !(retry_value.stop_after_success)){
            retry_value.stop_after_attempt = true;
        }

        if (retry_value.stop_after_success || (retry_value.stop_after_attempt && attempt >= retry_value.stop_after_attempt_count)){
            break;
        }

        if (retry_value.stop_after_delay && std::chrono::steady_clock::now() >= start_time + std::chrono::seconds(retry_value.stop_after_delay_count))

            break;

        ++attempt;
    }

    throw std::runtime_error("Retry limit exceeded");
}

int main() {
    // Using the template function with the example function
    retry_parameter retry_param;
    // retry_param.stop_after_delay = true;
    retry_param.stop_after_delay_count = 15;
    retry_param.wait_before_retry = 1;

    try {
        int result = retry(exampleFunction, retry_param, 3, 2);
        std::cout << "Result: " << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Failed: " << e.what() << std::endl;
    }

    return 0;
}
