#pragma once

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

#define LED_STRIP_PORT PORTC
#define LED_STRIP_DDR  DDRC
#define LED_STRIP_PIN  5

// The rgb_color struct represents the color for an 8-bit RGB LED.
// Examples:
//   Black:      (rgb_color){ 0, 0, 0 }
//   Pure red:   (rgb_color){ 255, 0, 0 }
//   Pure green: (rgb_color){ 0, 255, 0 }
//   Pure blue:  (rgb_color){ 0, 0, 255 }
//   White:      (rgb_color){ 255, 255, 255}
typedef struct rgb_color
{
  uint8_t red, green, blue;
} rgb_color;

class Ws2812b
{
public:
    Ws2812b();
    void led_strip_write(rgb_color * colors, uint16_t count);
};

