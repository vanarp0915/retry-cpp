#include <iostream>
#include <chrono>
#include <thread>
#include <stdexcept>
#include "retry-cpp.h"
#include <unistd.h> 

// Example function to be passed as argument
int exampleFunction(int a, int b) {
    // throw std::runtime_error("Maximum number of attempts reached");
    std::cout << "SOUNDS LIKE YOUR PROBLEM "<<std::endl;
    return a + b;
}



int main() {
    // Using the template function with the example function
    retry_parameter retry_param;
    // retry_param.stop_after_delay = true;
    retry_param.stop_after_attempt_count = 15;
    retry_param.wait_before_retry = 1;

    try {
        auto result = retry_async(exampleFunction,retry_param, 3, 2);
        for (int i =0; i <3; i++){
            std::cout << "attemp 1" <<std::endl;
            usleep(1000000);
        }
        std::cout << "Result: " << result.get() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Failed: " << e.what() << std::endl;
    }

    return 0;
}
