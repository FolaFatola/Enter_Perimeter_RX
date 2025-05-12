################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../TX_Modules/Src/nrf2401.cpp 

OBJS += \
./TX_Modules/Src/nrf2401.o 

CPP_DEPS += \
./TX_Modules/Src/nrf2401.d 


# Each subdirectory must supply rules for building sources it contributes
TX_Modules/Src/%.o TX_Modules/Src/%.su TX_Modules/Src/%.cyclo: ../TX_Modules/Src/%.cpp TX_Modules/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../TX_Modules/Inc -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-TX_Modules-2f-Src

clean-TX_Modules-2f-Src:
	-$(RM) ./TX_Modules/Src/nrf2401.cyclo ./TX_Modules/Src/nrf2401.d ./TX_Modules/Src/nrf2401.o ./TX_Modules/Src/nrf2401.su

.PHONY: clean-TX_Modules-2f-Src

