#include <Adafruit_NeoPixel.h>

#define PIN 6

/*strips*/
//also look into using NeoMatrix libraries??
Adafruit_NeoPixel strip = Adafruit_NeoPixel(5, PIN, NEO_GRB + NEO_KHZ800);

int brightness = 25;

/*colours*/
uint32_t red = strip.Color(255, 0, 0);
uint32_t green = strip.Color(0, 255, 0);
uint32_t blue = strip.Color(0, 0, 255);
uint32_t magenta = strip.Color(255, 0, 255);
uint32_t cyan = strip.Color(0, 255, 255);
uint32_t yellow = strip.Color(255, 255, 0);
uint32_t white = strip.Color(255, 255, 255);
uint32_t webwork_green = strip.Color(136, 255, 136);
uint32_t webwork_red = strip.Color(255, 148, 148);
uint32_t off = strip.Color(0, 0, 0);

void setup() {
  strip.begin();
  strip.show(); //initialize all pixels to 'off'
}

void loop() {
  initCheckers();
}

void matWrite(x, y, colour) {
  strip.setPixelColor(8y + x, colour);
}

void initCheckers() {
  matWrite(0, 0, red);
  matWrite(2, 0, red);
  matWrite(4, 0, red);
  matWrite(6, 0, red);
  matWrite(1, 1, red);
  matWrite(3, 1, red);
  matWrite(5, 1, red);
  matWrite(7, 1, red);

  matWrite(0, 7, blue);
  matWrite(2, 7, blue);
  matWrite(4, 7, blue);
  matWrite(6, 7, blue);
  matWrite(1, 6, blue);
  matWrite(3, 6, blue);
  matWrite(5, 6, blue);
  matWrite(7, 6, blue);
}

