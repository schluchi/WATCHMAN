################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
LD_SRCS += \
../src/lscript.ld 

C_SRCS += \
../src/axis_peripheral.c \
../src/data_test.c \
../src/i2c_access.c \
../src/iic_phyreset.c \
../src/interrupt.c \
../src/log_file.c \
../src/main.c \
../src/platform_mb.c \
../src/platform_ppc.c \
../src/sfp.c \
../src/si5324.c \
../src/time_hm.c \
../src/udp_peripheral.c \
../src/utility.c 

OBJS += \
./src/axis_peripheral.o \
./src/data_test.o \
./src/i2c_access.o \
./src/iic_phyreset.o \
./src/interrupt.o \
./src/log_file.o \
./src/main.o \
./src/platform_mb.o \
./src/platform_ppc.o \
./src/sfp.o \
./src/si5324.o \
./src/time_hm.o \
./src/udp_peripheral.o \
./src/utility.o 

C_DEPS += \
./src/axis_peripheral.d \
./src/data_test.d \
./src/i2c_access.d \
./src/iic_phyreset.d \
./src/interrupt.d \
./src/log_file.d \
./src/main.d \
./src/platform_mb.d \
./src/platform_ppc.d \
./src/sfp.d \
./src/si5324.d \
./src/time_hm.d \
./src/udp_peripheral.d \
./src/utility.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -I../../PC_PS_PL_log_bsp/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


