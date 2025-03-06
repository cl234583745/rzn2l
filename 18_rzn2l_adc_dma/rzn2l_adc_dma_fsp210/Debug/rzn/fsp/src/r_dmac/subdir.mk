################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../rzn/fsp/src/r_dmac/r_dmac.c 

LST += \
r_dmac.lst 

C_DEPS += \
./rzn/fsp/src/r_dmac/r_dmac.d 

OBJS += \
./rzn/fsp/src/r_dmac/r_dmac.o 

MAP += \
rzn2l_adc_dma_fsp210.map 


# Each subdirectory must supply rules for building sources it contributes
rzn/fsp/src/r_dmac/%.o: ../rzn/fsp/src/r_dmac/%.c
	@echo 'Building file: $<'
	$(file > $@.in,-mcpu=cortex-r52 -mthumb -mfloat-abi=hard -mfpu=neon-fp-armv8 -fdiagnostics-parseable-fixits -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -Wnull-dereference -g -D_RENESAS_RZN_ -D_RZN_CORE=CR52_0 -D_RZN_ORDINAL=1 -I"E:\\RS_workspace\\rzn2l_adc_dma_fsp210\\src" -I"." -I"E:\\RS_workspace\\rzn2l_adc_dma_fsp210\\rzn\\fsp\\inc" -I"E:\\RS_workspace\\rzn2l_adc_dma_fsp210\\rzn\\fsp\\inc\\api" -I"E:\\RS_workspace\\rzn2l_adc_dma_fsp210\\rzn\\fsp\\inc\\instances" -I"E:\\RS_workspace\\rzn2l_adc_dma_fsp210\\rzn\\fsp\\src\\bsp\\mcu\\all\\cr" -I"E:\\RS_workspace\\rzn2l_adc_dma_fsp210\\rzn\\arm\\CMSIS_5\\CMSIS\\Core_R\\Include" -I"E:\\RS_workspace\\rzn2l_adc_dma_fsp210\\rzn_gen" -I"E:\\RS_workspace\\rzn2l_adc_dma_fsp210\\rzn_cfg\\fsp_cfg\\bsp" -I"E:\\RS_workspace\\rzn2l_adc_dma_fsp210\\rzn_cfg\\fsp_cfg" -std=c99 -Wno-format-truncation -Wno-stringop-overflow --param=min-pagesize=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<")
	@arm-none-eabi-gcc @"$@.in"

