// fuse settings...
// avrdude -c usbtiny -p attiny85 -U lfuse:w:0xe2:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m

#include <FastLED.h>
#define NUM_LEDS 5
#define DATA_PIN 0

#include <avr/power.h>
#include <avr/sleep.h>

#define set(x) |= (1<<x)
#define clr(x) &=~(1<<x) 
#define inv(x) ^=(1<<x)

void nightyNight();
uint8_t awakeCounter = 110;

// CRGB leds[NUM_LEDS];
CRGBArray<NUM_LEDS> leds;
 
void setup() {
  // power control pin...
  DDRB set(DDB1);  
  
  // disable interrupts...
  cli();
  
  // reset the 'why the cpu was reset' flag...
  MCUSR = 0;

  // disable and clear watchdog settings a la ATtiny25/45/85 [DATASHEET] p. 46
	WDTCR = (1<<WDCE) | (1<<WDE);
	WDTCR = 0;
  
  //   // set interrupt time to about 1 second and enable...
  // WDTCR = (1<<WDIE) | (1<<WDP2) | (1<<WDP1);

  // set interrupt time to about 32ms and enable...
  WDTCR = (1<<WDIE) | (1<<WDP0);
  
  // enable sleep mode for power down...
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  
  // enable interrupts...
  sei();
  
  // FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  LEDS.addLeds<WS2812B,DATA_PIN, RGB>(leds, NUM_LEDS);
}

uint8_t ledsOn = 0;
void loop() {
  uint8_t color = 0;
  uint8_t currentPix = 0;
  uint8_t onTime = 0;
  
  uint8_t hue = 0;
  uint8_t hueDelta = 3;
  
  for (;;) {
    nightyNight();
    // digitalWrite(PORTB1, HIGH);
    // // Turn the first led red for 1 second
    // for(int dot = 0; dot < NUM_LEDS; dot++) {
    //    leds[dot] = CRGB::Red;
    //    FastLED.show();
    //    // clear this led for the next time around the loop
    //    leds[dot] = CRGB::Black;
    //    delay(30);
    // }

    // turn led pixels power on...

    if(awakeCounter == 0) {
      // turn off
      leds[0] = CRGB::Black;
      leds[1] = CRGB::Black;
      leds[2] = CRGB::Black;
      leds[3] = CRGB::Black;
      leds[4] = CRGB::Black;
      // WDTCR = (1<<WDIE) | (0<<WDP0);
      WDTCR = (1<<WDIE) | (1<<WDP0);
      FastLED.show();
      digitalWrite(PORTB1, LOW);      
    }

    if(awakeCounter > 200) {
      digitalWrite(PORTB1, HIGH);
      // WDTCR = (1<<WDIE) | (0<<WDP0);
      WDTCR = (1<<WDIE) | (1<<WDP0);
      onTime++;
      color++;
      if(currentPix == 4) {
        currentPix = 0;
      }
      else {
        currentPix++;
      }
      leds[0] = CRGB::Black;
      leds[1] = CRGB::Black;
      leds[2] = CRGB::Black;
      leds[3] = CRGB::Black;
      leds[4] = CRGB::Black;

      hue += hueDelta;
      // leds[currentPix] = CRGB::White;
      leds[currentPix].setHue(hue);
      FastLED.show();
    }
    else {
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



ISR(WDT_vect) {
  awakeCounter++;
}
