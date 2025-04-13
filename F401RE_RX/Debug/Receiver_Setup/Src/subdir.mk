################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Receiver_Setup/Src/rtc.cpp \
../Receiver_Setup/Src/timeunit.cpp 

OBJS += \
./Receiver_Setup/Src/rtc.o \
./Receiver_Setup/Src/timeunit.o 

CPP_DEPS += \
./Receiver_Setup/Src/rtc.d \
./Receiver_Setup/Src/timeunit.d 


# Each subdirectory must supply rules for building sources it contributes
Receiver_Setup/Src/%.o Receiver_Setup/Src/%.su Receiver_Setup/Src/%.cyclo: ../Receiver_Setup/Src/%.cpp Receiver_Setup/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Receiver_Setup/Inc -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Receiver_Setup-2f-Src

clean-Receiver_Setup-2f-Src:
	-$(RM) ./Receiver_Setup/Src/rtc.cyclo ./Receiver_Setup/Src/rtc.d ./Receiver_Setup/Src/rtc.o ./Receiver_Setup/Src/rtc.su ./Receiver_Setup/Src/timeunit.cyclo ./Receiver_Setup/Src/timeunit.d ./Receiver_Setup/Src/timeunit.o ./Receiver_Setup/Src/timeunit.su

.PHONY: clean-Receiver_Setup-2f-Src

