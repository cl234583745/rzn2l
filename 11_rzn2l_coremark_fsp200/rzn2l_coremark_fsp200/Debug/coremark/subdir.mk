################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../coremark/core_list_join.c \
../coremark/core_main.c \
../coremark/core_matrix.c \
../coremark/core_portme.c \
../coremark/core_state.c \
../coremark/core_util.c \
../coremark/func_atcm.c 

LST += \
core_list_join.lst \
core_main.lst \
core_matrix.lst \
core_portme.lst \
core_state.lst \
core_util.lst \
func_atcm.lst 

C_DEPS += \
./coremark/core_list_join.d \
./coremark/core_main.d \
./coremark/core_matrix.d \
./coremark/core_portme.d \
./coremark/core_state.d \
./coremark/core_util.d \
./coremark/func_atcm.d 

OBJS += \
./coremark/core_list_join.o \
./coremark/core_main.o \
./coremark/core_matrix.o \
./coremark/core_portme.o \
./coremark/core_state.o \
./coremark/core_util.o \
./coremark/func_atcm.o 

MAP += \
rzn2l_coremark_fsp200.map 


# Each subdirectory must supply rules for building sources it contributes
coremark/%.o: ../coremark/%.c
	@echo 'Building file: $<'
	$(file > $@.in,-mcpu=cortex-r52 -mthumb -mfloat-abi=hard -mfpu=neon-fp-armv8 -fdiagnostics-parseable-fixits -Ofast -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -Wnull-dereference -g -gdwarf-4 -D_RENESAS_RZN_ -D_RZN_CORE=CR52_0 -D_RZN_ORDINAL=1 -DFSP_XSPI0_BOOT_SRAM_ATCM=1 -I"E:/RS_workspace/rzn2l_coremark_fsp200/generate" -I"E:/RS_workspace/rzn2l_coremark_fsp200/src" -I"E:\\RS_workspace\\rzn2l_coremark_fsp200\\src" -I"." -I"E:\\RS_workspace\\rzn2l_coremark_fsp200\\rzn\\fsp\\inc" -I"E:\\RS_workspace\\rzn2l_coremark_fsp200\\rzn\\fsp\\inc\\api" -I"E:\\RS_workspace\\rzn2l_coremark_fsp200\\rzn\\fsp\\inc\\instances" -I"E:\\RS_workspace\\rzn2l_coremark_fsp200\\rzn\\fsp\\src\\bsp\\mcu\\all\\cr" -I"E:\\RS_workspace\\rzn2l_coremark_fsp200\\rzn\\arm\\CMSIS_5\\CMSIS\\Core_R\\Include" -I"E:\\RS_workspace\\rzn2l_coremark_fsp200\\rzn_gen" -I"E:\\RS_workspace\\rzn2l_coremark_fsp200\\rzn_cfg\\fsp_cfg\\bsp" -I"E:\\RS_workspace\\rzn2l_coremark_fsp200\\rzn_cfg\\fsp_cfg" -std=c99 -Wno-format-truncation -Wno-stringop-overflow --param=min-pagesize=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<")
	@arm-none-eabi-gcc @"$@.in"

