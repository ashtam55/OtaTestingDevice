#ifndef _neopixel_utils_H_
#define _neopixel_utils_H_

#define LED_PIN    15
#define LED_COUNT  25

class neopixel_utils
{
    public:
    void initNeopixel();
    void colorWipe(uint32_t color, int wait);
    void theaterChase(uint32_t color, int wait);
    void theaterChaseRainbow(int wait);
    void rainbow(int wait);
}





#endif