#include "Motor.h"
#include "AsynchronousDelayer.h"

#ifndef ELECTRIC_CYLINDER
#define ELECTRIC_CYLINDER

class ElectricCylinder
{
    private:
        AsynchronousDelayer delayer;
        Motor motor;
        int course_delay;

    public:
        void setup(uint8_t, uint8_t, int);
        void open();
        void close();
        void handle();
};

void ElectricCylinder::setup(uint8_t motor_pin1, uint8_t motor_pin2, int course_delay)
{
    this -> course_delay = course_delay;
    motor.setup(motor_pin1, motor_pin2);
}

void ElectricCylinder::open()
{
    motor.moveForward();
    delayer.restartTimer(course_delay);
}

void ElectricCylinder::close()
{
    motor.moveBackward();
    delayer.restartTimer(course_delay);
}

void ElectricCylinder::handle()
{
    if (delayer.hasElapsed()) motor.stop();
}

#endif