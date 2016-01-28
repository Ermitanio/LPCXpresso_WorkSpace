################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: Prueba_RF_TX.axf

# Tool invocations
Prueba_RF_TX.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"D:\UTN - Apuntes\UTN 2015\PROYECTO\LPCXpresso_WorkSpace\lpc_chip_175x_6x\Debug" -Xlinker -Map="Prueba_RF_TX.map" -Xlinker --gc-sections -mcpu=cortex-m3 -mthumb -T "Prueba_RF_TX_Debug.ld" -o "Prueba_RF_TX.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) Prueba_RF_TX.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "Prueba_RF_TX.axf"; # arm-none-eabi-objcopy -v -O binary "Prueba_RF_TX.axf" "Prueba_RF_TX.bin" ; # checksum -p LPC1769 -d "Prueba_RF_TX.bin";
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
