#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include <avr/sleep.h>

 
#define PIN      0
#define N_LEDS 5

void nightyNight();
uint32_t Wheel(byte);
uint8_t awakeCounter = 0;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);

int wait = 200; // how long we wait on each color (milliseconds)
 
void setup() {
  // disable interrupts...
  cli();
  
  // reset the 'why the cpu was reset' flag...
  MCUSR = 0;

  // disable and clear watchdog settings a la ATtiny25/45/85 [DATASHEET] p. 46
	WDTCR = (1<<WDCE) | (1<<WDE);
	WDTCR = 0;
  
  //   // set interrupt time to about 1 second and enable...
  // WDTCR = (1<<WDIE) | (1<<WDP2) | (1<<WDP1);

  // set interrupt time to about 64ms and enable...
	WDTCR = (1<<WDIE) | (1<<WDP0);
  
  // enable sleep mode for power down...
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  
  // enable interrupts...
  sei();
  
  strip.begin();
}
 
void loop() {
  uint8_t color = 0;
  uint8_t currentPix = 0;
  uint8_t onTime = 0;
  
  for (;;) {
    nightyNight();
               
    if(awakeCounter > 220) {
      WDTCR = (1<<WDIE) | (1<<WDP2) | (1<<WDP1);
      onTime++;
      color++;
      strip.clear();
      strip.setPixelColor(currentPix, Wheel(color));
      strip.show();
      if(currentPix == 4) {
        currentPix = 0;
      }
      else {
        currentPix++;
      }
    }
    else {
      // turn off
      strip.clear();
      strip.show();
      WDTCR = (1<<WDIE) | (0<<WDP0);
    }
  }
}


// reduce power requirements and go to sleep...
void nightyNight() {
  // disable adc...
  power_adc_disable();
  // go to sleep - sleep_cpu() requires more code to enable and disable sleep...
  sleep_mode();
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}


ISR(WDT_vect) {
  awakeCounter++;
}
