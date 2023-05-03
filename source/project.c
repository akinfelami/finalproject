/*
 * Copyright 2016-2023 NXP
 * All rights reserved.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL46Z4.h"
#include "fsl_debug_console.h"
#include "switch.h"
#include "utils.h"
#include "fsl_adc16.h"



/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_ADC16_BASE ADC0
#define DEMO_ADC16_CHANNEL_GROUP 0U
#define DEMO_ADC16_USER_CHANNEL 0U /* PTE20, ADC0_SE0 */

#define DEMO_ADC16_IRQn ADC0_IRQn
#define DEMO_ADC16_IRQ_HANDLER_FUNC ADC0_IRQHandler
#define BUFFER_SIZE 4096 // About 4 seconds of audio data



/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool g_Adc16ConversionDoneFlag = false;
volatile uint32_t g_Adc16ConversionValue;
volatile uint32_t g_Adc16InterruptCounter;



static int16_t buffer[BUFFER_SIZE];
static volatile uint32_t bufferIndex = 0;


void DEMO_ADC16_IRQ_HANDLER_FUNC(void)
{
    g_Adc16ConversionDoneFlag = true;
    /* Read conversion result to clear the conversion completed flag. */
    g_Adc16ConversionValue = ADC16_GetChannelConversionValue(DEMO_ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP);
    g_Adc16InterruptCounter++;

    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
      exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}


static void init_adc(void){

    adc16_config_t adc16ConfigStruct;
    adc16_channel_config_t adc16ChannelConfigStruct;

    ADC16_GetDefaultConfig(&adc16ConfigStruct);

#ifdef BOARD_ADC_USE_ALT_VREF
    adc16ConfigStruct.referenceVoltageSource = kADC16_ReferenceVoltageSourceValt;
#endif
    ADC16_Init(DEMO_ADC16_BASE, &adc16ConfigStruct);
    ADC16_EnableHardwareTrigger(DEMO_ADC16_BASE, false); /* Make sure the software trigger is used. */

#if defined(FSL_FEATURE_ADC16_HAS_CALIBRATION) && FSL_FEATURE_ADC16_HAS_CALIBRATION
    if (kStatus_Success == ADC16_DoAutoCalibration(DEMO_ADC16_BASE))
    {
        PRINTF("ADC16_DoAutoCalibration() Done.\r\n");
    }
    else
    {
        PRINTF("ADC16_DoAutoCalibration() Failed.\r\n");
    }
#endif /* FSL_FEATURE_ADC16_HAS_CALIBRATION */

    adc16ChannelConfigStruct.channelNumber = DEMO_ADC16_USER_CHANNEL;
    adc16ChannelConfigStruct.enableInterruptOnConversionCompleted = true; /* Enable the interrupt. */

#if defined(FSL_FEATURE_ADC16_HAS_DIFF_MODE) && FSL_FEATURE_ADC16_HAS_DIFF_MODE
    adc16ChannelConfigStruct.enableDifferentialConversion = false;
#endif /* FSL_FEATURE_ADC16_HAS_DIFF_MODE */
    ADC16_SetChannelConfig(DEMO_ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP, &adc16ChannelConfigStruct);

    NVIC_EnableIRQ(DEMO_ADC16_IRQn);


}



/*
 * @brief   Application entry point.
 */
int main(void) {
	LED_Initialize();
	operate_switch_interrupts();

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();


    init_adc();
    //init_pit();

    g_Adc16InterruptCounter = 0U;


    PRINTF("\r\nADC16 interrupt Starting....\r\n");

    // Configure PIT timer to do the conversion every millisecond when switch is pressed.
    // this happens as soon as switch interrupts in detected.

//
//    while(1){
//    	if(g_Adc16ConversionDoneFlag){
//    		buffer[bufferIndex] = g_Adc16ConversionValue;
//    		bufferIndex = (bufferIndex + 1) % BUFFER_SIZE;
//            PRINTF("ADC Value: %d\r\n", g_Adc16ConversionValue);
//            PRINTF("ADC Interrupt Count: %d\r\n", g_Adc16InterruptCounter);
//            g_Adc16ConversionDoneFlag = false;
//
//    	}
//    }

    while(1){}



	return 0;
}
