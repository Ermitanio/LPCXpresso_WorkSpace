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
all: MPU-6050.axf

# Tool invocations
MPU-6050.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"D:\UTN - Apuntes\UTN 2015\PROYECTO\LPCXpresso_WorkSpace\lpc_chip_175x_6x\Debug" -Xlinker -Map="MPU-6050.map" -Xlinker --gc-sections -mcpu=cortex-m3 -mthumb -T "MPU-6050_Debug.ld" -o "MPU-6050.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) MPU-6050.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "MPU-6050.axf"; # arm-none-eabi-objcopy -v -O binary "MPU-6050.axf" "MPU-6050.bin" ; # checksum -p LPC1769 -d "MPU-6050.bin";
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
