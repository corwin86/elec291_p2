#include <Adafruit_NeoPixel.h>

#define PIN 6

/*strips*/
//also look into using NeoMatrix libraries??
int n = 64;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(n, PIN, NEO_GRB + NEO_KHZ800);

/*brightness*/
int brightness = 25; //!!! DO NOT GO OVER 25 WHEN USING ARDUINO POWER ONLY !!!

/*colours*/
uint32_t red = strip.Color(255, 0, 0);
uint32_t green = strip.Color(0, 255, 0);
uint32_t blue = strip.Color(0, 0, 255);
uint32_t magenta = strip.Color(255, 0, 255);
uint32_t cyan = strip.Color(0, 255, 255);
uint32_t yellow = strip.Color(255, 255, 0);
uint32_t white = strip.Color(255, 255, 255);
uint32_t off = strip.Color(0, 0, 0);

//uint16_t n = strip.numPixels(); -> determines number of pixels in strip

void setup() {
  strip.begin();
  strip.show(); //initialize all pixels to 'off'
  strip.setBrightness(25);

}

int c = 0;
//connect 4 strategy: https://www.quora.com/What-is-the-winning-strategy-for-the-first-player-in-Connect-Four-games
void loop() {
  //to set the colour of a pixel, either set RGB, or pre-defined colour
  //n is the LED number (0 to n-1), red/green/blue from 0 to 255
  //strip.setPixelColor(n, red, green, blue); or,
  //strip.setPixelColor(n, color);

  int i;
  for(i = 0; i < n; i++) {
    strip.setPixelColor(i, off);
  }
  strip.show();
  delay(500);

  for(i = 0; i < n; i++) {
    uint32_t color;
    switch (i % (4 - c)) {
      case 0 : {
        color = cyan;
        break;
      }
      case 1 : { 
        color = yellow;
        break;
      }
      case 2 : {
        color = red;
        break;
      }
      default : {
        color = blue;
      }
    }
    strip.setPixelColor(i, color);
  }
  
  strip.show();
  delay(500);
  c = c == 2 ? 0 : c + 1;
}
