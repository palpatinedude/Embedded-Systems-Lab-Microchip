#include <avr/io.h>
#include <avr/interrupt.h>

// Define traffic light LEDs and pedestrian button
#define VEHICLE_LED PIN0_bm  // PD0 - Vehicle traffic light
#define TRAM_LED PIN1_bm     // PD1 - Tram traffic light
#define PED_LED PIN2_bm      // PD2 - Pedestrian traffic light
#define PED_BUTTON PIN5_bm   // PF5 - Pedestrian button

// Time intervals in timer ticks (based on 51.2us per tick)
#define T1_TICKS 100  // Tram activation time
#define T2_TICKS 40   // Pedestrian light activation time
#define T3_TICKS 10   // Cooldown period activation time

// Global flags
int tram_active = 0;          // Tracks if tram is active to avoid interruptions
int pedestrian_request = 0;   // Tracks if pedestrian button was pressed
int pedestrian_ready = 1;     // Ensures cooldown period before button is available again

// Function to turn ON a specific LED (active LOW)
void lightOn(uint8_t led) {
    PORTD.OUT &= ~led;  // Logical '0' = LED ON
}

// Function to turn OFF a specific LED
void lightOff(uint8_t led) {
    PORTD.OUT |= led;  // Logical '1' = LED OFF
}

// Initialize traffic lights (default state)
void initLeds() {
    lightOn(VEHICLE_LED);  // Vehicle light ON
    lightOff(TRAM_LED);    // Tram light OFF
    lightOff(PED_LED);     // Pedestrian light OFF
}

// Configure button settings
void setButton() {
    PORTD.DIR |= (VEHICLE_LED | TRAM_LED | PED_LED);  // Set LED pins as outputs
    PORTF.DIR &= ~PED_BUTTON;                        // Set pedestrian button as input
    PORTF.PIN5CTRL |= PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc;  // Enable pull-up resistor
}

// Initialize and configure timers
void setTimers() {
    TCA0.SPLIT.CTRLA = 0;  // Disable timer before configuring
    TCA0.SPLIT.CTRLD = TCA_SPLIT_SPLITM_bm;  // Enable split mode

    // Configure tram timer (T1)
    TCA0.SPLIT.HCNT = T1_TICKS;  // Set high timer counter for tram duration
    TCA0.SPLIT.INTCTRL |= TCA_SPLIT_HUNF_bm;  // Enable interrupt for tram timer

    // Configure pedestrian timer (T2)
    TCA0.SPLIT.LCMP0 = T2_TICKS;  // Set low counter for pedestrian light duration
    TCA0.SPLIT.INTCTRL |= TCA_SPLIT_LCMP0_bm;  // Enable interrupt for pedestrian timer

    // Configure cooldown timer (T3)
    TCA0.SPLIT.LCMP1 = T3_TICKS;  // Set low counter for cooldown period
    TCA0.SPLIT.INTCTRL |= TCA_SPLIT_LCMP1_bm;  // Enable interrupt for cooldown timer

    // Enable timer with prescaler (div1024 for longer intervals)
    TCA0.SPLIT.CTRLA = TCA_SPLIT_CLKSEL_DIV1024_gc | TCA_SPLIT_ENABLE_bm;
}

int main() {
    setButton();  // Configure button
    setTimers();  // Configure timers
    initLeds();   // Set default LED states
    sei();        // Enable global interrupts

    while (1) {
        // Main loop does nothing; all logic runs in interrupts
    }
}

// ########## INTERRUPT SERVICE ROUTINES (ISRs) ##########

// Tram light activation (happens when tram interval is reached)
ISR(TCA0_HUNF_vect) {
    int flags = TCA0.SPLIT.INTFLAGS;  // Read interrupt flag
    TCA0.SPLIT.INTFLAGS = flags;     // Clear interrupt flag

    tram_active = 1;  // Mark tram as active
    lightOff(VEHICLE_LED);  // Turn OFF vehicle light
    lightOn(TRAM_LED);      // Turn ON tram light
    lightOn(PED_LED);       // Turn ON pedestrian light (tram passing area)
}

// Tram has passed or pedestrian cycle ends, return to normal vehicle flow
ISR(TCA0_LCMP0_vect) {
    int flags = TCA0.SPLIT.INTFLAGS;  // Read interrupt flag
    TCA0.SPLIT.INTFLAGS = flags;     // Clear interrupt flag

    if (tram_active == 1) {
        tram_active = 0;  // Reset tram status
    }

    lightOn(VEHICLE_LED);  // Turn ON vehicle light
    lightOff(TRAM_LED);    // Turn OFF tram light
    lightOff(PED_LED);     // Turn OFF pedestrian light
}

// Cooldown period ends, allowing button press again
ISR(TCA0_LCMP1_vect) {
    int flags = TCA0.SPLIT.INTFLAGS;  // Read interrupt flag
    TCA0.SPLIT.INTFLAGS = flags;     // Clear interrupt flag

    pedestrian_ready = 1;  // Button can be pressed again
}

// Pedestrian button pressed
ISR(PORTF_PORT_vect) {
    int flags = PORTF.INTFLAGS;  // Read interrupt flag
    PORTF.INTFLAGS = flags;     // Clear interrupt flag

    // Check conditions before activating pedestrian light
    if (!tram_active && !pedestrian_request && pedestrian_ready) {
        pedestrian_request = 1;  // Mark pedestrian request

        lightOff(VEHICLE_LED);  // Turn OFF vehicle light
        lightOff(TRAM_LED);     // Turn OFF tram light
        lightOn(PED_LED);       // Turn ON pedestrian light

        pedestrian_ready = 0;  // Disable button until cooldown ends
    }
}

