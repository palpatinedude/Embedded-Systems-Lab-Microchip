    #include <avr/io.h>
    #include <util/delay.h>
    #include <avr/interrupt.h>

    // Global flags and variables
    #define AHEAD_LED PIN1_bm  // PD1 - LED that shows the robot goes straight ahead
    #define LEFT_LED PIN2_bm   // PD2 - LED to show left turn
    #define RIGHT_LED PIN0_bm  // PD0 - LED to show right turn

    #define SW_DIRECTION PIN5_bm  // PF5 - Switch to change direction (180deg)

    const int maxdeg = 360;  // Maximum degrees for the room for a 4 corners room

    #define T1_CYCLES 20  // Time interval for single conversion duration and turn around duration
    #define T2_CYCLES 40  // Time interval for free-running mode duration (2ms)

    int mode_flag = 0;  // 0 = single_conversion, 1 = free_running
    int degrees = 0;
    int reverse_mode = 0;

    void setPorts() {
        PORTD.DIR |= (AHEAD_LED | LEFT_LED | RIGHT_LED);
        // Logical '1' = LEDs OFF
        PORTD.OUT |= AHEAD_LED;
        PORTD.OUT |= LEFT_LED;
        PORTD.OUT |= RIGHT_LED;

        PORTF.DIR &= ~SW_DIRECTION;
        PORTF.PIN5CTRL |= PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc;
    }

    void setTimers() {
        TCA0.SINGLE.CNT = 0;  // Clear counter
        TCA0.SINGLE.CTRLB = 0;  // Normal mode
        TCA0.SINGLE.CTRLA = 0x7 << 1;
        TCA0.SINGLE.CTRLA |= 1;  // Enable
        TCA0.SINGLE.CMP0 = T1_CYCLES;
        TCA0.SINGLE.INTCTRL = TCA_SINGLE_CMP0_bm;  // Enable interrupts
    }

    void setADC() {
        // Initialize ADC for Free-Running mode
        ADC0.CTRLA |= ADC_RESSEL_10BIT_gc;  // 10-bit resolution
        ADC0.CTRLA &= ~ADC_FREERUN_bm;  // Single conversion start
        ADC0.CTRLA |= ADC_ENABLE_bm;  // Enable ADC

        ADC0.MUXPOS |= ADC_MUXPOS_AIN7_gc;  // Select the input pin (AIN7) right side sensor
        ADC0.DBGCTRL |= ADC_DBGRUN_bm;  // Enable Debug Mode

        // Single Conversion Mode
        ADC0.WINLT = 450;  // Set threshold lower indicates wall
        ADC0.WINHT = 550;  // Set threshold upper indicates no wall
        ADC0.INTCTRL |= ADC_WCMP_bm;  // Enable Interrupts for comparator
        ADC0.CTRLE |= 0x4;  // Interrupt when RES > WINHT OR RES < WINLT Window Comparator
        ADC0.COMMAND |= ADC_STCONV_bm;  // Take value
    }

    void ledOn(uint8_t led) {
        PORTD.OUT &= ~led;  // Logical '0' = LED ON
    }

    void ledOff(uint8_t led) {
        PORTD.OUT |= led;  // Logical '1' = LED OFF
    }

    void turnAround() {
        ADC0.CTRLA &= ~ADC_ENABLE_bm;

        // All LEDs on to indicate that a 180-degree turn is happening
        ledOn(LEFT_LED);
        ledOn(AHEAD_LED);
        ledOn(RIGHT_LED);

        // Setup timer for turnaround wait (T3_CYCLES)
        TCA0.SINGLE.CNT = 0;
        TCA0.SINGLE.CMP2 = T1_CYCLES;
        TCA0.SINGLE.INTCTRL = TCA_SINGLE_CMP2_bm;
    }

    ISR(TCA0_CMP2_vect) {
        cli();
        int flags = TCA0.SINGLE.INTFLAGS;  // Procedure to clear interrupt flag
        TCA0.SINGLE.INTFLAGS = flags;

        // All LEDs off to indicate that the 180-degree turn has ended and moving forward
        ledOff(LEFT_LED);
        ledOn(AHEAD_LED);
        ledOff(RIGHT_LED);

        ADC0.CTRLA |= ADC_ENABLE_bm;  // Enable ADC
        ADC0.CTRLA &= ~ADC_FREERUN_bm;  // Single conversion start
        ADC0.COMMAND |= ADC_STCONV_bm;  // Take value
        sei();
    }

    void stopMovement() {
        // LEDs off
        ledOff(LEFT_LED);
        ledOff(AHEAD_LED);
        ledOff(RIGHT_LED);

        // Disable timer
        TCA0.SINGLE.CTRLA &= ~1;  // Disable

        // Disable ADC
        ADC0.CTRLA &= ~ADC_ENABLE_bm;
    }

    ISR(ADC0_WCOMP_vect) {
        cli();
        int flags = ADC0.INTFLAGS;  // Get the interrupt flags
        ADC0.INTFLAGS = flags;  // Clear interrupt flags

        if (mode_flag == 1) {
            // Free running mode
            if (ADC0.RES < ADC0.WINLT) {
                _delay_ms(5);  // Simulate turning
                if (reverse_mode) {
                    ledOn(RIGHT_LED);
                    ledOff(LEFT_LED);
                    degrees -= 90;
                } else {
                    ledOn(LEFT_LED);
                    ledOff(RIGHT_LED);
                    degrees += 90;
                }
                ledOn(AHEAD_LED);
                ledOff(LEFT_LED);
                ledOff(RIGHT_LED);
            } else {
                ledOn(AHEAD_LED);
                ledOff(LEFT_LED);
                ledOff(RIGHT_LED);
            }
        }

        if (mode_flag == 0) {
            // Single conversion mode
            if (ADC0.RES > ADC0.WINHT) {
                ledOn(RIGHT_LED);
                ledOff(LEFT_LED);
                _delay_ms(5);
                if (reverse_mode)
                    degrees -= 90;
                else
                    degrees += 90;
            } else {
                ledOn(AHEAD_LED);
                ledOff(LEFT_LED);
                ledOff(RIGHT_LED);
            }
        }

        sei();
    }

    ISR(TCA0_CMP0_vect) {
        cli();
        int flags = TCA0.SINGLE.INTFLAGS;
        TCA0.SINGLE.INTFLAGS = flags;

        ADC0.MUXPOS |= ADC_MUXPOS_AIN6_gc;  // Switch to front sensor
        ADC0.CTRLA |= ADC_FREERUN_bm;  // Enable free running

        TCA0.SINGLE.CMP1 = T2_CYCLES;
        TCA0.SINGLE.INTCTRL = TCA_SINGLE_CMP1_bm;

        mode_flag = 1;
        sei();
    }

    ISR(TCA0_CMP1_vect) {
        cli();
        int flags = TCA0.SINGLE.INTFLAGS;
        TCA0.SINGLE.INTFLAGS = flags;

        if (reverse_mode == 0) {
            ADC0.MUXPOS |= ADC_MUXPOS_AIN7_gc;  // Switch back to right sensor
        } else {
            ADC0.MUXPOS |= ADC_MUXPOS_AIN5_gc;  // Switch back to left sensor
        }

        ADC0.CTRLA &= ~ADC_FREERUN_bm;
        ADC0.COMMAND |= ADC_STCONV_bm;

        TCA0.SINGLE.INTCTRL = TCA_SINGLE_CMP0_bm;
        TCA0.SINGLE.CNT = 0;

        mode_flag = 0;
        sei();
    }

    ISR(PORTF_PORT_vect) {
        cli();
        int flags = PORTF.INTFLAGS;
        PORTF.INTFLAGS = flags;

        reverse_mode = 1;
        turnAround();
        sei();
    }

    int main(void) {
        setPorts();
        setTimers();
        setADC();

        sei();
        ledOn(AHEAD_LED);

        while (1) {
            if (!reverse_mode && degrees == 360)
                stopMovement();
            else if (reverse_mode && degrees <= 0)
                stopMovement();
        }

        return 0;
    }

