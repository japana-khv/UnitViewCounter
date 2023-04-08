################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/CellTree.cpp \
../src/CellVector.cpp \
../src/CheckVisible.cpp \
../src/CrossingChecker.cpp \
../src/GridCell.cpp \
../src/PointsVector.cpp \
../src/UnitDatabase.cpp \
../src/geometry.cpp \
../src/main.cpp \
../src/task.cpp 

CPP_DEPS += \
./src/CellTree.d \
./src/CellVector.d \
./src/CheckVisible.d \
./src/CrossingChecker.d \
./src/GridCell.d \
./src/PointsVector.d \
./src/UnitDatabase.d \
./src/geometry.d \
./src/main.d \
./src/task.d 

OBJS += \
./src/CellTree.o \
./src/CellVector.o \
./src/CheckVisible.o \
./src/CrossingChecker.o \
./src/GridCell.o \
./src/PointsVector.o \
./src/UnitDatabase.o \
./src/geometry.o \
./src/main.o \
./src/task.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -march=native -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/CellTree.d ./src/CellTree.o ./src/CellVector.d ./src/CellVector.o ./src/CheckVisible.d ./src/CheckVisible.o ./src/CrossingChecker.d ./src/CrossingChecker.o ./src/GridCell.d ./src/GridCell.o ./src/PointsVector.d ./src/PointsVector.o ./src/UnitDatabase.d ./src/UnitDatabase.o ./src/geometry.d ./src/geometry.o ./src/main.d ./src/main.o ./src/task.d ./src/task.o

.PHONY: clean-src

