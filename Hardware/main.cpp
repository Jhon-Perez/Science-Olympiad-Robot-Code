#include <pigpio.h>
#include <iostream>
#include <unistd.h>

// Download library:
// sudo apt install pigpio

// Compile with:
// g++ -o motor_control motor_control.cpp -lpigpio -lrt -lpthread

#define IN1 17
#define IN2 18
#define IN3 22
#define IN4 23

void setup() {
    if (gpioInitialise() < 0) {
        std::cerr << "GPIO Initialization failed." << std::endl;
    }

    gpioSetMode(IN1, PI_OUTPUT);
    gpioSetMode(IN2, PI_OUTPUT);
    gpioSetMode(IN3, PI_OUTPUT);
    gpioSetMode(IN4, PI_OUTPUT);
}

// PWM range is 0-255
//! When testing make sure to start with low speed then start to make
//! the speed higher testing out what is the maximum speed that the motor can handle
//! if 255 is too much voltage
void moveMotor(int motor1_speed, int motor2_speed) {
    if (motor1_speed > 0) {
        gpioPWM(IN1, motor1_speed);
        gpioPWM(IN2, 0);
    } else {
        gpioPWM(IN1, 0);
        gpioPWM(IN2, -motor1_speed);
    }

    if (motor2_speed > 0) {
        gpioPWM(IN3, motor2_speed);
        gpioPWM(IN4, 0);  
    } else {
        gpioPWM(IN3, 0);
        gpioPWM(IN4, -motor2_speed);
    }
    
}

int main() {
    setup();

    // Use PID loop to control the speed of the motors
    moveMotor(200, 200);

    // delay 5 seconds
    usleep(5000000);

    moveMotor(0, 0);

    // Release GPIO resources
    gpioTerminate(); 
    return 0;
}
