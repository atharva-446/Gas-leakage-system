#include <reg52.h>

sbit gas_sensor = P1^0;   // gas sensor digital output -> P1.0 (pin 1)
sbit buzzer     = P1^1;   // buzzer transistor base drive -> P1.1 (pin 2)
sbit led        = P1^2;   // optional LED indicator -> P1.2

void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for(i=0; i<ms; i++)
        for(j=0; j<127; j++);   // approx ~1 ms @ 12 MHz (tune if needed)
}

void beep_for_seconds(unsigned int seconds) {
    unsigned int k;
    // simple beep pattern: short on-off repeatedly for 'seconds'
    for(k = 0; k < seconds; k++) {
        buzzer = 1;    // turn buzzer ON
        delay_ms(200);
        buzzer = 0;    // OFF
        delay_ms(200);
    }
}

void main(void) {
    // initial states
    buzzer = 0;
    led = 0;

    // Ensure pins are input/output as needed (8051 ports default to quasi-bidirectional)
    // Main loop
    while(1) {
        if (gas_sensor == 1) {      // gas detected (digital high)
            led = 1;                // optional visual indicator
            // Debounce: require signal stable for short time (avoid false triggers)
            delay_ms(50);
            if (gas_sensor == 1) { // still high => confirmed
                // Sound buzzer for 5 seconds (you can change)
                beep_for_seconds(5);   // parameter is seconds (approx)
            }
            led = 0;
            // After sounding, wait until gas clears to avoid immediate retrigger:
            while(gas_sensor == 1) {
                delay_ms(100);
            }
            // short delay after clearing
            delay_ms(500);
        }
        else {
            // idle
            buzzer = 0;
            led = 0;
        }
    }
}
