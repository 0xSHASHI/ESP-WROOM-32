#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>
#include <math.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// -------- Keypad Setup --------
const byte ROWS = 4; 
const byte COLS = 4; 
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {32, 33, 25, 26}; // change as per wiring
byte colPins[COLS] = {27, 14, 12, 13}; // change as per wiring
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// -------- Graph Setup --------
float x_min = -10, x_max = 10;
float y_min = -5, y_max = 5;

// Convert math coordinate to OLED pixel
int mapX(float x) {
  return (int)((x - x_min) * SCREEN_WIDTH / (x_max - x_min));
}
int mapY(float y) {
  return (int)(SCREEN_HEIGHT - ((y - y_min) * SCREEN_HEIGHT / (y_max - y_min)));
}

// Function selector
float func(float x, int choice) {
  switch(choice) {
    case 1: return x;             // y = x
    case 2: return x * x;         // y = x^2
    case 3: return sin(x);        // y = sin(x)
    default: return 0;
  }
}

void drawAxes() {
  // X-axis
  int y0 = mapY(0);
  display.drawLine(0, y0, SCREEN_WIDTH, y0, SSD1306_WHITE);

  // Y-axis
  int x0 = mapX(0);
  display.drawLine(x0, 0, x0, SCREEN_HEIGHT, SSD1306_WHITE);
}

void plotGraph(int choice) {
  display.clearDisplay();
  drawAxes();

  for (int px = 0; px < SCREEN_WIDTH; px++) {
    float x = x_min + (px * (x_max - x_min) / SCREEN_WIDTH);
    float y = func(x, choice);

    int py = mapY(y);
    if (py >= 0 && py < SCREEN_HEIGHT) {
      display.drawPixel(px, py, SSD1306_WHITE);
    }
  }
  display.display();
}

void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 20);
  display.println("Press 1-3 for func");
  display.display();
}

void loop() {
  char key = keypad.getKey();
  if (key) {
    int choice = key - '0'; // convert char '1' -> int 1
    if (choice >= 1 && choice <= 3) {
      plotGraph(choice);
    }
  }
}
