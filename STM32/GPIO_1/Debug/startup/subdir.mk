################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../startup/startup_stm32f407xx.s 

OBJS += \
./startup/startup_stm32f407xx.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo $(PWD)
	arm-none-eabi-as -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -I"/home/marius/workspace_st/GPIO_1/inc" -I"/home/marius/workspace_st/GPIO_1/CMSIS/core" -I"/home/marius/workspace_st/GPIO_1/CMSIS/device" -I"/home/marius/workspace_st/GPIO_1/Utilities/Components/lsm303dlhc" -I"/home/marius/workspace_st/GPIO_1/Utilities/Components/mfxstm32l152" -I"/home/marius/workspace_st/GPIO_1/Utilities/Components/ili9341" -I"/home/marius/workspace_st/GPIO_1/Utilities/Components/wm8994" -I"/home/marius/workspace_st/GPIO_1/Utilities/Components/stmpe811" -I"/home/marius/workspace_st/GPIO_1/Utilities/Components/ls016b8uy" -I"/home/marius/workspace_st/GPIO_1/Utilities/Components/n25q128a" -I"/home/marius/workspace_st/GPIO_1/Utilities/Components/ili9325" -I"/home/marius/workspace_st/GPIO_1/Utilities/Components/ft6x06" -I"/home/marius/workspace_st/GPIO_1/Utilities/Components/l3gd20" -I"/home/marius/workspace_st/GPIO_1/Utilities/Components/otm8009a" -I"/home/marius/workspace_st/GPIO_1/Utilities/Components/ts3510" -I"/home/marius/workspace_st/GPIO_1/Utilities/Components/n25q512a" -I"/home/marius/workspace_st/GPIO_1/Utilities/Components/s25fl512s" -I"/home/marius/workspace_st/GPIO_1/Utilities/Components/exc7200" -I"/home/marius/workspace_st/GPIO_1/Utilities/Components/ampire480272" -I"/home/marius/workspace_st/GPIO_1/Utilities/Components/st7735" -I"/home/marius/workspace_st/GPIO_1/Utilities/Components/lis3dsh" -I"/home/marius/workspace_st/GPIO_1/Utilities/Components/s5k5cag" -I"/home/marius/workspace_st/GPIO_1/Utilities/Components/n25q256a" -I"/home/marius/workspace_st/GPIO_1/Utilities/Components/ampire640480" -I"/home/marius/workspace_st/GPIO_1/Utilities/Components/Common" -I"/home/marius/workspace_st/GPIO_1/Utilities/Components/stmpe1600" -I"/home/marius/workspace_st/GPIO_1/Utilities/Components/cs43l22" -I"/home/marius/workspace_st/GPIO_1/Utilities/Components/st7789h2" -I"/home/marius/workspace_st/GPIO_1/Utilities/Components/ov2640" -I"/home/marius/workspace_st/GPIO_1/Utilities/Components/lis302dl" -I"/home/marius/workspace_st/GPIO_1/Utilities" -I"/home/marius/workspace_st/GPIO_1/Utilities/STM32F4-Discovery" -I"/home/marius/workspace_st/GPIO_1/HAL_Driver/Inc" -I"/home/marius/workspace_st/GPIO_1/HAL_Driver/Inc/Legacy" -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


