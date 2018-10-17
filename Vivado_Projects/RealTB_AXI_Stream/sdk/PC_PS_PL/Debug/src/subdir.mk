################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
LD_SRCS += \
../src/lscript.ld 

C_SRCS += \
../src/cmd_interpreter.c \
../src/data_test.c \
../src/echo.c \
../src/i2c_access.c \
../src/iic_phyreset.c \
../src/main.c \
../src/platform_mb.c \
../src/platform_ppc.c \
../src/platform_zynq.c \
../src/reg_read.c \
../src/reg_write.c \
../src/send_data.c \
../src/sfp.c \
../src/si5324.c 

OBJS += \
./src/cmd_interpreter.o \
./src/data_test.o \
./src/echo.o \
./src/i2c_access.o \
./src/iic_phyreset.o \
./src/main.o \
./src/platform_mb.o \
./src/platform_ppc.o \
./src/platform_zynq.o \
./src/reg_read.o \
./src/reg_write.o \
./src/send_data.o \
./src/sfp.o \
./src/si5324.o 

C_DEPS += \
./src/cmd_interpreter.d \
./src/data_test.d \
./src/echo.d \
./src/i2c_access.d \
./src/iic_phyreset.d \
./src/main.d \
./src/platform_mb.d \
./src/platform_ppc.d \
./src/platform_zynq.d \
./src/reg_read.d \
./src/reg_write.d \
./src/send_data.d \
./src/sfp.d \
./src/si5324.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -I../../PC_PS_PL_bsp/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


