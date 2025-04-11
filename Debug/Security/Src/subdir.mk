################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Security/Src/rtc.cpp 

OBJS += \
./Security/Src/rtc.o 

CPP_DEPS += \
./Security/Src/rtc.d 


# Each subdirectory must supply rules for building sources it contributes
Security/Src/%.o Security/Src/%.su Security/Src/%.cyclo: ../Security/Src/%.cpp Security/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../Core/Inc -I../Security/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Security-2f-Src

clean-Security-2f-Src:
	-$(RM) ./Security/Src/rtc.cyclo ./Security/Src/rtc.d ./Security/Src/rtc.o ./Security/Src/rtc.su

.PHONY: clean-Security-2f-Src

