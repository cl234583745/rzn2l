################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../rzn/fsp/src/bsp/mcu/all/bsp_address_expander.c \
../rzn/fsp/src/bsp/mcu/all/bsp_cache.c \
../rzn/fsp/src/bsp/mcu/all/bsp_clocks.c \
../rzn/fsp/src/bsp/mcu/all/bsp_common.c \
../rzn/fsp/src/bsp/mcu/all/bsp_ddr.c \
../rzn/fsp/src/bsp/mcu/all/bsp_ddr_fw_param.c \
../rzn/fsp/src/bsp/mcu/all/bsp_delay.c \
../rzn/fsp/src/bsp/mcu/all/bsp_io.c \
../rzn/fsp/src/bsp/mcu/all/bsp_irq.c \
../rzn/fsp/src/bsp/mcu/all/bsp_register_protection.c \
../rzn/fsp/src/bsp/mcu/all/bsp_reset.c \
../rzn/fsp/src/bsp/mcu/all/bsp_sbrk.c \
../rzn/fsp/src/bsp/mcu/all/bsp_semaphore.c \
../rzn/fsp/src/bsp/mcu/all/bsp_slave_stop.c \
../rzn/fsp/src/bsp/mcu/all/bsp_tzc400.c 

LST += \
bsp_address_expander.lst \
bsp_cache.lst \
bsp_clocks.lst \
bsp_common.lst \
bsp_ddr.lst \
bsp_ddr_fw_param.lst \
bsp_delay.lst \
bsp_io.lst \
bsp_irq.lst \
bsp_register_protection.lst \
bsp_reset.lst \
bsp_sbrk.lst \
bsp_semaphore.lst \
bsp_slave_stop.lst \
bsp_tzc400.lst 

C_DEPS += \
./rzn/fsp/src/bsp/mcu/all/bsp_address_expander.d \
./rzn/fsp/src/bsp/mcu/all/bsp_cache.d \
./rzn/fsp/src/bsp/mcu/all/bsp_clocks.d \
./rzn/fsp/src/bsp/mcu/all/bsp_common.d \
./rzn/fsp/src/bsp/mcu/all/bsp_ddr.d \
./rzn/fsp/src/bsp/mcu/all/bsp_ddr_fw_param.d \
./rzn/fsp/src/bsp/mcu/all/bsp_delay.d \
./rzn/fsp/src/bsp/mcu/all/bsp_io.d \
./rzn/fsp/src/bsp/mcu/all/bsp_irq.d \
./rzn/fsp/src/bsp/mcu/all/bsp_register_protection.d \
./rzn/fsp/src/bsp/mcu/all/bsp_reset.d \
./rzn/fsp/src/bsp/mcu/all/bsp_sbrk.d \
./rzn/fsp/src/bsp/mcu/all/bsp_semaphore.d \
./rzn/fsp/src/bsp/mcu/all/bsp_slave_stop.d \
./rzn/fsp/src/bsp/mcu/all/bsp_tzc400.d 

OBJS += \
./rzn/fsp/src/bsp/mcu/all/bsp_address_expander.o \
./rzn/fsp/src/bsp/mcu/all/bsp_cache.o \
./rzn/fsp/src/bsp/mcu/all/bsp_clocks.o \
./rzn/fsp/src/bsp/mcu/all/bsp_common.o \
./rzn/fsp/src/bsp/mcu/all/bsp_ddr.o \
./rzn/fsp/src/bsp/mcu/all/bsp_ddr_fw_param.o \
./rzn/fsp/src/bsp/mcu/all/bsp_delay.o \
./rzn/fsp/src/bsp/mcu/all/bsp_io.o \
./rzn/fsp/src/bsp/mcu/all/bsp_irq.o \
./rzn/fsp/src/bsp/mcu/all/bsp_register_protection.o \
./rzn/fsp/src/bsp/mcu/all/bsp_reset.o \
./rzn/fsp/src/bsp/mcu/all/bsp_sbrk.o \
./rzn/fsp/src/bsp/mcu/all/bsp_semaphore.o \
./rzn/fsp/src/bsp/mcu/all/bsp_slave_stop.o \
./rzn/fsp/src/bsp/mcu/all/bsp_tzc400.o 

MAP += \
rzn2l_adc_dma_fsp210.map 


# Each subdirectory must supply rules for building sources it contributes
rzn/fsp/src/bsp/mcu/all/%.o: ../rzn/fsp/src/bsp/mcu/all/%.c
	@echo 'Building file: $<'
	$(file > $@.in,-mcpu=cortex-r52 -mthumb -mfloat-abi=hard -mfpu=neon-fp-armv8 -fdiagnostics-parseable-fixits -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -Wnull-dereference -g -D_RENESAS_RZN_ -D_RZN_CORE=CR52_0 -D_RZN_ORDINAL=1 -I"E:\\RS_workspace\\rzn2l_adc_dma_fsp210\\src" -I"." -I"E:\\RS_workspace\\rzn2l_adc_dma_fsp210\\rzn\\fsp\\inc" -I"E:\\RS_workspace\\rzn2l_adc_dma_fsp210\\rzn\\fsp\\inc\\api" -I"E:\\RS_workspace\\rzn2l_adc_dma_fsp210\\rzn\\fsp\\inc\\instances" -I"E:\\RS_workspace\\rzn2l_adc_dma_fsp210\\rzn\\fsp\\src\\bsp\\mcu\\all\\cr" -I"E:\\RS_workspace\\rzn2l_adc_dma_fsp210\\rzn\\arm\\CMSIS_5\\CMSIS\\Core_R\\Include" -I"E:\\RS_workspace\\rzn2l_adc_dma_fsp210\\rzn_gen" -I"E:\\RS_workspace\\rzn2l_adc_dma_fsp210\\rzn_cfg\\fsp_cfg\\bsp" -I"E:\\RS_workspace\\rzn2l_adc_dma_fsp210\\rzn_cfg\\fsp_cfg" -std=c99 -Wno-format-truncation -Wno-stringop-overflow --param=min-pagesize=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<")
	@arm-none-eabi-gcc @"$@.in"

