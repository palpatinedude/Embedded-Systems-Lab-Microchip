  # Embedded Systems Lab – Microcontroller Exercises

## Overview

This repository contains a series of exercises designed to help you learn how to work with microcontrollers, control various peripherals using Pulse-Width Modulation (PWM), and manage interrupts. These exercises will guide you through the operation of timers, PWM signals, and external device management using LEDs, switches, and motors.

---

## Exercise Breakdown

### Exercise 1: LED Blink with Timer
- **Objective:** Learn the basics of timers and PWM by blinking an LED at a fixed interval using a timer.
- **Concepts Covered:**
  - Timer-based periodic control
  - LED toggling
  - Introduction to PWM timing logic

---

### Exercise 2: Variable LED Blink with Interrupts
- **Objective:** Modify the LED blink frequency using external interrupts.
- **Concepts Covered:**
  - External interrupt configuration
  - Dynamic adjustment of blink rate
  - User interaction via input signals

---

### Exercise 3: Servo Motor Control with PWM
- **Objective:** Control the position of a servo motor using PWM.
- **Concepts Covered:**
  - PWM frequency and duty cycle control
  - Angle control of servo motors
  - User input or pre-programmed positioning

---

### Exercise 4: Fan Operation Simulation
- **Objective:** Simulate a fan using two PWM-controlled LEDs representing fan blades and base.
- **Concepts Covered:**
  - Dual PWM channel configuration
  - Speed control via switch
  - Interrupt-driven state transitions

---

### Exercise 5: Soil Moisture Controlled Watering and Air Venting System
- **Objective:** Automatically control watering and air venting based on soil moisture sensor readings.
- **Concepts Covered:**
  - ADC window comparator interrupts for moisture level detection
  - Timer-based watering duration proportional to moisture deficit
  - PWM control for air venting system
  - Switch-based system enabling with interrupt handling
  - LED signaling for system status and errors
 
---

## Laboratory Exercises

### Laboratory Exercise 01: Elevator Simulation

- **Objective:** Simulate a 3-level elevator using LEDs and button-controlled movement.
- **Description:**
  - Starts at ground floor with all LEDs off.
  - "Up" button moves elevator one level up.
  - "Down" button moves elevator one level down.
  - 0 LEDs = Ground Floor, 1 LED = First Floor, 2 LEDs = Second Floor.
  - If both buttons are pressed at once, an error LED blinks for 10ms.
- **Key Concepts:**
  - Interrupt handling for Up/Down buttons
  - LED representation of floor levels
  - Error state detection and management

---

### Laboratory Exercise 02: Traffic Lights

- **Objective:** Control a traffic light system with vehicle, pedestrian, and tram management.
- **Description:**
  - Three lights: Vehicle (default green), Pedestrian (default red), Tram.
  - Pedestrian button press activates pedestrian crossing.
  - Tram passage triggers vehicle red + pedestrian green.
- **System Behavior:**
  - Initial: Vehicle = Green, Pedestrian = Red, Tram = Off.
  - Pedestrian press → Pedestrian = Green.
  - Tram passes → Vehicle = Red, Pedestrian = Green.
- **Key Concepts:**
  - Interrupt-driven button and tram signal handling
  - LED control via PORTD:
    - Vehicle: PORTD, PIN0
    - Tram: PORTD, PIN1
    - Pedestrian: PORTD, PIN2
  - Timer-based light duration
  - Error prevention for simultaneous events

---

### Laboratory Exercise 03: Smart Home Device with Movement in Space

- **Objective:** Simulate a smart device navigating a room and reacting to obstacles via ADC.
- **Description:**
  - Moves forward until detecting an obstacle.
  - Turns left or right based on ADC sensor readings.
  - Pressing Switch 5 makes it reverse direction.
- **Key Concepts:**
  - ADC-based obstacle detection
  - LED movement indication:
    - LED1 = Forward
    - LED0 = Right turn
    - LED2 = Left turn
  - Interrupts for ADC, switches, and timers
  - Timer-controlled motion and transitions
- **System Components:**
  - ADC for sensor input
  - Switch 5 for direction reversal
  - LEDs to simulate motion

---

### Laboratory Exercise 04: Fan Operation Simulation

- **Objective:** Simulate fan behavior with PWM-controlled LEDs for base and blade movement.
- **Fan Components:**
  - **LED1 (Base Movement):**
    - PWM: 2 ms period, 40% duty cycle
  - **LED0 (Blade Movement):**
    - PWM: 1 ms period, 60% duty cycle
- **Control via Switch (SW6):**
  - 1st Press → Start fan (both LEDs on)
  - 2nd Press → Increase blade speed (PWM: 0.5 ms period, 50% duty)
  - 3rd Press → Stop fan (both LEDs off)
- **Key Concepts:**
  - PWM-based motor simulation
  - Switch-based mode cycling
  - Interrupts on PWM edges to toggle LEDs

---

### Laboratory Exercise 05: Greenhouse Simulation

- **Objective:** Design a system that monitors soil moisture and automatically activates watering or air venting systems using LEDs and switches.
- **Description:**
  - Uses ADC to measure moisture sensor input.
  - Two LEDs indicate moisture status:
    - **Low Moisture LED (PD0):** Lights up when soil is too dry (needs watering).
    - **High Moisture LED (PD1):** Lights up when soil is too wet (needs air venting).
  - An air venting LED (PD2) indicates when air venting system is active.
  - Switches on PF5 and PF6 enable watering and air venting systems respectively.
  - Watering system runs for a period proportional to moisture deficit using a timer.
  - Air venting system uses PWM to control venting duration and intensity.
  - Incorrect switch usage triggers all LEDs as an error indicator.
- **Key Concepts:**
  - ADC with window comparator interrupts for moisture thresholds.
  - Timer-based watering with variable duration.
  - PWM for air venting control.
  - External interrupts with pull-up enabled switches.
  - Interrupt-driven LED signaling for system status and error conditions.
- **System Components:**
  - Moisture sensor input on ADC channel AIN7.
  - LEDs on PORTD pins PD0, PD1, PD2.
  - Switches on PORTF pins PF5 (watering enable), PF6 (air venting enable).
  - Timers and PWM using TCA0 peripheral.



