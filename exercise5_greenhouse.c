
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//Global flags and variables
#define LMOI_LED PIN0_bm //PD0 - Led that shows that the moisture level is too low (needs water)
#define HMOI_LED PIN1_bm //PD1 - Led that shows that the moisture level is too high 
#define AIR_LED PIN2_bm //PD2 - Led for enabled air venting system

#define EN_WATER PIN5_bm //PF5 - Switch to enable watering system
#define EN_AIR PIN6_bm //PF6 -Switch to enable air venting system

int moist_level;
int watering_system = 0;
int air_venting_system = 0;
int watering_period;
int pwmCNT = 0;

void setPorts(){
	//Leds
	PORTD.DIR |= (LMOI_LED | HMOI_LED | AIR_LED);
	
	//Logical '1' = Leds OFF
	PORTD.OUT |=LMOI_LED;
	PORTD.OUT |=HMOI_LED;
	PORTD.OUT |=AIR_LED;
	
	//Switches
	PORTF.DIR &= ~EN_WATER;
	PORTF.DIR &= ~EN_AIR;
	
	//Enable interrupts 
	PORTF.PIN5CTRL |= PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc;
	PORTF.PIN6CTRL |= PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc;
	
}

void setADC(){
	//Initialize ADC
	ADC0.CTRLA |= ADC_RESSEL_10BIT_gc; //10-bit resolution
	ADC0.CTRLA &= ~ADC_FREERUN_bm; // Single conversion start
	ADC0.CTRLA |= ADC_ENABLE_bm; //Enable ADC
	ADC0.MUXPOS |= ADC_MUXPOS_AIN7_gc; //Select the input pin (AIN7)right side sensor s
	ADC0.DBGCTRL |= ADC_DBGRUN_bm; //Enable Debug Mode
	
	// Single Conversion Mode
	ADC0.WINLT = 15; // Set threshold lower that shows the need for water 
	ADC0.WINHT = 30; //Set threshold upper that shows the need for air 
	ADC0.INTCTRL |= ADC_WCMP_bm; // Enable Interrupts for comparator
	ADC0.CTRLE |= 0x4; // Interrupt when RES > WINHT OR RES < WINLT Window Comparator
	
	ADC0.COMMAND |= ADC_STCONV_bm; // Take value
}

void setWater_Timer(){
	 TCA0.SINGLE.CNT = 0;
	 TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_NORMAL_gc;
	 TCA0.SINGLE.CMP0 = watering_period;
	 TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1024_gc |
	 TCA_SINGLE_ENABLE_bm;
	 TCA0.SINGLE.INTCTRL = TCA_SINGLE_CMP0_bm;
}


void setAirVenting_pwm(){
	
	TCA0.SINGLE.CNT = 0;
	TCA0.SINGLE.CTRLA=TCA_SINGLE_CLKSEL_DIV1024_gc;//set clock prescaler to 1024
	
	TCA0.SINGLE.PER = 0xFF; //select the resolution
	TCA0.SINGLE.CMP1 = 0x7F; //duty cycle 50%
	
	//select Single_Slope_PWM
	TCA0.SINGLE.CTRLB |= TCA_SINGLE_WGMODE_SINGLESLOPE_gc;
	
	//enable interrupt Overflow
	TCA0.SINGLE.INTCTRL |= TCA_SINGLE_OVF_bm;
	
	//enable interrupt COMP1
	TCA0.SINGLE.INTCTRL |= TCA_SINGLE_CMP1_bm;
	TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm; //Enable

}


//Function to be used when the wrong switch is used
void wrongModeSelected(){
	//All leds ON 
	PORTD.OUT &= ~LMOI_LED;
	PORTD.OUT &= ~HMOI_LED;
	PORTD.OUT &= ~AIR_LED;
}

ISR(ADC0_WCOMP_vect) {
	int flags = ADC0.INTFLAGS;
	ADC0.INTFLAGS = flags; //clear interrupt flags
	
	// Handling the ADC result
	if (ADC0.RES < ADC0.WINLT) {
		moist_level= 1;
		watering_period = ADC0.WINLT - ADC0.RES;
		PORTD.OUT &= ~LMOI_LED; //low moist led on (needs watering)
		
		} else if (ADC0.RES > ADC0.WINHT) {
		moist_level = 2;
		PORTD.OUT &= ~HMOI_LED; //high moist led on (needs air venting)
		
	}
	
}

ISR(PORTF_PORT_vect) {
int flags = PORTF.INTFLAGS;
PORTF.INTFLAGS = flags;
	
switch (moist_level){
		
	case 1: //moist_level = 1 when the moist is low (from adc isr)
		
	if (flags & EN_WATER) {
			setWater_Timer();
			watering_system = 1;
			} else {
			wrongModeSelected(); // Wrong button pressed
			}
			
	moist_level = 0;
	break;
	
	case 2: //moist_level = 2 when the moist is high(from adc isr)
	
	if (flags & EN_AIR) {
			setAirVenting_pwm();
			//PORTD.OUT &= ~AIR_LED;
			air_venting_system = 1;
			} else {
			wrongModeSelected(); // Wrong button pressed
			}
	moist_level = 0;
	break;
	//moist_level = 0;
	}
	
	}


ISR(TCA0_CMP0_vect) {
	int flags = TCA0.SINGLE.INTFLAGS;
	TCA0.SINGLE.INTFLAGS = flags;
	
	watering_system = 0;
	
 //Led0 OFF after watering
 	PORTD.OUT |=LMOI_LED;
	 
	
}

ISR(TCA0_OVF_vect) {
	int intflags = TCA0.SINGLE.INTFLAGS;
	TCA0.SINGLE.INTFLAGS = intflags;

	
	pwmCNT++; 
	PORTD.OUT &= ~AIR_LED; //led 2 ON
	
	if (pwmCNT == 4) {
		TCA0.SINGLE.CTRLA = 0; //stop pwm
		
		 // Leds 1,2 OFF after air venting
		 PORTD.OUT |=HMOI_LED;
		 PORTD.OUT |=AIR_LED;
		 
		pwmCNT = 0;
		air_venting_system = 0;
		
	}

}


ISR(TCA0_CMP1_vect) {
	int flags = TCA0.SINGLE.INTFLAGS;
	TCA0.SINGLE.INTFLAGS = flags;
	
	PORTD.OUT |=AIR_LED; //off led 2 on falling edge
	
	
}

// main
int main() {
	setPorts();
	setADC();
	sei(); // enable global interrupts
	while (1) {
	}
}