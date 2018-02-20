################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/rtos.c \
../source/rtos_main.c \
../source/semihost_hardfault.c 

OBJS += \
./source/rtos.o \
./source/rtos_main.o \
./source/semihost_hardfault.o 

C_DEPS += \
./source/rtos.d \
./source/rtos_main.d \
./source/semihost_hardfault.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DPRINTF_FLOAT_ENABLE=0 -D__USE_CMSIS -DCR_INTEGER_PRINTF -DSDK_DEBUGCONSOLE=1 -D__MCUXPRESSO -DDEBUG -DSDK_OS_BAREMETAL -DFSL_RTOS_BM -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -I"C:\SEBM_Team\SEBM2\rtos\board" -I"C:\SEBM_Team\SEBM2\rtos\source" -I"C:\SEBM_Team\SEBM2\rtos" -I"C:\SEBM_Team\SEBM2\rtos\drivers" -I"C:\SEBM_Team\SEBM2\rtos\CMSIS" -I"C:\SEBM_Team\SEBM2\rtos\utilities" -I"C:\SEBM_Team\SEBM2\rtos\startup" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


