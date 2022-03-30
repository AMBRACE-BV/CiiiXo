#include <FastLED.h>


/******************LEDs*******************/
#define NUM_LEDS 3
#define PIN_LED_DATA 5
CRGB leds[NUM_LEDS];


void flashLed(int index, const struct CRGB& color, int delay_ms = 500)
{
    leds[index] = color;
    FastLED.show();
    delay(delay_ms);
    leds[index] = CRGB::Black;
    FastLED.show();
}

void setLed(int index, const struct CRGB& color) {
    leds[index] = color;
    FastLED.show();
}

void setupLeds()
{
    FastLED.addLeds<NEOPIXEL, PIN_LED_DATA>(leds, NUM_LEDS);
#ifdef LOCAL_DEBUG
    Serial.println("Added the leds");
#endif
}


