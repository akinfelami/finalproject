#include <stdio.h>
#include <stdbool.h>
#include <MKL46Z4.h>
#include "switch.h"
#include "utils.h"

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

typedef struct {
	unsigned int sec;
	unsigned int msec;
} time;

time current_time;

volatile bool switchPressed = false;



// IRQ Handler for switch
void PORTC_PORTD_IRQHandler(void) {
	switchPressed = true;
//	init_pit();
	PTE->PTOR = GPIO_PTOR_PTTO(1 << RED_LED_PIN); // button pressed, toggle LED
	PORTC->PCR[SWITCH_1_PIN] |= PORT_PCR_ISF(1);  // clear the interrupt status flag by writing 1 to it
	switchPressed=false;

}

// IRQ for timer
void PIT_IRQHandler(void){

	if (current_time.msec==999){
		current_time.sec +=1;
		current_time.msec=0;
	}else{
		current_time.msec +=1;
	}
	if(current_time.sec%4==0){ // every 4seconds toggle.
		delay();
		LEDGreen_Toggle();
	}
	PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;

}

void operate_switch_interrupts(void) {


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
    printf("ADC Value: %d\r\n", switchPressed);
	while(1){}
}

void init_pit(void){

	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
	PIT->MCR = 0;
	PIT->CHANNEL[0].LDVAL = DEFAULT_SYSTEM_CLOCK / 1000;
	PIT->CHANNEL[0].TCTRL = PIT_TCTRL_TIE_MASK; // Timer interrupt enable
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK ; //enable timer to start countdown;


	current_time.sec = 0;
	current_time.msec= 0;
	NVIC_EnableIRQ(PIT_IRQn);

}



