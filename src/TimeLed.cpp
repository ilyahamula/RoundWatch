#include "TimeLed.h"

#define LED_COUNT 9

TimeLed::TimeLed(const uint8_t pin)
    : strip(LED_COUNT, pin, NEO_GRB + NEO_KHZ400)
{

}