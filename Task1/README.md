# Task1: Traffic Light System

## About the project
This is a simple automatic traffic light made with an Arduino Uno in Tinkercad. It uses three LEDs and follows a repeating sequence. The Arduino program uses a `switch` statement and `millis()` to control the timing.

## Traffic light sequence

| State       | Lights ON      | Duration  |
|-------------|----------------|-----------|
| RED         | Red            | 10 seconds|
| RED_YELLOW  | Red and Yellow | 5 seconds |
| GREEN       | Green          | 10 seconds|
| YELLOW      | Yellow         | 5 seconds |

The sequence repeats every 30 seconds.

## Components
- 1 Arduino Uno
- 1 breadboard
- 1 red LED, 1 yellow LED, 1 green LED
- 3 resistors (330 Ω each in the design)
- Jumper wires

## Connections

| Arduino pin | LED         |
|-------------|-------------|
| D8          | Red         |
| D9          | Yellow      |
| D10         | Green       |
| GND         | LED cathodes|

Each Arduino output pin connects to its LED through a separate resistor.

## Resistor calculations
We use 5 V from an Arduino digital output, aim for 10 mA (0.01 A) per LED, and assume these LED forward voltages:

- Red: 1.8 V
- Yellow: 2.1 V
- Green: 2.2 V

## Formula: `R = (5 V − LED forward voltage) / 0.01 A`

| LED    | Calculation      | Required resistance | Chosen resistance |
|--------|------------------|---------------------|-------------------|
| Red    | (5 − 1.8) / 0.01 | 320 Ω               | 330 Ω             |
| Yellow | (5 − 2.1) / 0.01 | 290 Ω               | 330 Ω             |
| Green  | (5 − 2.2) / 0.01 | 280 Ω               | 330 Ω             |

**Expected current with 330 Ω:**
- Red: `(5 − 1.8) / 330 = 9.70 mA`
- Yellow: `(5 − 2.1) / 330 = 8.79 mA`
- Green: `(5 − 2.2) / 330 = 8.48 mA`

These calculations are estimates based on assumed LED voltages.

## Diagrams
The class diagram shows two classes: `TrafficLightController` and `LED`. The `LightState` enumeration contains the four traffic light states. The state machine diagrams explain how the controller switches states and how each LED turns ON or OFF.

## Arduino program
The code is in `Task01_Traffic_Light_System.ino`.

- `LED` controls each light with `turnOn()` and `turnOff()`.
- `TrafficLightController` manages the sequence.
- `switch` selects the current state.
- `millis()` is used to measure the time.

## Simulation screenshots
The screenshots show the expected lights at different simulation times:

| Time shown | Expected state |
|------------|----------------|
| 5 seconds  | RED            |
| 11 seconds | RED_YELLOW     |
| 16 seconds | GREEN          |
| 26 seconds | YELLOW         |
