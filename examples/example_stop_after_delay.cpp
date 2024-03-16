#include <iostream>
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
    // Setting the retry parameter to stop retrying after certain amount of time. Additionally adding the wait time between each retry time
    retry_param.stop_after_delay = true;
    retry_param.stop_after_delay_count = 5;
    retry_param.wait_before_retry = 1;

    try {
        int result = retry(exampleFunction,retry_param, 3, 2);

    } catch (const std::exception& e) {
        std::cerr << "Failed: " << e.what() << std::endl;
    }

    return 0;
}
