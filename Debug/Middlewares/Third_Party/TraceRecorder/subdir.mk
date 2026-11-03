################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/TraceRecorder/trcAssert.c \
../Middlewares/Third_Party/TraceRecorder/trcCounter.c \
../Middlewares/Third_Party/TraceRecorder/trcDependency.c \
../Middlewares/Third_Party/TraceRecorder/trcDiagnostics.c \
../Middlewares/Third_Party/TraceRecorder/trcEntryTable.c \
../Middlewares/Third_Party/TraceRecorder/trcError.c \
../Middlewares/Third_Party/TraceRecorder/trcEvent.c \
../Middlewares/Third_Party/TraceRecorder/trcEventBuffer.c \
../Middlewares/Third_Party/TraceRecorder/trcExtension.c \
../Middlewares/Third_Party/TraceRecorder/trcHardwarePort.c \
../Middlewares/Third_Party/TraceRecorder/trcHeap.c \
../Middlewares/Third_Party/TraceRecorder/trcISR.c \
../Middlewares/Third_Party/TraceRecorder/trcInternalEventBuffer.c \
../Middlewares/Third_Party/TraceRecorder/trcInterval.c \
../Middlewares/Third_Party/TraceRecorder/trcKernelPort.c \
../Middlewares/Third_Party/TraceRecorder/trcMultiCoreEventBuffer.c \
../Middlewares/Third_Party/TraceRecorder/trcObject.c \
../Middlewares/Third_Party/TraceRecorder/trcPrint.c \
../Middlewares/Third_Party/TraceRecorder/trcRunnable.c \
../Middlewares/Third_Party/TraceRecorder/trcStackMonitor.c \
../Middlewares/Third_Party/TraceRecorder/trcStateMachine.c \
../Middlewares/Third_Party/TraceRecorder/trcStaticBuffer.c \
../Middlewares/Third_Party/TraceRecorder/trcStreamingRecorder.c \
../Middlewares/Third_Party/TraceRecorder/trcString.c \
../Middlewares/Third_Party/TraceRecorder/trcTask.c \
../Middlewares/Third_Party/TraceRecorder/trcTaskMonitor.c \
../Middlewares/Third_Party/TraceRecorder/trcTimestamp.c 

C_DEPS += \
./Middlewares/Third_Party/TraceRecorder/trcAssert.d \
./Middlewares/Third_Party/TraceRecorder/trcCounter.d \
./Middlewares/Third_Party/TraceRecorder/trcDependency.d \
./Middlewares/Third_Party/TraceRecorder/trcDiagnostics.d \
./Middlewares/Third_Party/TraceRecorder/trcEntryTable.d \
./Middlewares/Third_Party/TraceRecorder/trcError.d \
./Middlewares/Third_Party/TraceRecorder/trcEvent.d \
./Middlewares/Third_Party/TraceRecorder/trcEventBuffer.d \
./Middlewares/Third_Party/TraceRecorder/trcExtension.d \
./Middlewares/Third_Party/TraceRecorder/trcHardwarePort.d \
./Middlewares/Third_Party/TraceRecorder/trcHeap.d \
./Middlewares/Third_Party/TraceRecorder/trcISR.d \
./Middlewares/Third_Party/TraceRecorder/trcInternalEventBuffer.d \
./Middlewares/Third_Party/TraceRecorder/trcInterval.d \
./Middlewares/Third_Party/TraceRecorder/trcKernelPort.d \
./Middlewares/Third_Party/TraceRecorder/trcMultiCoreEventBuffer.d \
./Middlewares/Third_Party/TraceRecorder/trcObject.d \
./Middlewares/Third_Party/TraceRecorder/trcPrint.d \
./Middlewares/Third_Party/TraceRecorder/trcRunnable.d \
./Middlewares/Third_Party/TraceRecorder/trcStackMonitor.d \
./Middlewares/Third_Party/TraceRecorder/trcStateMachine.d \
./Middlewares/Third_Party/TraceRecorder/trcStaticBuffer.d \
./Middlewares/Third_Party/TraceRecorder/trcStreamingRecorder.d \
./Middlewares/Third_Party/TraceRecorder/trcString.d \
./Middlewares/Third_Party/TraceRecorder/trcTask.d \
./Middlewares/Third_Party/TraceRecorder/trcTaskMonitor.d \
./Middlewares/Third_Party/TraceRecorder/trcTimestamp.d 

