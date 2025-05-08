   Microcontroller Exercises Overview

This repository contains a series of exercises designed to help you learn how to work with microcontrollers, control various peripherals using Pulse-Width Modulation (PWM), and manage interrupts. These exercises will guide you through the operation of timers, PWM signals, and external device management using LEDs, switches, and motors.
Exercise Breakdown
Exercise 1: LED Blink with Timer

In this exercise, you will learn the basics of timers and PWM. You will create a simple program to blink an LED at a fixed interval using a timer. This exercise demonstrates how to use a timer to generate periodic events to control an LED.
Exercise 2: Variable LED Blink with Interrupts

Building on Exercise 1, this exercise introduces interrupts to control the LED blink rate. You will modify the blink frequency of the LED using external interrupts, enabling dynamic adjustments to the timing based on user input.
Exercise 3: Servo Motor Control with PWM

In this exercise, you will learn to control the position of a servo motor using PWM. By adjusting the PWM frequency and duty cycle, you will be able to position the servo at specific angles based on user input or predefined patterns.
Exercise 4: Fan Operation Simulation

This exercise simulates the operation of a fan with two PWM-controlled LEDs representing the fan blades and base. The fan can be activated and adjusted using a switch, with varying speeds for the blade and base. The exercise also involves managing multiple PWM signals and interrupts to control the fan's behavior.
Laboratory Exercises
Laboratory Exercise 01: Elevator Simulation

Objective: Simulate the operation of an elevator with 3 levels (Ground Floor, First Floor, Second Floor) using LEDs to indicate the elevator's position and buttons to control its movement.
Description:

The elevator operates between three floors (Ground, First, and Second) and behaves as follows:

    The elevator starts on the ground floor with all LEDs off.

    The "Up" button moves the elevator one level up, while the "Down" button moves it one level down.

    LEDs indicate the elevator's current position (0 LEDs = Ground, 1 LED = First Floor, 2 LEDs = Second Floor).

    If both buttons are pressed simultaneously, an error LED blinks for 10ms.

Key Concepts:

    Interrupt Handling: Both the "Up" and "Down" buttons trigger interrupts to manage the elevator's movement.

    LED Control: LEDs represent the current floor.

    Error Handling: When both buttons are pressed simultaneously, an error condition is triggered.

Laboratory Exercise 02: Traffic Lights

Objective: Simulate the operation of a traffic light control system for an intersection with vehicle traffic, pedestrian crossings, and tram passage. The system uses LEDs to indicate the traffic light status and buttons to control pedestrian light activation.
Description:

The traffic light system operates with three primary traffic lights: Vehicle Light, Pedestrian Light, and Tram Light. It responds to external events such as pedestrian button presses and tram passages, controlling the lights in the following way:

    The Vehicle Light is green by default, allowing vehicles to pass.

    The Pedestrian Light is red by default and can be activated by pressing the pedestrian button.

    The Tram Light activates when a tram passes, turning the vehicle light red and the pedestrian light green.

System Behavior:

    Initial State: The vehicle light is green, the pedestrian light is red, and the tram light is off.

    Pedestrian Button: When pressed, the pedestrian light turns green, allowing pedestrians to cross.

    Tram Passage: When a tram passes, the vehicle light turns red and the pedestrian light turns green to allow pedestrians to cross safely.

Key Concepts:

    Interrupt Handling: The system responds to the pedestrian button and tram passage using interrupts.

    LED Control: LEDs represent the three traffic lights:

        Vehicle light: controlled by PORTD, PIN0.

        Pedestrian light: controlled by PORTD, PIN2.

        Tram light: controlled by PORTD, PIN1.

    Timers: Timers manage the duration each light stays on.

    Error Handling: The system prevents conflicts, such as the simultaneous pressing of the pedestrian button.

Laboratory Exercise 03: Smart Home Device with Movement in Space

Objective: Simulate the operation of a smart home device that moves around an empty room, traces its outline, and reacts to obstacles using sensors. The device uses an Analog to Digital Converter (ADC) to read sensor values, making decisions based on the distance to obstacles.
Description:

This exercise involves programming a device to navigate an empty room using a combination of LED indicators, sensor readings from the ADC, and switch inputs. The device reacts to obstacles by:

    Moving straight until an obstacle is detected.

    Turning left or right depending on the sensor readings.

    Reversing its path when Switch 5 is pressed.

Key Concepts:

    Analog to Digital Conversion (ADC): The ADC reads the sensor values to determine obstacle distance.

    Interrupt Handling: Interrupts handle ADC measurements, switch presses, and timer events.

    LED Control: LEDs simulate the device's movement and turning direction:

        LED1 for forward movement.

        LED0 for a right turn.

        LED2 for a left turn.

    Timers: Timers are used to manage sensor readings and movement transitions.

System Components:

    Sensors: The ADC measures the distance to obstacles.

    Switch: Switch 5 is used to reverse the device's direction.

    LEDs: Represent movement (forward, left, right).

Movement Logic:

    The device moves forward until an obstacle is detected.

    Depending on sensor readings, it turns left or right.

    If Switch 5 is pressed, the device reverses direction and retraces its path.

Laboratory Exercise 04: Fan Operation Simulation

Objective: Simulate the operation of a fan using PWM to control the rotation of the fan blades and the fan base. Each movement is represented by PWM signals controlling two LEDs. The fan's operation can be toggled and adjusted using a switch (SW6), and the blade speed can be adjusted by pressing the switch multiple times.
Description:

The fan system consists of:

    Base Movement (LED1): Simulated with a PWM pulse of a 2 ms period and a 40% duty cycle.

    Blade Movement (LED0): Simulated with a PWM pulse of a 1 ms period and a 60% duty cycle.

Fan Control via Switch (SW6):

    First Press: Starts the fan and activates both base and blade PWM signals.

    Second Press: Increases the blade speed by halving the PWM period to 0.5 ms and reducing the duty cycle to 50%.

    Third Press: Deactivates the fan by disabling both PWM signals and turning off both LEDs.

Interrupts and PWM Operation:

    PWM Signals: Control the base and blade movements.

    Interrupts: Each PWM signal triggers an interrupt at the end of its period, toggling the LEDs at the rising edge of the PWM signal.
