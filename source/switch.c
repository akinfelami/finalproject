#include <MKL46Z4.h>
#include "switch.h"

/* ----------------------------------------------------------------------
 Note the different characters around the library names.
 The <> characters are used to include system libraries
 The "" characters are used to include your own libraries
 ------------------------------------------------------------------------*/


const int RED_LED_PIN = 29;
const int SWITCH_1_PIN = 3;
SIM_Type* global_SIM = SIM;
PORT_Type* global_PORTE = PORTE;
GPIO_Type* global_PTE = PTE;
PORT_Type* global_PORTC = PORTC;
GPIO_Type* global_PTC = PTC;

// IRQ Handler for switch
void PORTC_PORTD_IRQHandler(void) {
	PTE->PTOR = GPIO_PTOR_PTTO(1 << RED_LED_PIN); // button pressed, toggle LED
	PORTC->PCR[SWITCH_1_PIN] |= PORT_PCR_ISF(1);  // clear the interrupt status flag by writing 1 to it
}

/*
 Main program: entry point
 */
void operate_switch_interrupts(void) {
	// setup variables so we can see them in debugger
	// if you get rid of this it seems the compiler just optimizes the variables away
	// this is for educational purposes
	global_SIM = global_SIM;
	global_PORTE = global_PORTE;
	global_PTE = global_PTE;
	global_PORTC = global_PORTC;
	global_PTC = global_PTC;

	// setup red led
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK; //Enable the clock to port E
	PORTE->PCR[RED_LED_PIN] = PORT_PCR_MUX(0b001); //Set up PTE29 as GPIO
	PTE->PDDR |= GPIO_PDDR_PDD(1 << RED_LED_PIN); // make it output
	PTE->PSOR |= GPIO_PSOR_PTSO(1 << RED_LED_PIN); // turn off LED

	// setup switch 1
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK; //Enable the clock to port C
	PORTC->PCR[SWITCH_1_PIN] &= ~PORT_PCR_MUX(0b111); // Clear PCR Mux bits for PTC3
	PORTC->PCR[SWITCH_1_PIN] |= PORT_PCR_MUX(0b001); // Set up PTC3 as GPIO
	PTC->PDDR &= ~GPIO_PDDR_PDD(1 << SWITCH_1_PIN); // make it input
	PORTC->PCR[SWITCH_1_PIN] |= PORT_PCR_PE(1); // Turn on the pull enable
	PORTC->PCR[SWITCH_1_PIN] |= PORT_PCR_PS(1); // Enable the pullup resistor
	PORTC->PCR[SWITCH_1_PIN] &= ~PORT_PCR_IRQC(0b1111); // Clear IRQC bits for PTC3
	PORTC->PCR[SWITCH_1_PIN] |= PORT_PCR_IRQC(0b1011); // Set up the IRQC to interrupt on either edge (i.e. from high to low or low to high)

	NVIC_EnableIRQ(PORTC_PORTD_IRQn);
	while(1){}
}



