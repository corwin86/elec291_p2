#include <Adafruit_NeoPixel.h>

#define PIN 6

/*strips*/
//also look into using NeoMatrix libraries??
Adafruit_NeoPixel strip = Adafruit_NeoPixel(5, PIN, NEO_GRB + NEO_KHZ800);

/*brightness*/
int brightness = 25;

/*colours*/
uint32_t red = strip.Color(255, 0, 0);
uint32_t green = strip.Color(0, 255, 0);
uint32_t blue = strip.Color(0, 0, 255);
uint32_t magenta = strip.Color(255, 0, 255);
uint32_t cyan = strip.Color(0, 255, 255);
uint32_t yellow = strip.Color(255, 255, 0);
uint32_t white = strip.Color(255, 255, 255);

//uint16_t n = strip.numPixels(); -> determines number of pixels in strip

void setup() {
  strip.begin();
  strip.show(); //initialize all pixels to 'off'

}

//connect 4 strategy: https://www.quora.com/What-is-the-winning-strategy-for-the-first-player-in-Connect-Four-games
void loop() {
  //to set the colour of a pixel, either set RGB, or pre-defined colour
  //n is the LED number (0 to n-1), red/green/blue from 0 to 255
  //strip.setPixelColor(n, red, green, blue); or,
  //strip.setPixelColor(n, color);

  strip.setPixelColor(0, red); //sets colour of first pixel
  strip.setPixelColor(1, yellow); //sets colour of first pixel
  strip.setPixelColor(2, green); //sets colour of first pixel
  strip.setPixelColor(3, cyan); //sets colour of first pixel
  strip.setPixelColor(4, magenta); //sets colour of first pixel

  strip.show(); //'pushes' colour data to the strip

  strip.setBrightness(brightness); //from 0 to 255

}
