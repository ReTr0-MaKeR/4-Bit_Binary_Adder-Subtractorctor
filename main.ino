#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel LED_RGB(1, 48, NEO_GRB + NEO_KHZ800);


bool XOR(bool a, bool b) {
  return (a + b) % 2;
}

bool AND(bool a, bool b) {
  return a && b;
}

bool OR(bool a, bool b) {
  return a || b;
}

bool FullAdderSum(bool x, bool y, bool z) {
  return XOR(XOR(x, y), z);
}

bool FullAdderCarry(bool x, bool y, bool z) {
  return OR(AND(z, XOR(x, y)), AND(x, y));
}

bool NOT(bool a) {
  return !a;
}

bool FullSubtractorDiff(bool x, bool y, bool b) {
  return XOR(XOR(x, y), b);
}

bool FullSubtractorBout(bool x, bool y, bool b) {
  return OR(AND(NOT(x), y), AND(NOT(XOR(x, y)), b));
}

// Digits of the first number X3X2X1X0
int X0 = 11;
int X1 = 12;
int X2 = 13;
int X3 = 14;

// Digits of the first number Y3Y2Y1Y0
int Y0 = 4;
int Y1 = 5;
int Y2 = 6;
int Y3 = 7;

// Digits of the result R4R3R2R1R0
int R0 = 39;
int R1 = 38;
int R2 = 37;
int R3 = 36;
int R4 = 35;

// Mode switch pin
int modeSwitch = 8;

void setup() {
  Serial.begin(9600);
  LED_RGB.begin();
  LED_RGB.setBrightness(30);  // Set brightness (0-255)


  pinMode(X0, INPUT);
  pinMode(X1, INPUT);
  pinMode(X2, INPUT);
  pinMode(X3, INPUT);

  pinMode(Y0, INPUT);
  pinMode(Y1, INPUT);
  pinMode(Y2, INPUT);
  pinMode(Y3, INPUT);

    // Set X pins to low
  digitalWrite(X0, LOW);
  digitalWrite(X1, LOW);
  digitalWrite(X2, LOW);
  digitalWrite(X3, LOW);

  // Set Y pins to low
  digitalWrite(Y0, LOW);
  digitalWrite(Y1, LOW);
  digitalWrite(Y2, LOW);
  digitalWrite(Y3, LOW);

  pinMode(R0, OUTPUT);
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT);
  pinMode(R4, OUTPUT);

  pinMode(modeSwitch, INPUT_PULLUP);  // Assuming the mode switch is connected to ground when pressed

  digitalWrite(R0, LOW);
  digitalWrite(R1, LOW);
  digitalWrite(R2, LOW);
  digitalWrite(R3, LOW);
  digitalWrite(R4, LOW);
}

void loop() {
  bool modeButton = digitalRead(modeSwitch);
  Serial.println(modeButton);


  if (modeButton == 0) {  // Addition mode
    LED_RGB.setPixelColor(0, LED_RGB.Color(0, 255, 0));
    LED_RGB.show();

    bool x0 = digitalRead(X0);
    bool y0 = digitalRead(Y0);

    bool sum0 = FullAdderSum(x0, y0, 0);
    bool carry0 = FullAdderCarry(x0, y0, 0);

    bool x1 = digitalRead(X1);
    bool y1 = digitalRead(Y1);

    bool sum1 = FullAdderSum(x1, y1, carry0);
    bool carry1 = FullAdderCarry(x1, y1, carry0);

    bool x2 = digitalRead(X2);
    bool y2 = digitalRead(Y2);

    bool sum2 = FullAdderSum(x2, y2, carry1);
    bool carry2 = FullAdderCarry(x2, y2, carry1);

    bool x3 = digitalRead(X3);
    bool y3 = digitalRead(Y3);

    bool sum3 = FullAdderSum(x3, y3, carry2);
    bool carry3 = FullAdderCarry(x3, y3, carry2);

    digitalWrite(R0, sum0);
    digitalWrite(R1, sum1);
    digitalWrite(R2, sum2);
    digitalWrite(R3, sum3);
    digitalWrite(R4, carry3);
  } 
  else if (modeButton == 1)
   {  // Subtraction mode
    LED_RGB.setPixelColor(0, LED_RGB.Color(0, 0, 255));
    LED_RGB.show();

    bool x0 = digitalRead(X0);
    bool y0 = digitalRead(Y0);

    bool diff0 = FullSubtractorDiff(x0, y0, 0);
    bool bout0 = FullSubtractorBout(x0, y0, 0);

    bool x1 = digitalRead(X1);
    bool y1 = digitalRead(Y1);

    bool diff1 = FullSubtractorDiff(x1, y1, bout0);
    bool bout1 = FullSubtractorBout(x1, y1, bout0);

    bool x2 = digitalRead(X2);
    bool y2 = digitalRead(Y2);

    bool diff2 = FullSubtractorDiff(x2, y2, bout1);
    bool bout2 = FullSubtractorBout(x2, y2, bout1);

    bool x3 = digitalRead(X3);
    bool y3 = digitalRead(Y3);

    bool diff3 = FullSubtractorDiff(x3, y3, bout2);
    bool bout3 = FullSubtractorBout(x3, y3, bout2);

    digitalWrite(R0, diff0);
    digitalWrite(R1, diff1);
    digitalWrite(R2, diff2);
    digitalWrite(R3, diff3);
    digitalWrite(R4, bout3);
  }
}
