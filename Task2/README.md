# Task2 – Traffic Light System

## About the Project

This project extends the traffic light system from Task 1 by adding a pedestrian traffic light and a pushbutton.

The system runs on an Arduino Uno and was built in Tinkercad. A pedestrian can press the button to request a safe crossing. The car and pedestrian green lights are never on at the same time.

## Components

- 1 × Arduino Uno
- 1 × Breadboard
- 2 × Red LEDs (one car, one pedestrian)
- 1 × Yellow LED (car)
- 2 × Green LEDs (one car, one pedestrian)
- 5 × 330 Ω resistors
- 1 × Pushbutton
- Jumper wires

## Connections

| Component            | Arduino pin |
|----------------------|-------------|
| Car red LED          | D8          |
| Car yellow LED       | D9          |
| Car green LED        | D10         |
| Pedestrian red LED   | D3          |
| Pedestrian green LED | D4          |
| Pushbutton           | D2          |

Each LED is connected in series with a 330 Ω resistor. All LED cathodes connect to GND. The pushbutton connects **D2 to GND** when pressed. The code uses `INPUT_PULLUP`, so the button does not need an external resistor.

## How It Works

Without a button press, the car traffic lights repeat this cycle:

| Car state  | Duration   | Pedestrian light |
|------------|------------|------------------|
| RED        | 10 seconds | Red              |
| RED_YELLOW | 5 seconds  | Red              |
| GREEN      | 10 seconds | Red              |
| YELLOW     | 5 seconds  | Red              |

When the pedestrian presses the button:

1. Arduino detects the button press through an interrupt on D2.
2. The program remembers the crossing request.
3. If the car is green, it first completes its 10-second green period and then its 5-second yellow period.
4. When it is safe, car red turns on and pedestrian green turns on for 10 seconds.
5. Pedestrian red turns on again, and the normal traffic light cycle restarts at RED.

If cars are already in **RED** or **RED_YELLOW**, the request is handled promptly by switching to the pedestrian crossing state.

**Safety rule:** Car green and pedestrian green must never be on together.

## Arduino Code

The code uses three classes:

- **LED:** Switches individual LEDs on and off.
- **PedestrianButton:** Detects the pushbutton interrupt and stores a pending press.
- **TrafficLightController:** Manages the traffic light states and crossing requests.

The five states are `RED`, `RED_YELLOW`, `GREEN`, `YELLOW`, and `PEDESTRIAN_GREEN`.

The program uses `switch-case` to manage state changes, `millis()` for timing without `delay()`, and `attachInterrupt()` to detect button presses. The interrupt handler records a request; LED switching is done by the controller. A 50 ms debounce check is included.

## Simulation & Testing

The Arduino code was tested successfully in Tinkercad. The traffic lights follow the correct sequence with the required timings.

The pedestrian button was tested during the car GREEN state. After pressing the button, the Arduino detects the request using an interrupt. The car completes its minimum 10-second GREEN period and 5-second YELLOW period before switching to RED and allowing the pedestrian GREEN light to turn on.

The pedestrian GREEN light stays on for 10 seconds before returning to RED, and the normal traffic light cycle continues.

The test confirmed that the pedestrian request works correctly and that the car and pedestrian GREEN lights do not turn on at the same time.