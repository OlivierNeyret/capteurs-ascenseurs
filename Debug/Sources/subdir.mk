################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/Events.c \
../Sources/accelerometer.c \
../Sources/barometer.c \
../Sources/main.c 

OBJS += \
./Sources/Events.o \
./Sources/accelerometer.o \
./Sources/barometer.o \
./Sources/main.o 

C_DEPS += \
./Sources/Events.d \
./Sources/accelerometer.d \
./Sources/barometer.d \
./Sources/main.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/%.o: ../Sources/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -I"/home/olivier/git/capteurs-ascenseurs/Static_Code/System" -I"/home/olivier/git/capteurs-ascenseurs/Static_Code/PDD" -I"/home/olivier/git/capteurs-ascenseurs/Static_Code/IO_Map" -I"/opt/Freescale/KDS_v3/eclipse/ProcessorExpert/lib/Kinetis/pdd/inc" -I"/home/olivier/git/capteurs-ascenseurs/Sources" -I"/home/olivier/git/capteurs-ascenseurs/Generated_Code" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


