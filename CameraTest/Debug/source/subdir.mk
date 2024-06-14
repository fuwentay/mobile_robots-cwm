################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/ADC.c \
../source/CameraTest.c \
../source/camera.c \
../source/delay.c \
../source/mtb.c 

OBJS += \
./source/ADC.o \
./source/CameraTest.o \
./source/camera.o \
./source/delay.o \
./source/mtb.o 

C_DEPS += \
./source/ADC.d \
./source/CameraTest.d \
./source/camera.d \
./source/delay.d \
./source/mtb.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCR_INTEGER_PRINTF -DSDK_DEBUGCONSOLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DSDK_OS_BAREMETAL -DFSL_RTOS_BM -DCPU_MKL46Z256VLL4 -DCPU_MKL46Z256VLL4_cm0plus -D__REDLIB__ -I"C:\work\Robot\projects\CameraTest\source" -I"C:\work\Robot\projects\CameraTest" -I"C:\work\Robot\projects\CameraTest\drivers" -I"C:\work\Robot\projects\CameraTest\CMSIS" -I"C:\work\Robot\projects\CameraTest\startup" -I"C:\work\Robot\projects\CameraTest\frdmkl46z" -I"C:\work\Robot\projects\CameraTest\utilities" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

