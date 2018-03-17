################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../board/board.c \
../board/clock_config.c \
../board/peripherals.c \
../board/pin_mux.c 

OBJS += \
./board/board.o \
./board/clock_config.o \
./board/peripherals.o \
./board/pin_mux.o 

C_DEPS += \
./board/board.d \
./board/clock_config.d \
./board/peripherals.d \
./board/pin_mux.d 


# Each subdirectory must supply rules for building sources it contributes
board/%.o: ../board/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DPRINTF_FLOAT_ENABLE=0 -D__USE_CMSIS -DCR_INTEGER_PRINTF -DSDK_DEBUGCONSOLE=0 -D__MCUXPRESSO -DDEBUG -DSDK_OS_FREE_RTOS -DFSL_RTOS_FREE_RTOS -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -I"C:\Users\Cursos\Documents\MCUXpressoIDE_10.1.1_606\workspace\PRUEBA_SPI\board" -I"C:\Users\Cursos\Documents\MCUXpressoIDE_10.1.1_606\workspace\PRUEBA_SPI\source" -I"C:\Users\Cursos\Documents\MCUXpressoIDE_10.1.1_606\workspace\PRUEBA_SPI" -I"C:\Users\Cursos\Documents\MCUXpressoIDE_10.1.1_606\workspace\PRUEBA_SPI\freertos\include" -I"C:\Users\Cursos\Documents\MCUXpressoIDE_10.1.1_606\workspace\PRUEBA_SPI\freertos\portable" -I"C:\Users\Cursos\Documents\MCUXpressoIDE_10.1.1_606\workspace\PRUEBA_SPI\drivers" -I"C:\Users\Cursos\Documents\MCUXpressoIDE_10.1.1_606\workspace\PRUEBA_SPI\CMSIS" -I"C:\Users\Cursos\Documents\MCUXpressoIDE_10.1.1_606\workspace\PRUEBA_SPI\utilities" -I"C:\Users\Cursos\Documents\MCUXpressoIDE_10.1.1_606\workspace\PRUEBA_SPI\startup" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


