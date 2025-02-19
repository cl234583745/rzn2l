################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../rzt_gen/common_data.c \
../rzt_gen/hal_data.c \
../rzt_gen/main.c \
../rzt_gen/pin_data.c \
../rzt_gen/vector_data.c 

LST += \
common_data.lst \
hal_data.lst \
main.lst \
pin_data.lst \
vector_data.lst 

C_DEPS += \
./rzt_gen/common_data.d \
./rzt_gen/hal_data.d \
./rzt_gen/main.d \
./rzt_gen/pin_data.d \
./rzt_gen/vector_data.d 

OBJS += \
./rzt_gen/common_data.o \
./rzt_gen/hal_data.o \
./rzt_gen/main.o \
./rzt_gen/pin_data.o \
./rzt_gen/vector_data.o 

MAP += \
rzt2m_cmbacktrace.map 


# Each subdirectory must supply rules for building sources it contributes
rzt_gen/%.o: ../rzt_gen/%.c
	@echo 'Building file: $<'
	$(file > $@.in,-mcpu=cortex-r52 -mthumb -mfloat-abi=hard -mfpu=neon-fp-armv8 -fdiagnostics-parseable-fixits -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -Wnull-dereference -g -gdwarf-4 -D_RENESAS_RZT_ -D_RZT_CORE=CR52_0 -D_RZT_ORDINAL=1 -I"E:\\RS_workspace\\rzt2m_cmbacktrace\\src" -I"." -I"E:\\RS_workspace\\rzt2m_cmbacktrace\\rzt\\fsp\\inc" -I"E:\\RS_workspace\\rzt2m_cmbacktrace\\rzt\\fsp\\inc\\api" -I"E:\\RS_workspace\\rzt2m_cmbacktrace\\rzt\\fsp\\inc\\instances" -I"E:\\RS_workspace\\rzt2m_cmbacktrace\\rzt\\fsp\\src\\bsp\\mcu\\all\\cr" -I"E:\\RS_workspace\\rzt2m_cmbacktrace\\rzt\\arm\\CMSIS_5\\CMSIS\\Core_R\\Include" -I"E:\\RS_workspace\\rzt2m_cmbacktrace\\rzt_gen" -I"E:\\RS_workspace\\rzt2m_cmbacktrace\\rzt_cfg\\fsp_cfg\\bsp" -I"E:\\RS_workspace\\rzt2m_cmbacktrace\\rzt_cfg\\fsp_cfg" -std=c99 -Wno-format-truncation -Wno-stringop-overflow --param=min-pagesize=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<")
	@arm-none-eabi-gcc @"$@.in"

