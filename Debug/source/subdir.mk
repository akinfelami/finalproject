################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/mtb.c \
../source/project.c \
../source/semihost_hardfault.c \
../source/switch.c \
../source/utils.c 

C_DEPS += \
./source/mtb.d \
./source/project.d \
./source/semihost_hardfault.d \
./source/switch.d \
./source/utils.d 

OBJS += \
./source/mtb.o \
./source/project.o \
./source/semihost_hardfault.o \
./source/switch.o \
./source/utils.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MKL46Z256VLL4_cm0plus -DCPU_MKL46Z256VLL4 -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"/Users/akin/MCUXpressoIDE_11.7.0_9198/workspace/final_project/board" -I"/Users/akin/MCUXpressoIDE_11.7.0_9198/workspace/final_project/source" -I"/Users/akin/MCUXpressoIDE_11.7.0_9198/workspace/final_project" -I"/Users/akin/MCUXpressoIDE_11.7.0_9198/workspace/final_project/drivers" -I"/Users/akin/MCUXpressoIDE_11.7.0_9198/workspace/final_project/CMSIS" -I"/Users/akin/MCUXpressoIDE_11.7.0_9198/workspace/final_project/utilities" -I"/Users/akin/MCUXpressoIDE_11.7.0_9198/workspace/final_project/startup" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/mtb.d ./source/mtb.o ./source/project.d ./source/project.o ./source/semihost_hardfault.d ./source/semihost_hardfault.o ./source/switch.d ./source/switch.o ./source/utils.d ./source/utils.o

.PHONY: clean-source

