


#include <avr/io.h>
#include <avr/interrupt.h>

// Fan states
#define FAN_OFF     0
#define FAN_ON1     1  // Initial run
#define FAN_ON2     2  // Faster blade
#define LED_BLADE PIN0_bm  // PORTD PIN0 (LED for blade)
#define LED_BASE  PIN1_bm  // PORTD PIN1 (LED for base)
#define SWITCH    PIN6_bm  // PORTF PIN6 (Switch for fan control)

 uint8_t fan_state = FAN_OFF;

// Initialize LED and switch ports
void init_ports() {
   	PORTD.DIR = LED_BASE | LED_BLADE;
   	PORTD.OUT |= LED_BLADE | LED_BASE;  // Active-low: LEDs OFF initially

   	// Switch as input with pull-up and interrupt on both edges
   	PORTF.DIR &= ~SWITCH;
    
    PORTF.PIN6CTRL = PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc;  // Switch 6 (PF5) with pull-up and interrupt
}

// Initialize PWM in split mode
void init_pwm() {
    TCA0.SPLIT.CTRLD = TCA_SPLIT_SPLITM_bm;                    // Enable split mode
    TCA0.SPLIT.CTRLA = TCA_SPLIT_CLKSEL_DIV64_gc;              // Faster clock for better resolution

    // Blade (LOW byte): 1ms period, 60% duty (PER=51, CMP=31)
    TCA0.SPLIT.LPER = 51;
    TCA0.SPLIT.LCMP0 = 31;
	
    // Base (HIGH byte): 2ms period, 40% duty (PER=103, CMP=41)
    TCA0.SPLIT.HPER = 103;
    TCA0.SPLIT.HCMP0 = 41;

    // Enable both channels
    TCA0.SPLIT.CTRLB = TCA_SPLIT_LCMP0EN_bm | TCA_SPLIT_HCMP0EN_bm;

    // Enable interrupts
    TCA0.SPLIT.INTCTRL = TCA_SPLIT_LUNF_bm | TCA_SPLIT_HUNF_bm;
}

// Update blade pulse for faster rotation (0.5ms, 50% duty)
void update_blade_fast() {
	 TCA0.SPLIT.LPER = 25;     // 0.5ms
    TCA0.SPLIT.LCMP0 = 13;    // 50% duty
}

// Start fan PWM
void start_pwm() {
    TCA0.SPLIT.CTRLA |= TCA_SPLIT_ENABLE_bm;
}

// Stop fan PWM
void stop_pwm() {
    TCA0.SPLIT.CTRLA &= ~TCA_SPLIT_ENABLE_bm;
    PORTD.OUT = LED_BLADE | LED_BASE;  // Turn off LEDs
}

// Interrupt: switch press on PF5
ISR(PORTF_PORT_vect) {
    int flags = PORTF.INTFLAGS;  // Read interrupt flag
    PORTF.INTFLAGS = flags;      // Clear interrupt flag

    switch (fan_state) {
        case FAN_OFF:
            init_pwm();
            start_pwm();
            fan_state = FAN_ON1;
            break;

        case FAN_ON1:
            update_blade_fast();
            fan_state = FAN_ON2;
            break;

        case FAN_ON2:
            stop_pwm();
            fan_state = FAN_OFF;
            break;
    }
}

// ISR: Toggle base LED (low byte period)
ISR(TCA0_LUNF_vect) {
	TCA0.SPLIT.INTFLAGS = TCA_SPLIT_LUNF_bm;
	//int flags = TCA0.SPLIT.INTFLAGS;
	//TCA0.SPLIT.INTFLAGS = flags;

	// Base LED ON, Blade LED OFF (active-low)
	PORTD.OUT &= ~LED_BASE;   // ON
	PORTD.OUT |= LED_BLADE;   // OFF
}

// ISR: Toggle blade LED (high byte period)
ISR(TCA0_HUNF_vect) {
	TCA0.SPLIT.INTFLAGS = TCA_SPLIT_HUNF_bm;
	//int flags = TCA0.SPLIT.INTFLAGS;
	//TCA0.SPLIT.INTFLAGS = flags;

	// Blade LED ON, Base LED OFF (active-low)
	PORTD.OUT &= ~LED_BLADE;  // ON
	PORTD.OUT |= LED_BASE;    // OFF
}
int main() {
    init_ports();
    sei();  // Enable global interrupts

    while (1) {
        // Fan is controlled via interrupts
    }
} 