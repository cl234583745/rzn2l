################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../rzt/fsp/src/r_dmac/r_dmac.c 

LST += \
r_dmac.lst 

C_DEPS += \
./rzt/fsp/src/r_dmac/r_dmac.d 

OBJS += \
./rzt/fsp/src/r_dmac/r_dmac.o 

MAP += \
rzt2m_gpt_dma.map 


# Each subdirectory must supply rules for building sources it contributes
rzt/fsp/src/r_dmac/%.o: ../rzt/fsp/src/r_dmac/%.c
	@echo 'Building file: $<'
	$(file > $@.in,-mcpu=cortex-r52 -mthumb -mfloat-abi=hard -mfpu=neon-fp-armv8 -fdiagnostics-parseable-fixits -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -Wnull-dereference -g -D_RENESAS_RZT_ -D_RZT_CORE=CR52_0 -D_RZT_ORDINAL=1 -I"E:\\RS_workspace\\rzt2m_gpt_dma\\src" -I"." -I"E:\\RS_workspace\\rzt2m_gpt_dma\\rzt\\fsp\\inc" -I"E:\\RS_workspace\\rzt2m_gpt_dma\\rzt\\fsp\\inc\\api" -I"E:\\RS_workspace\\rzt2m_gpt_dma\\rzt\\fsp\\inc\\instances" -I"E:\\RS_workspace\\rzt2m_gpt_dma\\rzt\\arm\\CMSIS_5\\CMSIS\\Core_R\\Include" -I"E:\\RS_workspace\\rzt2m_gpt_dma\\rzt_gen" -I"E:\\RS_workspace\\rzt2m_gpt_dma\\rzt_cfg\\fsp_cfg\\bsp" -I"E:\\RS_workspace\\rzt2m_gpt_dma\\rzt_cfg\\fsp_cfg" -std=c99 -Wno-format-truncation -Wno-stringop-overflow --param=min-pagesize=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<")
	@arm-none-eabi-gcc @"$@.in"