OBJS += \
./Middlewares/Third_Party/TraceRecorder/trcAssert.o \
./Middlewares/Third_Party/TraceRecorder/trcCounter.o \
./Middlewares/Third_Party/TraceRecorder/trcDependency.o \
./Middlewares/Third_Party/TraceRecorder/trcDiagnostics.o \
./Middlewares/Third_Party/TraceRecorder/trcEntryTable.o \
./Middlewares/Third_Party/TraceRecorder/trcError.o \
./Middlewares/Third_Party/TraceRecorder/trcEvent.o \
./Middlewares/Third_Party/TraceRecorder/trcEventBuffer.o \
./Middlewares/Third_Party/TraceRecorder/trcExtension.o \
./Middlewares/Third_Party/TraceRecorder/trcHardwarePort.o \
./Middlewares/Third_Party/TraceRecorder/trcHeap.o \
./Middlewares/Third_Party/TraceRecorder/trcISR.o \
./Middlewares/Third_Party/TraceRecorder/trcInternalEventBuffer.o \
./Middlewares/Third_Party/TraceRecorder/trcInterval.o \
./Middlewares/Third_Party/TraceRecorder/trcKernelPort.o \
./Middlewares/Third_Party/TraceRecorder/trcMultiCoreEventBuffer.o \
./Middlewares/Third_Party/TraceRecorder/trcObject.o \
./Middlewares/Third_Party/TraceRecorder/trcPrint.o \
./Middlewares/Third_Party/TraceRecorder/trcRunnable.o \
./Middlewares/Third_Party/TraceRecorder/trcStackMonitor.o \
./Middlewares/Third_Party/TraceRecorder/trcStateMachine.o \
./Middlewares/Third_Party/TraceRecorder/trcStaticBuffer.o \
./Middlewares/Third_Party/TraceRecorder/trcStreamingRecorder.o \
./Middlewares/Third_Party/TraceRecorder/trcString.o \
./Middlewares/Third_Party/TraceRecorder/trcTask.o \
./Middlewares/Third_Party/TraceRecorder/trcTaskMonitor.o \
./Middlewares/Third_Party/TraceRecorder/trcTimestamp.o 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/TraceRecorder/%.o Middlewares/Third_Party/TraceRecorder/%.su Middlewares/Third_Party/TraceRecorder/%.cyclo: ../Middlewares/Third_Party/TraceRecorder/%.c Middlewares/Third_Party/TraceRecorder/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G491xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/TraceRecorder/include -I../Middlewares/Third_Party/TraceRecorder/config -I../Middlewares/Third_Party/TraceRecorder/streamports/RingBuffer/include -I../Middlewares/Third_Party/TraceRecorder/streamports/RingBuffer/config -O0 -ffunction-sections -fdata-sections -Wall -u _printf_float -fno-builtin-printf -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-Third_Party-2f-TraceRecorder

