#include <Arduino.h>

#ifndef MOTOR
#define MOTOR

class Motor
{
    private:
        uint8_t pin1;
        uint8_t pin2;
    
    public:
        void setup(uint8_t, uint8_t);
        void moveForward();
        void moveBackward();
        void stop();
};

void Motor::setup(uint8_t pin1, uint8_t pin2)
{
    this -> pin1 = pin1;
    this -> pin2 = pin2;

    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);
}

void Motor::moveForward()
{
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, LOW);
}

void Motor::moveBackward()
{
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, HIGH);
}

void Motor::stop()
{
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW);
}

#endif