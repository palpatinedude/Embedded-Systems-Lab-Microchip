Laboratory Exercise 01: Elevator Simulation

Objective: Simulate the operation of an elevator with 3 levels (Ground Floor, First Floor, Second Floor) using LEDs to indicate the elevator's position and buttons to control its movement.

Description:
The elevator can move between three floors (Ground, First, and Second) with the following behavior:

    The elevator starts on the ground floor (all LEDs off).

    The up button moves the elevator one level up, and the down button moves it one level down.

    LEDs indicate the floor where the elevator is located (0 LEDs = Ground, 1 LED = First Floor, 2 LEDs = Second Floor).

    If both buttons are pressed simultaneously, an error LED blinks for 10ms.

The elevator responds to button presses using interrupts to handle button events efficiently.

Key Concepts:

    Interrupt Handling: Both buttons (Up and Down) trigger interrupts, and the program handles these interrupts to move the elevator.

    LED Control: LEDs indicate the current floor.

    Error Handling: When both buttons are pressed, an error condition is triggered.
