################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../main.c 

OBJS += \
./main.o 

C_DEPS += \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU C Compiler'
	arm-none-eabi-gcc -mthumb -mcpu=cortex-m3 -IN:\Project\GIT\MotorControlWork\motorCONTROL\SoftConsole\motorCONTROL_MSS_MSS_CM3_0\motorCONTROL_MSS_MSS_CM3_0_hw_platform -IN:\Project\GIT\MotorControlWork\motorCONTROL\SoftConsole\motorCONTROL_MSS_MSS_CM3_0\motorCONTROL_MSS_MSS_CM3_0_hw_platform\CMSIS -IN:\Project\GIT\MotorControlWork\motorCONTROL\SoftConsole\motorCONTROL_MSS_MSS_CM3_0\motorCONTROL_MSS_MSS_CM3_0_hw_platform\CMSIS\startup_gcc -IN:\Project\GIT\MotorControlWork\motorCONTROL\SoftConsole\motorCONTROL_MSS_MSS_CM3_0\motorCONTROL_MSS_MSS_CM3_0_hw_platform\drivers -IN:\Project\GIT\MotorControlWork\motorCONTROL\SoftConsole\motorCONTROL_MSS_MSS_CM3_0\motorCONTROL_MSS_MSS_CM3_0_hw_platform\drivers\mss_gpio -IN:\Project\GIT\MotorControlWork\motorCONTROL\SoftConsole\motorCONTROL_MSS_MSS_CM3_0\motorCONTROL_MSS_MSS_CM3_0_hw_platform\drivers\mss_nvm -IN:\Project\GIT\MotorControlWork\motorCONTROL\SoftConsole\motorCONTROL_MSS_MSS_CM3_0\motorCONTROL_MSS_MSS_CM3_0_hw_platform\drivers\mss_nvm\drivers -IN:\Project\GIT\MotorControlWork\motorCONTROL\SoftConsole\motorCONTROL_MSS_MSS_CM3_0\motorCONTROL_MSS_MSS_CM3_0_hw_platform\drivers\mss_nvm\drivers\F2DSS_NVM -IN:\Project\GIT\MotorControlWork\motorCONTROL\SoftConsole\motorCONTROL_MSS_MSS_CM3_0\motorCONTROL_MSS_MSS_CM3_0_hw_platform\drivers\mss_pdma -IN:\Project\GIT\MotorControlWork\motorCONTROL\SoftConsole\motorCONTROL_MSS_MSS_CM3_0\motorCONTROL_MSS_MSS_CM3_0_hw_platform\drivers\mss_rtc -IN:\Project\GIT\MotorControlWork\motorCONTROL\SoftConsole\motorCONTROL_MSS_MSS_CM3_0\motorCONTROL_MSS_MSS_CM3_0_hw_platform\drivers\mss_rtc\drivers -IN:\Project\GIT\MotorControlWork\motorCONTROL\SoftConsole\motorCONTROL_MSS_MSS_CM3_0\motorCONTROL_MSS_MSS_CM3_0_hw_platform\drivers\mss_rtc\drivers\mss_rtc -IN:\Project\GIT\MotorControlWork\motorCONTROL\SoftConsole\motorCONTROL_MSS_MSS_CM3_0\motorCONTROL_MSS_MSS_CM3_0_hw_platform\drivers\mss_timer -IN:\Project\GIT\MotorControlWork\motorCONTROL\SoftConsole\motorCONTROL_MSS_MSS_CM3_0\motorCONTROL_MSS_MSS_CM3_0_hw_platform\drivers\mss_uart -IN:\Project\GIT\MotorControlWork\motorCONTROL\SoftConsole\motorCONTROL_MSS_MSS_CM3_0\motorCONTROL_MSS_MSS_CM3_0_hw_platform\hal -IN:\Project\GIT\MotorControlWork\motorCONTROL\SoftConsole\motorCONTROL_MSS_MSS_CM3_0\motorCONTROL_MSS_MSS_CM3_0_hw_platform\hal\CortexM3 -IN:\Project\GIT\MotorControlWork\motorCONTROL\SoftConsole\motorCONTROL_MSS_MSS_CM3_0\motorCONTROL_MSS_MSS_CM3_0_hw_platform\hal\CortexM3\GNU -O1 -ffunction-sections -fdata-sections -g3 -Wall -c -fmessage-length=0 -v -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


