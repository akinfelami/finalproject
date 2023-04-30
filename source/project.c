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


/*
 * @brief   Application entry point.
 */
int main(void) {
	LED_Initialize();
	operate_switch_interrupts();
	return 0;
}
