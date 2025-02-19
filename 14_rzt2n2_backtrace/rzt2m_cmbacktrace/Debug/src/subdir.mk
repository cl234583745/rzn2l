################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/hal_entry.c \
../src/rzn2_rzt2_backtrace.c \
../src/syscall.c 

LST += \
hal_entry.lst \
rzn2_rzt2_backtrace.lst \
syscall.lst 

C_DEPS += \
./src/hal_entry.d \
./src/rzn2_rzt2_backtrace.d \
./src/syscall.d 

OBJS += \
./src/hal_entry.o \
./src/rzn2_rzt2_backtrace.o \
./src/syscall.o 

MAP += \
rzt2m_cmbacktrace.map 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	$(file > $@.in,-mcpu=cortex-r52 -mthumb -mfloat-abi=hard -mfpu=neon-fp-armv8 -fdiagnostics-parseable-fixits -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -Wnull-dereference -g -gdwarf-4 -D_RENESAS_RZT_ -D_RZT_CORE=CR52_0 -D_RZT_ORDINAL=1 -I"E:\\RS_workspace\\rzt2m_cmbacktrace\\src" -I"." -I"E:\\RS_workspace\\rzt2m_cmbacktrace\\rzt\\fsp\\inc" -I"E:\\RS_workspace\\rzt2m_cmbacktrace\\rzt\\fsp\\inc\\api" -I"E:\\RS_workspace\\rzt2m_cmbacktrace\\rzt\\fsp\\inc\\instances" -I"E:\\RS_workspace\\rzt2m_cmbacktrace\\rzt\\fsp\\src\\bsp\\mcu\\all\\cr" -I"E:\\RS_workspace\\rzt2m_cmbacktrace\\rzt\\arm\\CMSIS_5\\CMSIS\\Core_R\\Include" -I"E:\\RS_workspace\\rzt2m_cmbacktrace\\rzt_gen" -I"E:\\RS_workspace\\rzt2m_cmbacktrace\\rzt_cfg\\fsp_cfg\\bsp" -I"E:\\RS_workspace\\rzt2m_cmbacktrace\\rzt_cfg\\fsp_cfg" -std=c99 -Wno-format-truncation -Wno-stringop-overflow --param=min-pagesize=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<")
	@arm-none-eabi-gcc @"$@.in"

