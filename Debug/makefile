################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./Application/main.obj" \
"./Application/structure.obj" \
"./Library/CTS_HAL.obj" \
"./Library/CTS_Layer.obj" \
"./mujoeDrivers/mujoe_common.obj" \
"./mujoeDrivers/mujoe_fuelProbeMgr.obj" \
"./mujoeDrivers/mujoe_i2cRegMap.obj" \
"./mujoeDrivers/mujoe_i2cSlave.obj" \
"./mujoeOS/muJoe_i2cTask.obj" \
"./mujoeOS/muJoe_mainTask.obj" \
"./mujoeOS/muJoe_taskMgr.obj" \
"./mujoeOS/mujoe_bspTask.obj" \
"../lnk_msp430g2553.cmd" \
$(GEN_CMDS__FLAG) \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include Application/subdir_vars.mk
-include Library/subdir_vars.mk
-include mujoeDrivers/subdir_vars.mk
-include mujoeOS/subdir_vars.mk
-include subdir_rules.mk
-include Application/subdir_rules.mk
-include Library/subdir_rules.mk
-include mujoeDrivers/subdir_rules.mk
-include mujoeOS/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
MSPFuelGauge.out \

EXE_OUTPUTS__QUOTED += \
"MSPFuelGauge.out" \

BIN_OUTPUTS += \
MSPFuelGauge.hex \

BIN_OUTPUTS__QUOTED += \
"MSPFuelGauge.hex" \


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "MSPFuelGauge.out"

# Tool invocations
MSPFuelGauge.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: MSP430 Linker'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/bin/cl430" -vmsp -Ooff --opt_for_speed=1 --use_hw_mpy=none --advice:power=all --define=__MSP430G2553__ -g --gcc --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number -z -m"MSPFuelGauge.map" --heap_size=80 --stack_size=80 -i"C:/ti/ccsv7/ccs_base/msp430/include" -i"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/lib" -i"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/include" --reread_libs --diag_wrap=off --display_error_number --warn_sections --xml_link_info="MSPFuelGauge_linkInfo.xml" --use_hw_mpy=none --rom_model -o "MSPFuelGauge.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

MSPFuelGauge.hex: $(EXE_OUTPUTS)
	@echo 'Invoking: MSP430 Hex Utility'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/bin/hex430" --memwidth=8 --romwidth=8 -o "MSPFuelGauge.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "Application\main.obj" "Application\structure.obj" "Library\CTS_HAL.obj" "Library\CTS_Layer.obj" "mujoeDrivers\mujoe_common.obj" "mujoeDrivers\mujoe_fuelProbeMgr.obj" "mujoeDrivers\mujoe_i2cRegMap.obj" "mujoeDrivers\mujoe_i2cSlave.obj" "mujoeOS\muJoe_i2cTask.obj" "mujoeOS\muJoe_mainTask.obj" "mujoeOS\muJoe_taskMgr.obj" "mujoeOS\mujoe_bspTask.obj" 
	-$(RM) "Application\main.d" "Application\structure.d" "Library\CTS_HAL.d" "Library\CTS_Layer.d" "mujoeDrivers\mujoe_common.d" "mujoeDrivers\mujoe_fuelProbeMgr.d" "mujoeDrivers\mujoe_i2cRegMap.d" "mujoeDrivers\mujoe_i2cSlave.d" "mujoeOS\muJoe_i2cTask.d" "mujoeOS\muJoe_mainTask.d" "mujoeOS\muJoe_taskMgr.d" "mujoeOS\mujoe_bspTask.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