clean-Middlewares-2f-Third_Party-2f-TraceRecorder:
	-$(RM) ./Middlewares/Third_Party/TraceRecorder/trcAssert.cyclo ./Middlewares/Third_Party/TraceRecorder/trcAssert.d ./Middlewares/Third_Party/TraceRecorder/trcAssert.o ./Middlewares/Third_Party/TraceRecorder/trcAssert.su ./Middlewares/Third_Party/TraceRecorder/trcCounter.cyclo ./Middlewares/Third_Party/TraceRecorder/trcCounter.d ./Middlewares/Third_Party/TraceRecorder/trcCounter.o ./Middlewares/Third_Party/TraceRecorder/trcCounter.su ./Middlewares/Third_Party/TraceRecorder/trcDependency.cyclo ./Middlewares/Third_Party/TraceRecorder/trcDependency.d ./Middlewares/Third_Party/TraceRecorder/trcDependency.o ./Middlewares/Third_Party/TraceRecorder/trcDependency.su ./Middlewares/Third_Party/TraceRecorder/trcDiagnostics.cyclo ./Middlewares/Third_Party/TraceRecorder/trcDiagnostics.d ./Middlewares/Third_Party/TraceRecorder/trcDiagnostics.o ./Middlewares/Third_Party/TraceRecorder/trcDiagnostics.su ./Middlewares/Third_Party/TraceRecorder/trcEntryTable.cyclo ./Middlewares/Third_Party/TraceRecorder/trcEntryTable.d ./Middlewares/Third_Party/TraceRecorder/trcEntryTable.o ./Middlewares/Third_Party/TraceRecorder/trcEntryTable.su ./Middlewares/Third_Party/TraceRecorder/trcError.cyclo ./Middlewares/Third_Party/TraceRecorder/trcError.d ./Middlewares/Third_Party/TraceRecorder/trcError.o ./Middlewares/Third_Party/TraceRecorder/trcError.su ./Middlewares/Third_Party/TraceRecorder/trcEvent.cyclo ./Middlewares/Third_Party/TraceRecorder/trcEvent.d ./Middlewares/Third_Party/TraceRecorder/trcEvent.o ./Middlewares/Third_Party/TraceRecorder/trcEvent.su ./Middlewares/Third_Party/TraceRecorder/trcEventBuffer.cyclo ./Middlewares/Third_Party/TraceRecorder/trcEventBuffer.d ./Middlewares/Third_Party/TraceRecorder/trcEventBuffer.o ./Middlewares/Third_Party/TraceRecorder/trcEventBuffer.su ./Middlewares/Third_Party/TraceRecorder/trcExtension.cyclo ./Middlewares/Third_Party/TraceRecorder/trcExtension.d ./Middlewares/Third_Party/TraceRecorder/trcExtension.o ./Middlewares/Third_Party/TraceRecorder/trcExtension.su ./Middlewares/Third_Party/TraceRecorder/trcHardwarePort.cyclo ./Middlewares/Third_Party/TraceRecorder/trcHardwarePort.d ./Middlewares/Third_Party/TraceRecorder/trcHardwarePort.o ./Middlewares/Third_Party/TraceRecorder/trcHardwarePort.su ./Middlewares/Third_Party/TraceRecorder/trcHeap.cyclo ./Middlewares/Third_Party/TraceRecorder/trcHeap.d ./Middlewares/Third_Party/TraceRecorder/trcHeap.o ./Middlewares/Third_Party/TraceRecorder/trcHeap.su ./Middlewares/Third_Party/TraceRecorder/trcISR.cyclo ./Middlewares/Third_Party/TraceRecorder/trcISR.d ./Middlewares/Third_Party/TraceRecorder/trcISR.o ./Middlewares/Third_Party/TraceRecorder/trcISR.su ./Middlewares/Third_Party/TraceRecorder/trcInternalEventBuffer.cyclo ./Middlewares/Third_Party/TraceRecorder/trcInternalEventBuffer.d ./Middlewares/Third_Party/TraceRecorder/trcInternalEventBuffer.o ./Middlewares/Third_Party/TraceRecorder/trcInternalEventBuffer.su ./Middlewares/Third_Party/TraceRecorder/trcInterval.cyclo ./Middlewares/Third_Party/TraceRecorder/trcInterval.d ./Middlewares/Third_Party/TraceRecorder/trcInterval.o ./Middlewares/Third_Party/TraceRecorder/trcInterval.su ./Middlewares/Third_Party/TraceRecorder/trcKernelPort.cyclo ./Middlewares/Third_Party/TraceRecorder/trcKernelPort.d ./Middlewares/Third_Party/TraceRecorder/trcKernelPort.o ./Middlewares/Third_Party/TraceRecorder/trcKernelPort.su ./Middlewares/Third_Party/TraceRecorder/trcMultiCoreEventBuffer.cyclo ./Middlewares/Third_Party/TraceRecorder/trcMultiCoreEventBuffer.d ./Middlewares/Third_Party/TraceRecorder/trcMultiCoreEventBuffer.o ./Middlewares/Third_Party/TraceRecorder/trcMultiCoreEventBuffer.su ./Middlewares/Third_Party/TraceRecorder/trcObject.cyclo ./Middlewares/Third_Party/TraceRecorder/trcObject.d ./Middlewares/Third_Party/TraceRecorder/trcObject.o ./Middlewares/Third_Party/TraceRecorder/trcObject.su ./Middlewares/Third_Party/TraceRecorder/trcPrint.cyclo ./Middlewares/Third_Party/TraceRecorder/trcPrint.d ./Middlewares/Third_Party/TraceRecorder/trcPrint.o ./Middlewares/Third_Party/TraceRecorder/trcPrint.su ./Middlewares/Third_Party/TraceRecorder/trcRunnable.cyclo ./Middlewares/Third_Party/TraceRecorder/trcRunnable.d ./Middlewares/Third_Party/TraceRecorder/trcRunnable.o ./Middlewares/Third_Party/TraceRecorder/trcRunnable.su ./Middlewares/Third_Party/TraceRecorder/trcStackMonitor.cyclo ./Middlewares/Third_Party/TraceRecorder/trcStackMonitor.d ./Middlewares/Third_Party/TraceRecorder/trcStackMonitor.o ./Middlewares/Third_Party/TraceRecorder/trcStackMonitor.su ./Middlewares/Third_Party/TraceRecorder/trcStateMachine.cyclo ./Middlewares/Third_Party/TraceRecorder/trcStateMachine.d ./Middlewares/Third_Party/TraceRecorder/trcStateMachine.o ./Middlewares/Third_Party/TraceRecorder/trcStateMachine.su ./Middlewares/Third_Party/TraceRecorder/trcStaticBuffer.cyclo ./Middlewares/Third_Party/TraceRecorder/trcStaticBuffer.d ./Middlewares/Third_Party/TraceRecorder/trcStaticBuffer.o ./Middlewares/Third_Party/TraceRecorder/trcStaticBuffer.su ./Middlewares/Third_Party/TraceRecorder/trcStreamingRecorder.cyclo ./Middlewares/Third_Party/TraceRecorder/trcStreamingRecorder.d ./Middlewares/Third_Party/TraceRecorder/trcStreamingRecorder.o ./Middlewares/Third_Party/TraceRecorder/trcStreamingRecorder.su ./Middlewares/Third_Party/TraceRecorder/trcString.cyclo ./Middlewares/Third_Party/TraceRecorder/trcString.d ./Middlewares/Third_Party/TraceRecorder/trcString.o ./Middlewares/Third_Party/TraceRecorder/trcString.su ./Middlewares/Third_Party/TraceRecorder/trcTask.cyclo ./Middlewares/Third_Party/TraceRecorder/trcTask.d ./Middlewares/Third_Party/TraceRecorder/trcTask.o ./Middlewares/Third_Party/TraceRecorder/trcTask.su ./Middlewares/Third_Party/TraceRecorder/trcTaskMonitor.cyclo ./Middlewares/Third_Party/TraceRecorder/trcTaskMonitor.d ./Middlewares/Third_Party/TraceRecorder/trcTaskMonitor.o ./Middlewares/Third_Party/TraceRecorder/trcTaskMonitor.su ./Middlewares/Third_Party/TraceRecorder/trcTimestamp.cyclo
	-$(RM) ./Middlewares/Third_Party/TraceRecorder/trcTimestamp.d ./Middlewares/Third_Party/TraceRecorder/trcTimestamp.o ./Middlewares/Third_Party/TraceRecorder/trcTimestamp.su

.PHONY: clean-Middlewares-2f-Third_Party-2f-TraceRecorder

