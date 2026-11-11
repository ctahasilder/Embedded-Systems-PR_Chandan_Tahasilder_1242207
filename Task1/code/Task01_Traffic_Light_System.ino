
enum LightState {
  RED,
  RED_YELLOW,
  GREEN,
  YELLOW
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

class TrafficLightController {
private:
  LightState state;
  unsigned long stateStartTime;

  LED redLED;
  LED yellowLED;
  LED greenLED;

  void enterState() {
    stateStartTime = millis();

    switch (state) {
      case RED:
        redLED.turnOn();
        yellowLED.turnOff();
        greenLED.turnOff();
        break;

      case RED_YELLOW:
        redLED.turnOn();
        yellowLED.turnOn();
        greenLED.turnOff();
        break;

      case GREEN:
        redLED.turnOff();
        yellowLED.turnOff();
        greenLED.turnOn();
        break;

      case YELLOW:
        redLED.turnOff();
        yellowLED.turnOn();
        greenLED.turnOff();
        break;
    }
  }

public:
  TrafficLightController()
    : state(RED),
      stateStartTime(0),
      redLED(8),
      yellowLED(9),
      greenLED(10) {}

  void begin() {
    redLED.begin();
    yellowLED.begin();
    greenLED.begin();

    state = RED;
    enterState();
  }

  void update() {
    unsigned long elapsed = millis() - stateStartTime;

    switch (state) {
      case RED:
        if (elapsed >= 10000UL) {
          state = RED_YELLOW;
          enterState();
        }
        break;

      case RED_YELLOW:
        if (elapsed >= 5000UL) {
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
