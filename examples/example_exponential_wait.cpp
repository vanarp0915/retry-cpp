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
    // Setting the retry parameter to expoentially attempt retry after for a factor 2.
    retry_param.exponential_delay = true ;
    retry_param.exponential_delay_factor = 1.1;
    retry_param.stop_after_success = true; 
    retry_param.wait_before_retry = 1;

    try {
        int result = retry(exampleFunction,retry_param, 3, 2);

    } catch (const std::exception& e) {
        std::cerr << "Failed: " << e.what() << std::endl;
    }

    return 0;
}
