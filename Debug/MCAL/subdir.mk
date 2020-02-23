################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MCAL/DIO.c \
../MCAL/PWM.c \
../MCAL/Timer.c \
../MCAL/external_interrupts.c 

OBJS += \
./MCAL/DIO.o \
./MCAL/PWM.o \
./MCAL/Timer.o \
./MCAL/external_interrupts.o 

C_DEPS += \
./MCAL/DIO.d \
./MCAL/PWM.d \
./MCAL/Timer.d \
./MCAL/external_interrupts.d 


# Each subdirectory must supply rules for building sources it contributes
MCAL/%.o: ../MCAL/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


