################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../rzn/fsp/src/bsp/cmsis/Device/RENESAS/Source/cr/startup_core.c \
../rzn/fsp/src/bsp/cmsis/Device/RENESAS/Source/cr/system_core.c 

LST += \
startup_core.lst \
system_core.lst 

C_DEPS += \
./rzn/fsp/src/bsp/cmsis/Device/RENESAS/Source/cr/startup_core.d \
./rzn/fsp/src/bsp/cmsis/Device/RENESAS/Source/cr/system_core.d 

OBJS += \
./rzn/fsp/src/bsp/cmsis/Device/RENESAS/Source/cr/startup_core.o \
./rzn/fsp/src/bsp/cmsis/Device/RENESAS/Source/cr/system_core.o 

MAP += \
rzn2l_uart_dma_recv_volatile.map 


# Each subdirectory must supply rules for building sources it contributes
rzn/fsp/src/bsp/cmsis/Device/RENESAS/Source/cr/%.o: ../rzn/fsp/src/bsp/cmsis/Device/RENESAS/Source/cr/%.c
	@echo 'Building file: $<'
	$(file > $@.in,-mcpu=cortex-r52 -mthumb -mfloat-abi=hard -mfpu=neon-fp-armv8 -fdiagnostics-parseable-fixits -O1 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -Wnull-dereference -g -gdwarf-4 -D_RENESAS_RZN_ -D_RZN_CORE=CR52_0 -D_RZN_ORDINAL=1 -I"E:/RS_workspace/rzn2l_uart_dma_recv_volatile/generate" -I"E:/RS_workspace/rzn2l_uart_dma_recv_volatile/src" -I"E:\\RS_workspace\\rzn2l_uart_dma_recv_volatile\\src" -I"." -I"E:\\RS_workspace\\rzn2l_uart_dma_recv_volatile\\rzn\\fsp\\inc" -I"E:\\RS_workspace\\rzn2l_uart_dma_recv_volatile\\rzn\\fsp\\inc\\api" -I"E:\\RS_workspace\\rzn2l_uart_dma_recv_volatile\\rzn\\fsp\\inc\\instances" -I"E:\\RS_workspace\\rzn2l_uart_dma_recv_volatile\\rzn\\fsp\\src\\bsp\\mcu\\all\\cr" -I"E:\\RS_workspace\\rzn2l_uart_dma_recv_volatile\\rzn\\arm\\CMSIS_5\\CMSIS\\Core_R\\Include" -I"E:\\RS_workspace\\rzn2l_uart_dma_recv_volatile\\rzn_gen" -I"E:\\RS_workspace\\rzn2l_uart_dma_recv_volatile\\rzn_cfg\\fsp_cfg\\bsp" -I"E:\\RS_workspace\\rzn2l_uart_dma_recv_volatile\\rzn_cfg\\fsp_cfg" -std=c99 -Wno-format-truncation -Wno-stringop-overflow --param=min-pagesize=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<")
	@arm-none-eabi-gcc @"$@.in"

