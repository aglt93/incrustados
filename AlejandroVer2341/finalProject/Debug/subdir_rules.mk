################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
Ball.obj: ../Ball.cpp $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"/home/aglt93/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/aglt93/ti/ccsv6/ccs_base/arm/include" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/GrLib/grlib" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/GrLib/fonts" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/LcdDriver" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/driverlib/MSP432P4xx" --include_path="/home/aglt93/ti/ccsv6/ccs_base/arm/include/CMSIS" --include_path="/home/aglt93/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/include" --advice:power=all -g --gcc --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs --display_error_number --diag_warning=225 --diag_wrap=off --abi=eabi --preproc_with_compile --preproc_dependency="Ball.d" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

Button.obj: ../Button.cpp $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"/home/aglt93/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/aglt93/ti/ccsv6/ccs_base/arm/include" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/GrLib/grlib" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/GrLib/fonts" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/LcdDriver" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/driverlib/MSP432P4xx" --include_path="/home/aglt93/ti/ccsv6/ccs_base/arm/include/CMSIS" --include_path="/home/aglt93/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/include" --advice:power=all -g --gcc --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs --display_error_number --diag_warning=225 --diag_wrap=off --abi=eabi --preproc_with_compile --preproc_dependency="Button.d" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

GamePiece.obj: ../GamePiece.cpp $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"/home/aglt93/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/aglt93/ti/ccsv6/ccs_base/arm/include" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/GrLib/grlib" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/GrLib/fonts" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/LcdDriver" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/driverlib/MSP432P4xx" --include_path="/home/aglt93/ti/ccsv6/ccs_base/arm/include/CMSIS" --include_path="/home/aglt93/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/include" --advice:power=all -g --gcc --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs --display_error_number --diag_warning=225 --diag_wrap=off --abi=eabi --preproc_with_compile --preproc_dependency="GamePiece.d" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

LED.obj: ../LED.cpp $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"/home/aglt93/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/aglt93/ti/ccsv6/ccs_base/arm/include" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/GrLib/grlib" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/GrLib/fonts" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/LcdDriver" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/driverlib/MSP432P4xx" --include_path="/home/aglt93/ti/ccsv6/ccs_base/arm/include/CMSIS" --include_path="/home/aglt93/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/include" --advice:power=all -g --gcc --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs --display_error_number --diag_warning=225 --diag_wrap=off --abi=eabi --preproc_with_compile --preproc_dependency="LED.d" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

Racket.obj: ../Racket.cpp $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"/home/aglt93/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/aglt93/ti/ccsv6/ccs_base/arm/include" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/GrLib/grlib" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/GrLib/fonts" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/LcdDriver" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/driverlib/MSP432P4xx" --include_path="/home/aglt93/ti/ccsv6/ccs_base/arm/include/CMSIS" --include_path="/home/aglt93/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/include" --advice:power=all -g --gcc --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs --display_error_number --diag_warning=225 --diag_wrap=off --abi=eabi --preproc_with_compile --preproc_dependency="Racket.d" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

Scheduler.obj: ../Scheduler.cpp $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"/home/aglt93/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/aglt93/ti/ccsv6/ccs_base/arm/include" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/GrLib/grlib" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/GrLib/fonts" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/LcdDriver" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/driverlib/MSP432P4xx" --include_path="/home/aglt93/ti/ccsv6/ccs_base/arm/include/CMSIS" --include_path="/home/aglt93/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/include" --advice:power=all -g --gcc --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs --display_error_number --diag_warning=225 --diag_wrap=off --abi=eabi --preproc_with_compile --preproc_dependency="Scheduler.d" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

Screen.obj: ../Screen.cpp $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"/home/aglt93/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/aglt93/ti/ccsv6/ccs_base/arm/include" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/GrLib/grlib" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/GrLib/fonts" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/LcdDriver" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/driverlib/MSP432P4xx" --include_path="/home/aglt93/ti/ccsv6/ccs_base/arm/include/CMSIS" --include_path="/home/aglt93/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/include" --advice:power=all -g --gcc --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs --display_error_number --diag_warning=225 --diag_wrap=off --abi=eabi --preproc_with_compile --preproc_dependency="Screen.d" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

Servo.obj: ../Servo.cpp $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"/home/aglt93/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/aglt93/ti/ccsv6/ccs_base/arm/include" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/GrLib/grlib" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/GrLib/fonts" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/LcdDriver" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/driverlib/MSP432P4xx" --include_path="/home/aglt93/ti/ccsv6/ccs_base/arm/include/CMSIS" --include_path="/home/aglt93/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/include" --advice:power=all -g --gcc --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs --display_error_number --diag_warning=225 --diag_wrap=off --abi=eabi --preproc_with_compile --preproc_dependency="Servo.d" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

Task.obj: ../Task.cpp $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"/home/aglt93/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/aglt93/ti/ccsv6/ccs_base/arm/include" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/GrLib/grlib" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/GrLib/fonts" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/LcdDriver" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/driverlib/MSP432P4xx" --include_path="/home/aglt93/ti/ccsv6/ccs_base/arm/include/CMSIS" --include_path="/home/aglt93/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/include" --advice:power=all -g --gcc --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs --display_error_number --diag_warning=225 --diag_wrap=off --abi=eabi --preproc_with_compile --preproc_dependency="Task.d" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.cpp $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"/home/aglt93/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/aglt93/ti/ccsv6/ccs_base/arm/include" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/GrLib/grlib" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/GrLib/fonts" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/LcdDriver" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/driverlib/MSP432P4xx" --include_path="/home/aglt93/ti/ccsv6/ccs_base/arm/include/CMSIS" --include_path="/home/aglt93/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/include" --advice:power=all -g --gcc --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs --display_error_number --diag_warning=225 --diag_wrap=off --abi=eabi --preproc_with_compile --preproc_dependency="main.d" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

startup_msp432p401r_ccs.obj: ../startup_msp432p401r_ccs.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"/home/aglt93/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/aglt93/ti/ccsv6/ccs_base/arm/include" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/GrLib/grlib" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/GrLib/fonts" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/LcdDriver" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/driverlib/MSP432P4xx" --include_path="/home/aglt93/ti/ccsv6/ccs_base/arm/include/CMSIS" --include_path="/home/aglt93/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/include" --advice:power=all -g --gcc --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs --display_error_number --diag_warning=225 --diag_wrap=off --abi=eabi --preproc_with_compile --preproc_dependency="startup_msp432p401r_ccs.d" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

system_msp432p401r.obj: ../system_msp432p401r.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"/home/aglt93/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/aglt93/ti/ccsv6/ccs_base/arm/include" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/GrLib/grlib" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/GrLib/fonts" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/LcdDriver" --include_path="/home/aglt93/workspace_v6_1_3/finalProject/driverlib/MSP432P4xx" --include_path="/home/aglt93/ti/ccsv6/ccs_base/arm/include/CMSIS" --include_path="/home/aglt93/ti/ccsv6/tools/compiler/arm_15.12.3.LTS/include" --advice:power=all -g --gcc --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs --display_error_number --diag_warning=225 --diag_wrap=off --abi=eabi --preproc_with_compile --preproc_dependency="system_msp432p401r.d" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '


