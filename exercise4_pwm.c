
/*
    this code controls a fan using a button and LEDs to show status

    - fan has three states: off, on (slow), on (fast)
    - pressing the button cycles through these states
    - PWM controls blade and base speeds using split timer mode
    - LEDs indicate which part (blade/base) is active
    - everything is handled using interrupts (button press and PWM underflows)
*/

/*

#include <avr/io.h>
#include <avr/interrupt.h>

// fan states
#define FAN_OFF     0
#define FAN_ON1     1  // initial run
#define FAN_ON2     2  // faster blade

// pin definitions
#define LED_BLADE PIN0_bm  // PORTD PIN0 (LED for blade)
#define LED_BASE  PIN1_bm  // PORTD PIN1 (LED for base)
#define SWITCH    PIN6_bm  // PORTF PIN6 (switch for fan control)

uint8_t fan_state = FAN_OFF;  // stores current fan state

// set up LEDs as output and switch as input with interrupt
void init_ports() {
	PORTD.DIR = LED_BASE | LED_BLADE;  // configure LEDs as outputs
	PORTD.OUT |= LED_BLADE | LED_BASE;  // turn both LEDs off (active-low)

	PORTF.DIR &= ~SWITCH;  // configure switch pin as input
	PORTF.PIN6CTRL = PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc;  // enable pull-up and both edge interrupt
}

// initialize PWM in split mode for blade and base
void init_pwm() {
	TCA0.SPLIT.CTRLD = TCA_SPLIT_SPLITM_bm;  // enable split mode
	TCA0.SPLIT.CTRLA = TCA_SPLIT_CLKSEL_DIV64_gc;  // set clock prescaler to 64

	// configure blade PWM: 1ms period, 60% duty cycle
	TCA0.SPLIT.LPER = 51;       // set period to 1ms
	TCA0.SPLIT.LCMP0 = 31;      // set duty cycle to 60%

	// configure base PWM: 2ms period, 40% duty cycle
	TCA0.SPLIT.HPER = 103;      // set period to 2ms
	TCA0.SPLIT.HCMP0 = 41;      // set duty cycle to 40%

	// enable both compare channels
	TCA0.SPLIT.CTRLB = TCA_SPLIT_LCMP0EN_bm | TCA_SPLIT_HCMP0EN_bm;

	// enable underflow interrupts for both timers
	TCA0.SPLIT.INTCTRL = TCA_SPLIT_LUNF_bm | TCA_SPLIT_HUNF_bm;
}

// change blade PWM to faster mode: 0.5ms period, 50% duty cycle
void update_blade_fast() {
	TCA0.SPLIT.LPER = 25;     // set period to 0.5ms
	TCA0.SPLIT.LCMP0 = 13;    // set duty cycle to 50%
}

// start PWM generation
void start_pwm() {
	TCA0.SPLIT.CTRLA |= TCA_SPLIT_ENABLE_bm;  // enable the timer
}

// stop PWM generation and turn off LEDs
void stop_pwm() {
	TCA0.SPLIT.CTRLA &= ~TCA_SPLIT_ENABLE_bm;  // disable the timer
	PORTD.OUT = LED_BLADE | LED_BASE;  // turn off both LEDs (active-low)
}

// switch toggles fan states
ISR(PORTF_PORT_vect) {
	int flags = PORTF.INTFLAGS;     // read interrupt flags
	PORTF.INTFLAGS = flags;         // clear interrupt flags

	switch (fan_state) {
		case FAN_OFF:
		init_pwm();             // set up PWM
		start_pwm();            // start PWM
		fan_state = FAN_ON1;    // move to first on state
		break;

		case FAN_ON1:
		update_blade_fast();    // increase blade speed
		fan_state = FAN_ON2;    // move to second on state
		break;

		case FAN_ON2:
		stop_pwm();             // stop PWM and turn off LEDs
		fan_state = FAN_OFF;    // move to off state
		break;
	}
}

// blade PWM period complete, toggle base LED
ISR(TCA0_LUNF_vect) {
	TCA0.SPLIT.INTFLAGS = TCA_SPLIT_LUNF_bm;  // clear underflow flag

	PORTD.OUT &= ~LED_BASE;   // turn base LED on (active-low)
	PORTD.OUT |= LED_BLADE;   // turn blade LED off
}

// base PWM period complete, toggle blade LED
ISR(TCA0_HUNF_vect) {
	TCA0.SPLIT.INTFLAGS = TCA_SPLIT_HUNF_bm;  // clear underflow flag

	PORTD.OUT &= ~LED_BLADE;  // turn blade LED on (active-low)
	PORTD.OUT |= LED_BASE;    // turn base LED off
}

// main 
int main() {
	init_ports();  // configure I/O
	sei();         // enable global interrupts

	while (1) {
		// fan control is interrupt-driven, nothing to do here
	}
}
*/

/*
    this code controls a fan using a button and LEDs to show status

    - fan has three states: off, on (slow), on (fast)
    - pressing the button cycles through these states
    - PWM controls blade and base speeds using split timer mode
    - LEDs indicate which part (blade/base) is active
    - everything is handled using interrupts (button press and PWM underflows)
*/

#include <avr/io.h>
#include <avr/interrupt.h>

// fan states
#define FAN_OFF     0
#define FAN_ON1     1  // initial run
#define FAN_ON2     2  // faster blade

// pin definitions
#define LED_BLADE PIN0_bm  // PORTD PIN0 (LED for blade)
#define LED_BASE  PIN1_bm  // PORTD PIN1 (LED for base)
#define SWITCH    PIN6_bm  // PORTF PIN6 (switch for fan control)

uint8_t fan_state = FAN_OFF;  // stores current fan state

// set up LEDs as output and switch as input with interrupt
void init_ports() {
	PORTD.DIR = LED_BASE | LED_BLADE;  // configure LEDs as outputs
	PORTD.OUT |= LED_BLADE | LED_BASE;  // turn both LEDs off (active-low)

	PORTF.DIR &= ~SWITCH;  // configure switch pin as input
	PORTF.PIN6CTRL = PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc;  // enable pull-up and both edge interrupt
}

// initialize PWM in split mode for blade and base
void init_pwm() {
	TCA0.SPLIT.CTRLD = TCA_SPLIT_SPLITM_bm;  // enable split mode
	TCA0.SPLIT.CTRLA = TCA_SPLIT_CLKSEL_DIV64_gc;  // set clock prescaler to 64

	// configure blade pwm: 1ms period, 60% duty cycle
	TCA0.SPLIT.LPER = 77;       // set period to 1ms
	TCA0.SPLIT.LCMP0 = 46;      // set duty cycle to 60%

	// configure base pwm: 2ms period, 40% duty cycle
	TCA0.SPLIT.HPER = 155;      // set period to 2ms
	TCA0.SPLIT.HCMP0 = 62;      // set duty cycle to 40%

	// enable both compare channels
	TCA0.SPLIT.CTRLB = TCA_SPLIT_LCMP0EN_bm | TCA_SPLIT_HCMP0EN_bm;

	// enable underflow interrupts for both timers
	TCA0.SPLIT.INTCTRL = TCA_SPLIT_LUNF_bm | TCA_SPLIT_HUNF_bm;
}

// change blade PWM to faster mode: 0.5ms period, 50% duty cycle
void update_blade_fast() {
	TCA0.SPLIT.LPER = 38;     // set period to 0.5ms
	TCA0.SPLIT.LCMP0 = 19;    // set duty cycle to 50%
}

// start PWM generation
void start_pwm() {
	TCA0.SPLIT.CTRLA |= TCA_SPLIT_ENABLE_bm;  // enable the timer
}

// stop PWM generation and turn off LEDs
void stop_pwm() {
	TCA0.SPLIT.CTRLA &= ~TCA_SPLIT_ENABLE_bm;  // disable the timer
	PORTD.OUT = LED_BLADE | LED_BASE;  // turn off both LEDs (active-low)
}

// switch toggles fan states
ISR(PORTF_PORT_vect) {
	int flags = PORTF.INTFLAGS;     // read interrupt flags
	PORTF.INTFLAGS = flags;         // clear interrupt flags

	switch (fan_state) {
		case FAN_OFF:
		init_pwm();             // set up PWM
		start_pwm();            // start PWM
		fan_state = FAN_ON1;    // move to first on state
		break;

		case FAN_ON1:
		update_blade_fast();    // increase blade speed
		fan_state = FAN_ON2;    // move to second on state
		break;

		case FAN_ON2:
		stop_pwm();             // stop PWM and turn off LEDs
		fan_state = FAN_OFF;    // move to off state
		break;
	}
}

// blade PWM period complete, toggle base LED
ISR(TCA0_LUNF_vect) {
	TCA0.SPLIT.INTFLAGS = TCA_SPLIT_LUNF_bm;  // clear underflow flag

	PORTD.OUT &= ~LED_BASE;   // turn base LED on (active-low)
	PORTD.OUT |= LED_BLADE;   // turn blade LED off
}

// base PWM period complete, toggle blade LED
ISR(TCA0_HUNF_vect) {
	TCA0.SPLIT.INTFLAGS = TCA_SPLIT_HUNF_bm;  // clear underflow flag

	PORTD.OUT &= ~LED_BLADE;  // turn blade LED on (active-low)
	PORTD.OUT |= LED_BASE;    // turn base LED off
}

// main 
int main() {
	init_ports();  // configure I/O
	sei();         // enable global interrupts

	while (1) {
		// fan control is interrupt-driven, nothing to do here
	}
}
