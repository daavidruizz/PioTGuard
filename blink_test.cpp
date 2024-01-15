#include <iostream>
#include <wiringPi.h>
#include <csignal>

// global flag used to exit from the main loop
bool RUNNING = true;

// Blink an LED
void blink_led(int led, int time) {
    digitalWrite(led, HIGH);
    delay(100);
    digitalWrite(led, LOW);
    delay(time);
}

// Callback handler if CTRL-C signal is detected
void my_handler(int s) {
    std::cout << "Detected CTRL-C signal no. " << s << '\n';
    RUNNING = false;
}

int main() {
    // Register a callback function to be called if the user presses CTRL-C
    std::signal(SIGINT, my_handler);

    // Initialize wiringPi and allow the use of BCM pin numbering
    wiringPiSetupGpio();

    std::cout << "Controlling the GPIO pins with wiringPi\n";

    // Define the 3 pins we are going to use
    int green = 4;

    // Setup the pins
    pinMode(green, OUTPUT);

    int time = 2000;   // interval at which a pin is turned HIGH/LOW
    while(RUNNING) {
        blink_led(green, time);
    }

    std::cout << "Program ended ...\n";
}