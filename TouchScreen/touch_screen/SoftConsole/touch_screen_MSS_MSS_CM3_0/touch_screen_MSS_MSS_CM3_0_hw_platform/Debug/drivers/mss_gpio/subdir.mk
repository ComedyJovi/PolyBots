################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/mss_gpio/mss_gpio.c 

OBJS += \
./drivers/mss_gpio/mss_gpio.o 

C_DEPS += \
./drivers/mss_gpio/mss_gpio.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/mss_gpio/%.o: ../drivers/mss_gpio/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU C Compiler'
	arm-none-eabi-gcc -mthumb -mcpu=cortex-m3 -IN:\eecs373\PolyBots\TouchScreen\touch_screen\SoftConsole\touch_screen_MSS_MSS_CM3_0\touch_screen_MSS_MSS_CM3_0_hw_platform -IN:\eecs373\PolyBots\TouchScreen\touch_screen\SoftConsole\touch_screen_MSS_MSS_CM3_0\touch_screen_MSS_MSS_CM3_0_hw_platform\CMSIS -IN:\eecs373\PolyBots\TouchScreen\touch_screen\SoftConsole\touch_screen_MSS_MSS_CM3_0\touch_screen_MSS_MSS_CM3_0_hw_platform\CMSIS\startup_gcc -IN:\eecs373\PolyBots\TouchScreen\touch_screen\SoftConsole\touch_screen_MSS_MSS_CM3_0\touch_screen_MSS_MSS_CM3_0_hw_platform\drivers -IN:\eecs373\PolyBots\TouchScreen\touch_screen\SoftConsole\touch_screen_MSS_MSS_CM3_0\touch_screen_MSS_MSS_CM3_0_hw_platform\drivers\mss_gpio -IN:\eecs373\PolyBots\TouchScreen\touch_screen\SoftConsole\touch_screen_MSS_MSS_CM3_0\touch_screen_MSS_MSS_CM3_0_hw_platform\drivers\mss_i2c -IN:\eecs373\PolyBots\TouchScreen\touch_screen\SoftConsole\touch_screen_MSS_MSS_CM3_0\touch_screen_MSS_MSS_CM3_0_hw_platform\drivers\mss_nvm -IN:\eecs373\PolyBots\TouchScreen\touch_screen\SoftConsole\touch_screen_MSS_MSS_CM3_0\touch_screen_MSS_MSS_CM3_0_hw_platform\drivers\mss_nvm\drivers -IN:\eecs373\PolyBots\TouchScreen\touch_screen\SoftConsole\touch_screen_MSS_MSS_CM3_0\touch_screen_MSS_MSS_CM3_0_hw_platform\drivers\mss_nvm\drivers\F2DSS_NVM -IN:\eecs373\PolyBots\TouchScreen\touch_screen\SoftConsole\touch_screen_MSS_MSS_CM3_0\touch_screen_MSS_MSS_CM3_0_hw_platform\drivers\mss_pdma -IN:\eecs373\PolyBots\TouchScreen\touch_screen\SoftConsole\touch_screen_MSS_MSS_CM3_0\touch_screen_MSS_MSS_CM3_0_hw_platform\drivers\mss_rtc -IN:\eecs373\PolyBots\TouchScreen\touch_screen\SoftConsole\touch_screen_MSS_MSS_CM3_0\touch_screen_MSS_MSS_CM3_0_hw_platform\drivers\mss_rtc\drivers -IN:\eecs373\PolyBots\TouchScreen\touch_screen\SoftConsole\touch_screen_MSS_MSS_CM3_0\touch_screen_MSS_MSS_CM3_0_hw_platform\drivers\mss_rtc\drivers\mss_rtc -IN:\eecs373\PolyBots\TouchScreen\touch_screen\SoftConsole\touch_screen_MSS_MSS_CM3_0\touch_screen_MSS_MSS_CM3_0_hw_platform\drivers\mss_spi -IN:\eecs373\PolyBots\TouchScreen\touch_screen\SoftConsole\touch_screen_MSS_MSS_CM3_0\touch_screen_MSS_MSS_CM3_0_hw_platform\drivers\mss_spi\drivers -IN:\eecs373\PolyBots\TouchScreen\touch_screen\SoftConsole\touch_screen_MSS_MSS_CM3_0\touch_screen_MSS_MSS_CM3_0_hw_platform\drivers\mss_spi\drivers\mss_spi -IN:\eecs373\PolyBots\TouchScreen\touch_screen\SoftConsole\touch_screen_MSS_MSS_CM3_0\touch_screen_MSS_MSS_CM3_0_hw_platform\drivers\mss_timer -IN:\eecs373\PolyBots\TouchScreen\touch_screen\SoftConsole\touch_screen_MSS_MSS_CM3_0\touch_screen_MSS_MSS_CM3_0_hw_platform\hal -IN:\eecs373\PolyBots\TouchScreen\touch_screen\SoftConsole\touch_screen_MSS_MSS_CM3_0\touch_screen_MSS_MSS_CM3_0_hw_platform\hal\CortexM3 -IN:\eecs373\PolyBots\TouchScreen\touch_screen\SoftConsole\touch_screen_MSS_MSS_CM3_0\touch_screen_MSS_MSS_CM3_0_hw_platform\hal\CortexM3\GNU -I"N:\eecs373\PolyBots\TouchScreen\Adafruit_GFX" -I"N:\eecs373\PolyBots\TouchScreen\Adafruit_ILI9341" -I"N:\eecs373\PolyBots\TouchScreen\freetype_2_7_1" -I"N:\eecs373\PolyBots\TouchScreen\Adafruit_GFX\fontconvert" -I"N:\eecs373\PolyBots\TouchScreen\Adafruit_GFX\Fonts" -O0 -ffunction-sections -fdata-sections -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


