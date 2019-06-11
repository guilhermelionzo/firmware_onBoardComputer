################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Full_Demo/FreeRTOS+CLI/FreeRTOS_CLI.obj: C:/ti/FreeRTOSv10.0.1/FreeRTOS-Plus/Source/FreeRTOS-Plus-CLI/FreeRTOS_CLI.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O0 --opt_for_speed=2 --include_path="C:/ti/ccsv8/ccs_base/arm/include" --include_path="C:/Program Files (x86)/Percepio/Tracealyzer 4/FreeRTOS/TraceRecorder/streamports/File/include" --include_path="C:/ti/simplelink_msp432p4_sdk_2_10_00_14/source" --include_path="C:/Users/Guilherme/workspace_v8/RTOSDemo/Full_Demo" --include_path="C:/Users/Guilherme/workspace_v8/RTOSDemo/driverlib/inc" --include_path="C:/Users/Guilherme/workspace_v8/RTOSDemo/driverlib" --include_path="C:/ti/FreeRTOSv10.0.1/FreeRTOS-Plus/Source/FreeRTOS-Plus-CLI" --include_path="C:/ti/FreeRTOSv10.0.1/FreeRTOS/Demo/Common/include" --include_path="C:/ti/FreeRTOSv10.0.1/FreeRTOS/Source/include" --include_path="C:/ti/FreeRTOSv10.0.1/FreeRTOS/Source/portable/CCS/ARM_CM4F" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.1.LTS/include" --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.1.LTS/include/" --include_path="C:/ti/ccsv8/ccs_base/arm/include/" --include_path="C:/ti/ccsv8/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Guilherme/workspace_v8/RTOSDemo" -g --gcc --define=__MSP432P401R__ --define=USE_CMSIS_REGISTER_FORMAT=1 --define=TARGET_IS_MSP432P4XX --define=ccs --diag_warning=225 --diag_wrap=off --display_error_number --verbose_diagnostics --gen_func_subsections=on --preproc_with_compile --preproc_dependency="Full_Demo/FreeRTOS+CLI/FreeRTOS_CLI.d_raw" --obj_directory="Full_Demo/FreeRTOS+CLI" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


