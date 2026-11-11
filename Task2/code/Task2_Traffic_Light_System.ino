
enum LightState {
  RED,
  RED_YELLOW,
  GREEN,
  YELLOW,
  PEDESTRIAN_GREEN
};

class LED {
private:
  int pin;
  bool isOn;

public:
  LED(int ledPin) : pin(ledPin), isOn(false) {}

  void begin() {
    pinMode(pin, OUTPUT);
    turnOff();
  }

  void turnOn() {
    digitalWrite(pin, HIGH);
    isOn = true;
  }

  void turnOff() {
    digitalWrite(pin, LOW);
    isOn = false;
  }
};

class PedestrianButton {
private:
  int pin;
  static volatile bool pressPending;

public:
  PedestrianButton(int buttonPin) : pin(buttonPin) {}

  void begin() {
    pinMode(pin, INPUT_PULLUP);
    attachInterrupt(
      digitalPinToInterrupt(pin),
      handleInterrupt,
      FALLING
    );
  }

  bool consumePress() {
    noInterrupts();
    bool pressed = pressPending;
    pressPending = false;
    interrupts();
    return pressed;
  }

  static void handleInterrupt() {
    const unsigned long DEBOUNCE_MICROS = 50000UL;
    static unsigned long lastAcceptedMicros = 0;
    static bool haveAcceptedPress = false;

    unsigned long now = micros();

    if (!haveAcceptedPress ||
        (unsigned long)(now - lastAcceptedMicros)
            >= DEBOUNCE_MICROS) {
      pressPending = true;
      lastAcceptedMicros = now;
      haveAcceptedPress = true;
    }
  }
};

volatile bool PedestrianButton::pressPending = false;

class TrafficLightController {
private:
  LightState state;
  unsigned long stateStartTime;

  LED redLED;
  LED yellowLED;
  LED greenLED;
  bool crossingRequested;
  LED pedestrianRedLED;
  LED pedestrianGreenLED;
  PedestrianButton button;

  void enterState() {
    greenLED.turnOff();
    pedestrianGreenLED.turnOff();

    stateStartTime = millis();

    switch (state) {
      case RED:
        redLED.turnOn();
        yellowLED.turnOff();
        pedestrianRedLED.turnOn();
        break;

      case RED_YELLOW:
        redLED.turnOn();
        yellowLED.turnOn();
        pedestrianRedLED.turnOn();
        break;

      case GREEN:
        redLED.turnOff();
        yellowLED.turnOff();
        pedestrianRedLED.turnOn();
        greenLED.turnOn();
        break;

      case YELLOW:
        redLED.turnOff();
        yellowLED.turnOn();
        pedestrianRedLED.turnOn();
        break;

      case PEDESTRIAN_GREEN:
        crossingRequested = false;
        button.consumePress();
        redLED.turnOn();
        yellowLED.turnOff();
        pedestrianRedLED.turnOff();
        pedestrianGreenLED.turnOn();
        break;
    }
  }

public:
  TrafficLightController()
    : state(RED),
      stateStartTime(0),
      redLED(8),
      yellowLED(9),
      greenLED(10),
      crossingRequested(false),
      pedestrianRedLED(3),
      pedestrianGreenLED(4),
      button(2) {}

  void begin() {
    redLED.begin();
    yellowLED.begin();
    greenLED.begin();
    pedestrianRedLED.begin();
    pedestrianGreenLED.begin();

    crossingRequested = false;
    state = RED;
    enterState();
    button.begin();
  }

  void update() {
    bool pressed = button.consumePress();

    if (pressed && state != PEDESTRIAN_GREEN) {
      crossingRequested = true;
    }

    unsigned long elapsed = millis() - stateStartTime;

    switch (state) {
      case RED:
        if (crossingRequested) {
          state = PEDESTRIAN_GREEN;
          enterState();
        }
        else if (elapsed >= 10000UL) {
          state = RED_YELLOW;
          enterState();
        }
        break;

      case RED_YELLOW:
        if (crossingRequested) {
          state = PEDESTRIAN_GREEN;
          enterState();
        }
        else if (elapsed >= 5000UL) {
          state = GREEN;
          enterState();
        }
        break;

      case GREEN:
        if (elapsed >= 10000UL) {
          state = YELLOW;
          enterState();
        }
        break;

      case YELLOW:
        if (elapsed >= 5000UL) {
          if (crossingRequested) {
            state = PEDESTRIAN_GREEN;
          }
          else {
            state = RED;
          }
          enterState();
        }
        break;

      case PEDESTRIAN_GREEN:
        if (elapsed >= 10000UL) {
          button.consumePress();
          state = RED;
          enterState();
        }
        break;
    }
  }
};

TrafficLightController trafficLight;

void setup() {
  trafficLight.begin();
}

void loop() {
  trafficLight.update();
}
