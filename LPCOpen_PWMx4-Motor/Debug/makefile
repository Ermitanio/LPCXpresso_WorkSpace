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
all: LPCOpen_PWMx4-Motor.axf

# Tool invocations
LPCOpen_PWMx4-Motor.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"D:\UTN - Apuntes\UTN 2015\PROYECTO\LPCXpresso_WorkSpace\lpc_board_nxp_lpcxpresso_1769\Debug" -L"D:\UTN - Apuntes\UTN 2015\PROYECTO\LPCXpresso_WorkSpace\lpc_chip_175x_6x\Debug" -Xlinker -Map="LPCOpen_PWMx4-Motor.map" -Xlinker --gc-sections -mcpu=cortex-m3 -mthumb -T "LPCOpen_PWMx4-Motor_Debug.ld" -o "LPCOpen_PWMx4-Motor.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) LPCOpen_PWMx4-Motor.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "LPCOpen_PWMx4-Motor.axf" ; arm-none-eabi-objcopy -v -O binary "LPCOpen_PWMx4-Motor.axf" "LPCOpen_PWMx4-Motor.bin" ; checksum -p LPC1769 -d "LPCOpen_PWMx4-Motor.bin" ; arm-none-eabi-objcopy -I binary "LPCOpen_PWMx4-Motor.bin" -O ihex "LPCOpen_PWMx4-Motor.hex"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
