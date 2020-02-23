################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include MCAL/subdir.mk
-include HAL/subdir.mk
-include APP/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
Static_Design_Challenge.lss \

FLASH_IMAGE += \
Static_Design_Challenge.hex \

SIZEDUMMY += \
sizedummy \


# All Target
all: Static_Design_Challenge.elf secondary-outputs

# Tool invocations
Static_Design_Challenge.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,Static_Design_Challenge.map -mmcu=atmega16 -o "Static_Design_Challenge.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

Static_Design_Challenge.lss: Static_Design_Challenge.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S Static_Design_Challenge.elf  >"Static_Design_Challenge.lss"
	@echo 'Finished building: $@'
	@echo ' '

Static_Design_Challenge.hex: Static_Design_Challenge.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex Static_Design_Challenge.elf  "Static_Design_Challenge.hex"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: Static_Design_Challenge.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega16 Static_Design_Challenge.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) Static_Design_Challenge.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents

-include ../makefile.targets
