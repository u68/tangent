################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -f

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

-include ../makefile.defs

# Export environment variables
export DCL=C:/lapis/LEXIDE/Dcl
export INCLU8=C:/lapis/LEXIDE/Inc;C:/lapis/LEXIDE/BuildTools/Ver.20231124/Inc/U16
export LIBU8=C:/lapis/LEXIDE/BuildTools/Ver.20231124/Lib/U16
export PATH=C:/lapis/LEXIDE/Bin;C:/lapis/LEXIDE/BuildTools/Ver.20231124/Bin;C:/lapis/LEXIDE/Utilities/Bin

# Add inputs and outputs from these tool invocations to the build variables 
OBJHEX += \
Tangent.HEX \

ABS += \
Tangent.abs \

MAP += \
Tangent.map \


# All Target
all: $(OBJHEX)

# Tool invocations
Tangent.HEX: $(ABS)
	@echo 'Building target: $@'
	@echo 'Invoking: Object converter'
	CnvOHU8 -H $(ABS) "Tangent.HEX";
	@echo 'Finished building target: $@'
	@echo ' '

Tangent.abs: $(OBJS)
	@echo 'Invoking: Linker'
	rlu8 -D -SD -NS -CC -ABW $(OBJS__QUOTED),"Tangent.abs",,"LU8100LW.LIB";
	@echo 'Finished building: $@'
	@echo ' '

Tangent.map: Tangent.abs $(OBJS)

# Other Targets
clean:
	-$(RM) "./Tangent.sc" "./io.prn" "./main.prn" "./startup.prn" "./thefont.prn" "./vm.prn" "./Tangent.abs" "./TangentRB.s" "./TangentRA.s" "./TangentRA.log" "./io.asm" "./main.asm" "./thefont.asm" "./vm.asm" "./Tangent.HEX" "./io.obj" "./main.obj" "./startup.obj" "./thefont.obj" "./vm.obj" "./io.i" "./main.i" "./thefont.i" "./vm.i" "./Tangent.map" 
	-@echo ' '

.PHONY: all clean dependents

-include ../makefile.targets
