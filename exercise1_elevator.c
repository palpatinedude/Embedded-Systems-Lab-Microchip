#include  <avr/io.h> 
#include  <avr/interrupt.h>
 #include <util/delay.h>

// Define button and LED pins
#define BTN_UP	0x20	// PF5 (Up Button) #define  BTN_DOWN	0x40	// PF6 (Down Button)

#define LED_ERROR	0x01	// PD0 (Error LED) #define LED_FLOOR1 0x02	// PD1 (Floor 1 LED) #define LED_FLOOR2 0x04	// PD2 (Floor 2 LED)

//  Floor  levels  #define FLOOR_GROUND 0
#define  FLOOR_ONE	1
#define  FLOOR_TWO	2

#define DEL 2	// Delay time in milliseconds

uint8_t currentFloor = FLOOR_GROUND; // Track current floor
volatile uint8_t buttonState = 0; // Store pressed button info ,volatile cause it's update

// Function to update LEDs based on the current floor
void updateLEDs() {
// Turn off all LEDs initially (Active-low logic)
PORTD.OUT |= (LED_ERROR | LED_FLOOR1 | LED_FLOOR2);
// Activate the LED corresponding to the current floor
if (currentFloor == FLOOR_GROUND) {
PORTD.OUT &= ~LED_ERROR;	//  Show  ground  floor  (error  LED  as  indicator)
}
else if (currentFloor == FLOOR_ONE) {
PORTD.OUT &= ~LED_FLOOR1;	// Show first floor
}
else if (currentFloor == FLOOR_TWO) {
PORTD.OUT &= ~LED_FLOOR2;	// Show second floor
}
}
// Function to process button presses and move the elevator
void processButtonPress() {
if (buttonState == (BTN_UP | BTN_DOWN)) {	// Both buttons pressed (error)
PORTD.OUT &= ~LED_ERROR; // Turn on error LED
_delay_ms(DEL);
PORTD.OUT |= LED_ERROR;	// Turn off error LED
_delay_ms(DEL);
}
else if (buttonState & BTN_UP) {	// Up button pressed
if (currentFloor < FLOOR_TWO) { currentFloor++;	// Move elevator up updateLEDs();
}
}
else if (buttonState & BTN_DOWN) {	// Down button pressed
if (currentFloor > FLOOR_GROUND) { currentFloor--;	// Move elevator down updateLEDs();
}
}
buttonState = 0;	// Reset button state after processing
}
// Interrupt Service Routine (ISR) for button detection
ISR(PORTF_PORT_vect) {
buttonState = PORTF.INTFLAGS & (BTN_UP | BTN_DOWN);	// Store which button was pre
PORTF.INTFLAGS = buttonState;	// Clear the interrupt flags
}
int main() {
// Configure LED pins as output
PORTD.DIR |= (LED_ERROR | LED_FLOOR1 | LED_FLOOR2);
PORTD.OUT |= (LED_ERROR | LED_FLOOR1 | LED_FLOOR2);	// Turn off all LEDs initiall

// Configure button pins as input with pull-up resistors
PORTF.DIR &= ~(BTN_UP | BTN_DOWN);
PORTF.PIN5CTRL |= PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc; PORTF.PIN6CTRL |= PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc;
sei();	// Enable global interrupts

while (1) {
if  (buttonState)  {	// Process only if a button was pressed
processButtonPress();
}
}
cli();	// Disable global interrupts (not really needed since loop runs forever)
return 0;
}
