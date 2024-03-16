# retry-cpp
A retry mechanism for the C++ users. Can be heavily used for API call functions, functions having multiple failure case parameters etc.

## Features
* Synchronous and Asynchronous retry mechanism for any type functions.
* Can specify stop conditions (i.e Stop retry after certain attempts, Stop retry after certain interval of time, Infinte retry untill the function call is success.)
* Can specify wait conditions (i.e Stop wait after certain time for the next retry, exponential wait for the next retry)

## Installaton and dependency
* Give a star to this repo (at the top)
* Clone the repository

```
git clone https://github.com/vanarp0915/retry-cpp.git
```

## Adding to CMakeLists
```
include_directories(${PROJECT_SOURCE_DIR}/YOUR_FOLDER_PATH/retry-cpp)
```

## Parameters
```
- stop_after_attempt = true     // Flag to enable the feature of stoping the retry after certain attempt.
- stop_after_delay = false      // Flag to enable the feature of stoping the retry after certain amout of time.
- stop_after_success = false    // Flag to enable the feature of stoping the retry only after the execution is success.
- exponential_delay = false     // Flag to enable the feature of stoping the retry after certain attempt.
- stop_after_attempt_count = 3  // No of times the retry has to run if stop_after_attempt is true. 
- stop_after_delay_count = 3    // How long the retry has to execute in seconds if the stop_after_delay is true.
- wait_before_retry = 1         // Time difference between each retry
- exponential_delay_factor = 2  // Exponential factor on the wait_time of each delay when exponential_delay is enabled.
```

## Examples
#### Example function to retry.
Consider a function which takes two input and retruns the sum. The function also will throw error randomly for testing.

```
int exampleFunction(int a, int b) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    int randomNum = std::rand() % 10;
    std::cout <<randomNum<<std::endl;
    if (randomNum < 5) {
        throw std::runtime_error("Random error occurred!");
    }
    std::cout << "RETURNING THE NUMBER: " << std::endl;
    return a + b;
}
```
### Synchronous retry with default parameters
In this example, a retry will occur synchronously with a default stop condition set to 3 attempts and a wait condition of 1 seconds between each retry. This configuration implies that the function will be retried a maximum of three times, with a one-second pause between each attempt.
```
#include <iostream>
#include <stdexcept>
#include "retry-cpp.h"

int main() {
    // Invoking the exampleFunction with its parameters, including retrying with default settings.
    int result = retry(exampleFunction, 3, 2);
    return 0;
}
```
### Asynchronous retry with default parameters
In this example, a retry will occur asynchronously with a default stop condition set to 3 attempts and a wait condition of 1 seconds between each retry. This configuration implies that the function will be retried a maximum of three times, with a one-second pause between each attempt.
```
#include <iostream>
#include <stdexcept>
#include "retry-cpp.h"

int main() {
    // Invoking the exampleFunction with its parameters, including asynchronous retrying with default settings.
    int result = retry_async(exampleFunction, 3, 2);
    return 0;
}
```

### Synchronous retry for 5 attempts and wait time between retry is 3 seconds.
In this example, retry will occur maximum 5 times in the interval of 15 seconds
```
#include <iostream>
#include <stdexcept>
#include "retry-cpp.h"

retry_parameter retry_param;
retry_param.stop_after_attempt = true;
retry_param.stop_after_attempt_count = 5;
retry_param.wait_before_retry = 3;

int main() {
    // Invoking the exampleFunction with its parameters, including retrying the function for 5 times and waiting time between retry is 3 seconds.
    int result = retry(exampleFunction, retry_param 3, 2);
    return 0;
}
```
### Synchronous/Asynchronous parameter retry for 5 attempts and wait time between retry is 3 seconds.
```
retry_parameter retry_param;
retry_param.stop_after_attempt = true;
retry_param.stop_after_attempt_count = 5;
retry_param.wait_before_retry = 3;
```

### Synchronous/Asynchronous parameter retry until success and wait time between retry is 3 seconds.
```
retry_parameter retry_param;
retry_param.stop_after_success = true;
retry_param.wait_before_retry = 3;
```

### Synchronous/Asynchronous parameter retry for 10 seconds and wait time between retry is 1 seconds.
In this example retry will happen 10 times within 10 seconds
```
retry_parameter retry_param;
retry_param.stop_after_delay = true;
retry_param.stop_after_delay_count = 10; 
retry_param.wait_before_retry = 1;
```

### Synchronous/Asynchronous retry parameter for until success and wait time between retry is 1 seconds with the exponential delay factor of 2.
In this example retry will happen until success. But the time difference between each retry will be double with respect to the previous one.
```
retry_parameter retry_param;
retry_param.stop_after_success = true;
retry_param.exponential_delay = true;
retry_param.exponential_delay_factor = 2;
retry_param.wait_before_retry = 1;
```
Note: All the parameters will also cater to the asynchronous retry function.
