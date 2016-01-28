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
all: LPCOpen_PWM-MotorBrushless.axf

# Tool invocations
LPCOpen_PWM-MotorBrushless.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"D:\UTN - Apuntes\UTN 2015\PROYECTO\LPCXpresso_WorkSpace\lpc_chip_175x_6x\Debug" -Xlinker -Map="LPCOpen_PWM-MotorBrushless.map" -Xlinker --gc-sections -mcpu=cortex-m3 -mthumb -T "LPCOpen_PWM-MotorBrushless_Debug.ld" -o "LPCOpen_PWM-MotorBrushless.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) LPCOpen_PWM-MotorBrushless.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "LPCOpen_PWM-MotorBrushless.axf" ; arm-none-eabi-objcopy -v -O binary "LPCOpen_PWM-MotorBrushless.axf" "LPCOpen_PWM-MotorBrushless.bin" ; checksum -p LPC1769 -d "LPCOpen_PWM-MotorBrushless.bin" ; arm-none-eabi-objcopy -I binary "LPCOpen_PWM-MotorBrushless.bin" -O ihex "LPCOpen_PWM-MotorBrushless.hex"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
