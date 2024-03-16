#include <iostream>
#include <chrono>
#include <thread>
#include <stdexcept>
#include "retry-cpp.h"
#include <unistd.h> 


retry_parameter retry_param;

int exampleFunction(int a, int b) {
    // Function to randomly throw error 
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    int randomNum = std::rand() % 10;
    std::cout <<randomNum<<std::endl;

    if (randomNum < 5) {
        throw std::runtime_error("Random error occurred!");
    }
    std::cout << "RETURNING THE NUMBER: " << std::endl;
    return a + b;
}


int main() {
    // Make the retry asynchronously. 
    retry_param.stop_after_attempt = true;
    retry_param.stop_after_attempt = 5;
    retry_param.wait_before_retry = 1;

    try {
        std::cout << "Calling the retry function " <<std::endl;
        auto result = retry_async(exampleFunction,retry_param, 3, 2);
        std::cout << "Print after Calling the rerty" <<std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Failed: " << e.what() << std::endl;
    }

    return 0;
}
