################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../rzn_gen/common_data.c \
../rzn_gen/hal_data.c \
../rzn_gen/main.c \
../rzn_gen/pin_data.c \
../rzn_gen/vector_data.c 

LST += \
common_data.lst \
hal_data.lst \
main.lst \
pin_data.lst \
vector_data.lst 

C_DEPS += \
./rzn_gen/common_data.d \
./rzn_gen/hal_data.d \
./rzn_gen/main.d \
./rzn_gen/pin_data.d \
./rzn_gen/vector_data.d 

OBJS += \
./rzn_gen/common_data.o \
./rzn_gen/hal_data.o \
./rzn_gen/main.o \
./rzn_gen/pin_data.o \
./rzn_gen/vector_data.o 

MAP += \
rzn2l_gpt_dma.map 


# Each subdirectory must supply rules for building sources it contributes
rzn_gen/%.o: ../rzn_gen/%.c
	@echo 'Building file: $<'
	$(file > $@.in,-mcpu=cortex-r52 -mthumb -mfloat-abi=hard -mfpu=neon-fp-armv8 -fdiagnostics-parseable-fixits -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -Wnull-dereference -g -D_RENESAS_RZN_ -D_RZN_CORE=CR52_0 -D_RZN_ORDINAL=1 -I"D:/RS_workspace/rzn2l_gpt_dma/generate" -I"D:/RS_workspace/rzn2l_gpt_dma/src" -I"D:\\RS_workspace\\rzn2l_gpt_dma\\src" -I"." -I"D:\\RS_workspace\\rzn2l_gpt_dma\\rzn\\fsp\\inc" -I"D:\\RS_workspace\\rzn2l_gpt_dma\\rzn\\fsp\\inc\\api" -I"D:\\RS_workspace\\rzn2l_gpt_dma\\rzn\\fsp\\inc\\instances" -I"D:\\RS_workspace\\rzn2l_gpt_dma\\rzn\\arm\\CMSIS_5\\CMSIS\\Core_R\\Include" -I"D:\\RS_workspace\\rzn2l_gpt_dma\\rzn_cfg\\fsp_cfg\\bsp" -I"D:\\RS_workspace\\rzn2l_gpt_dma\\rzn_gen" -I"D:\\RS_workspace\\rzn2l_gpt_dma\\rzn_cfg\\fsp_cfg" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<")
	@arm-none-eabi-gcc @"$@.in"

