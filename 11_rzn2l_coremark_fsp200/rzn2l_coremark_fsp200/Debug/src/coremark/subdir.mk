################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/coremark/core_list_join.c \
../src/coremark/core_main.c \
../src/coremark/core_matrix.c \
../src/coremark/core_portme.c \
../src/coremark/core_state.c \
../src/coremark/core_util.c 

LST += \
core_list_join.lst \
core_main.lst \
core_matrix.lst \
core_portme.lst \
core_state.lst \
core_util.lst 

C_DEPS += \
./src/coremark/core_list_join.d \
./src/coremark/core_main.d \
./src/coremark/core_matrix.d \
./src/coremark/core_portme.d \
./src/coremark/core_state.d \
./src/coremark/core_util.d 

OBJS += \
./src/coremark/core_list_join.o \
./src/coremark/core_main.o \
./src/coremark/core_matrix.o \
./src/coremark/core_portme.o \
./src/coremark/core_state.o \
./src/coremark/core_util.o 

MAP += \
rzn2l_coremark_fsp200.map 


# Each subdirectory must supply rules for building sources it contributes
src/coremark/%.o: ../src/coremark/%.c
	@echo 'Building file: $<'
	$(file > $@.in,-mcpu=cortex-r52 -mthumb -mfloat-abi=hard -mfpu=neon-fp-armv8 -fdiagnostics-parseable-fixits -O1 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -Wnull-dereference -g -gdwarf-4 -D_RENESAS_RZN_ -D_RZN_CORE=CR52_0 -D_RZN_ORDINAL=1 -I"E:/RS_workspace/rzn2l_coremark_fsp200/generate" -I"E:/RS_workspace/rzn2l_coremark_fsp200/src" -I"E:\\RS_workspace\\rzn2l_coremark_fsp200\\src" -I"." -I"E:\\RS_workspace\\rzn2l_coremark_fsp200\\rzn\\fsp\\inc" -I"E:\\RS_workspace\\rzn2l_coremark_fsp200\\rzn\\fsp\\inc\\api" -I"E:\\RS_workspace\\rzn2l_coremark_fsp200\\rzn\\fsp\\inc\\instances" -I"E:\\RS_workspace\\rzn2l_coremark_fsp200\\rzn\\fsp\\src\\bsp\\mcu\\all\\cr" -I"E:\\RS_workspace\\rzn2l_coremark_fsp200\\rzn\\arm\\CMSIS_5\\CMSIS\\Core_R\\Include" -I"E:\\RS_workspace\\rzn2l_coremark_fsp200\\rzn_gen" -I"E:\\RS_workspace\\rzn2l_coremark_fsp200\\rzn_cfg\\fsp_cfg\\bsp" -I"E:\\RS_workspace\\rzn2l_coremark_fsp200\\rzn_cfg\\fsp_cfg" -std=c99 -Wno-format-truncation -Wno-stringop-overflow --param=min-pagesize=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<")
	@arm-none-eabi-gcc @"$@.in"

