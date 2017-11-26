################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Library/CTS_HAL.obj: ../Library/CTS_HAL.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/bin/cl430" -vmsp -Ooff --opt_for_speed=1 --use_hw_mpy=none --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/EE/Firmware/CCS/MSP430/MSPFuelGauge" --include_path="C:/EE/Firmware/CCS/MSP430/MSPFuelGauge/Library" --include_path="C:/EE/Firmware/CCS/MSP430/MSPFuelGauge/Application" --include_path="C:/EE/Firmware/CCS/MSP430/MSPFuelGauge/mujoeOS" --include_path="C:/EE/Firmware/CCS/MSP430/MSPFuelGauge/mujoeDrivers" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/include" --advice:power=all --define=__MSP430G2553__ -g --gcc --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Library/CTS_HAL.d_raw" --obj_directory="Library" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Library/CTS_Layer.obj: ../Library/CTS_Layer.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/bin/cl430" -vmsp -Ooff --opt_for_speed=1 --use_hw_mpy=none --include_path="C:/ti/ccsv7/ccs_base/msp430/include" --include_path="C:/EE/Firmware/CCS/MSP430/MSPFuelGauge" --include_path="C:/EE/Firmware/CCS/MSP430/MSPFuelGauge/Library" --include_path="C:/EE/Firmware/CCS/MSP430/MSPFuelGauge/Application" --include_path="C:/EE/Firmware/CCS/MSP430/MSPFuelGauge/mujoeOS" --include_path="C:/EE/Firmware/CCS/MSP430/MSPFuelGauge/mujoeDrivers" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.6.LTS/include" --advice:power=all --define=__MSP430G2553__ -g --gcc --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Library/CTS_Layer.d_raw" --obj_directory="Library" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


