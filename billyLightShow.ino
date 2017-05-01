#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

Adafruit_NeoPixel strip = Adafruit_NeoPixel(29, 2, NEO_GRB + NEO_KHZ800);
uint32_t color;

int oldR, oldG, oldB;
const int changeDur = 1500;
const int fadeSpeed = 20;

void setup() {

  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code
  
  strip.begin();

  for(int i=0;i<29;i++){
    color = strip.Color(i, 255, 255, 255);
    strip.setPixelColor(i, color);
  }
  strip.show();
  oldR = 255;
  oldG = 255;
  oldB = 255;
}

void loop() {
  
  int randNum = floor(random(0, 4));
  int R, G, B;
  switch(randNum){

    case 0:
      R = 255;
      G = 0;
      B = 0;
      break;
    case 1:
      R = 0;
      G = 255;
      B = 0;
      break;
    case 2:
      R = 0;
      G = 0;
      B = 255;
      break;
    case 3:
      R = 255;
      G = 255;
      B = 0;
      break;
  }
  int CAR = (R - oldR)/(changeDur/fadeSpeed);
  int CAG = (G - oldG)/(changeDur/fadeSpeed);
  int CAB = (B - oldB)/(changeDur/fadeSpeed);
  for(int i=0;i<(changeDur/fadeSpeed);i++){
    color = strip.Color(oldR + CAR*(i+1), oldG + CAG*(i+1), oldB + CAB*(i+1));
    for(int j=0;j<29;j++){
      strip.setPixelColor(j,color);
    }
    strip.show();
    delay(fadeSpeed);
  }
  oldR = R;
  oldG = G;
  oldB = B;
  delay(500);
      
}
