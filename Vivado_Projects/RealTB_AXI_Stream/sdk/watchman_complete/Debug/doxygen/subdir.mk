################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
LD_SRCS += \
../doxygen/lscript.ld 

C_SRCS += \
../doxygen/axis_peripheral.c \
../doxygen/data_test.c \
../doxygen/file_hm.c \
../doxygen/global.c \
../doxygen/interrupt.c \
../doxygen/main.c \
../doxygen/time_hm.c \
../doxygen/udp_peripheral.c \
../doxygen/utility.c 

OBJS += \
./doxygen/axis_peripheral.o \
./doxygen/data_test.o \
./doxygen/file_hm.o \
./doxygen/global.o \
./doxygen/interrupt.o \
./doxygen/main.o \
./doxygen/time_hm.o \
./doxygen/udp_peripheral.o \
./doxygen/utility.o 

C_DEPS += \
./doxygen/axis_peripheral.d \
./doxygen/data_test.d \
./doxygen/file_hm.d \
./doxygen/global.d \
./doxygen/interrupt.d \
./doxygen/main.d \
./doxygen/time_hm.d \
./doxygen/udp_peripheral.d \
./doxygen/utility.d 


# Each subdirectory must supply rules for building sources it contributes
doxygen/%.o: ../doxygen/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -I../../watchman_complete_bsp/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


