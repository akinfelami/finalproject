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




/*
 * @brief   Application entry point.
 */
int main(void) {
	LED_Initialize();
	operate_switch_interrupts();
	return 0;
}
