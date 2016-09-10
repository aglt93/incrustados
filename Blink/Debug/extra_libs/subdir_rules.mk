################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
extra_libs/HAL_I2C.obj: ../extra_libs/HAL_I2C.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"/opt/ti/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/opt/ti/ccsv6/ccs_base/arm/include" --include_path="/home/kvallecillog/Documents/incrustados/Blink/driverlib/MSP432P4xx" --include_path="/home/kvallecillog/Documents/incrustados/Blink/extra_libs" --include_path="/opt/ti/ccsv6/ccs_base/arm/include/CMSIS" --include_path="/opt/ti/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/include" --advice:power="all" -g --gcc --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs --display_error_number --diag_warning=225 --diag_wrap=off --abi=eabi --preproc_with_compile --preproc_dependency="extra_libs/HAL_I2C.d" --obj_directory="extra_libs" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

extra_libs/HAL_OPT3001.obj: ../extra_libs/HAL_OPT3001.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"/opt/ti/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/opt/ti/ccsv6/ccs_base/arm/include" --include_path="/home/kvallecillog/Documents/incrustados/Blink/driverlib/MSP432P4xx" --include_path="/home/kvallecillog/Documents/incrustados/Blink/extra_libs" --include_path="/opt/ti/ccsv6/ccs_base/arm/include/CMSIS" --include_path="/opt/ti/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/include" --advice:power="all" -g --gcc --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs --display_error_number --diag_warning=225 --diag_wrap=off --abi=eabi --preproc_with_compile --preproc_dependency="extra_libs/HAL_OPT3001.d" --obj_directory="extra_libs" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '


