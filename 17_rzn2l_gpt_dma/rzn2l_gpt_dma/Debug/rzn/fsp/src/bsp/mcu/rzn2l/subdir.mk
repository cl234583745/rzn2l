################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../rzn/fsp/src/bsp/mcu/rzn2l/bsp_cache.c \
../rzn/fsp/src/bsp/mcu/rzn2l/bsp_loader_param.c 

LST += \
bsp_cache.lst \
bsp_loader_param.lst 

C_DEPS += \
./rzn/fsp/src/bsp/mcu/rzn2l/bsp_cache.d \
./rzn/fsp/src/bsp/mcu/rzn2l/bsp_loader_param.d 

OBJS += \
./rzn/fsp/src/bsp/mcu/rzn2l/bsp_cache.o \
./rzn/fsp/src/bsp/mcu/rzn2l/bsp_loader_param.o 

MAP += \
rzn2l_gpt_dma.map 


# Each subdirectory must supply rules for building sources it contributes
rzn/fsp/src/bsp/mcu/rzn2l/%.o: ../rzn/fsp/src/bsp/mcu/rzn2l/%.c
	@echo 'Building file: $<'
	$(file > $@.in,-mcpu=cortex-r52 -mthumb -mfloat-abi=hard -mfpu=neon-fp-armv8 -fdiagnostics-parseable-fixits -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -Wnull-dereference -g -D_RENESAS_RZN_ -D_RZN_CORE=CR52_0 -D_RZN_ORDINAL=1 -I"D:/RS_workspace/rzn2l_gpt_dma/generate" -I"D:/RS_workspace/rzn2l_gpt_dma/src" -I"D:\\RS_workspace\\rzn2l_gpt_dma\\src" -I"." -I"D:\\RS_workspace\\rzn2l_gpt_dma\\rzn\\fsp\\inc" -I"D:\\RS_workspace\\rzn2l_gpt_dma\\rzn\\fsp\\inc\\api" -I"D:\\RS_workspace\\rzn2l_gpt_dma\\rzn\\fsp\\inc\\instances" -I"D:\\RS_workspace\\rzn2l_gpt_dma\\rzn\\arm\\CMSIS_5\\CMSIS\\Core_R\\Include" -I"D:\\RS_workspace\\rzn2l_gpt_dma\\rzn_cfg\\fsp_cfg\\bsp" -I"D:\\RS_workspace\\rzn2l_gpt_dma\\rzn_gen" -I"D:\\RS_workspace\\rzn2l_gpt_dma\\rzn_cfg\\fsp_cfg" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<")
	@arm-none-eabi-gcc @"$@.in"

