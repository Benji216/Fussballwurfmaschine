################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=
LINKER_SCRIPT_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS := 


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../Motoransteuerung.c \
../LCD1zeil.c \
../main.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
Motoransteuerung.o \
LCD1zeil.o \
main.o

OBJS_AS_ARGS +=  \
Motoransteuerung.o \
LCD1zeil.o \
main.o

C_DEPS +=  \
Motoransteuerung.d \
LCD1zeil.d \
main.d

C_DEPS_AS_ARGS +=  \
Motoransteuerung.d \
LCD1zeil.d \
main.d

OUTPUT_FILE_PATH +=Motoransteuerung.elf

OUTPUT_FILE_PATH_AS_ARGS +=Motoransteuerung.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

LINKER_SCRIPT_DEP+= 


# AVR32/GNU C Compiler
./Motoransteuerung.o: .././Motoransteuerung.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 5.4.0
	$(QUOTE)C:\Programmieren\Atmel Studio_7.0\Atmel Studio_7.0\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DDEBUG  -I"C:\Programmieren\Atmel Studio_7.0\Atmel Studio_7.0\7.0\Packs\atmel\ATmega_DFP\1.2.209\include"  -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=atmega32u4 -B "C:\Programmieren\Atmel Studio_7.0\Atmel Studio_7.0\7.0\Packs\atmel\ATmega_DFP\1.2.209\gcc\dev\atmega32u4" -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./LCD1zeil.o: .././LCD1zeil.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 5.4.0
	$(QUOTE)C:\Programmieren\Atmel Studio_7.0\Atmel Studio_7.0\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DDEBUG  -I"C:\Programmieren\Atmel Studio_7.0\Atmel Studio_7.0\7.0\Packs\atmel\ATmega_DFP\1.2.209\include"  -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=atmega32u4 -B "C:\Programmieren\Atmel Studio_7.0\Atmel Studio_7.0\7.0\Packs\atmel\ATmega_DFP\1.2.209\gcc\dev\atmega32u4" -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./main.o: .././main.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 5.4.0
	$(QUOTE)C:\Programmieren\Atmel Studio_7.0\Atmel Studio_7.0\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DDEBUG  -I"C:\Programmieren\Atmel Studio_7.0\Atmel Studio_7.0\7.0\Packs\atmel\ATmega_DFP\1.2.209\include"  -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=atmega32u4 -B "C:\Programmieren\Atmel Studio_7.0\Atmel Studio_7.0\7.0\Packs\atmel\ATmega_DFP\1.2.209\gcc\dev\atmega32u4" -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	




# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP) $(LINKER_SCRIPT_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker : 5.4.0
	$(QUOTE)C:\Programmieren\Atmel Studio_7.0\Atmel Studio_7.0\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="Motoransteuerung.map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mmcu=atmega32u4 -B "C:\Programmieren\Atmel Studio_7.0\Atmel Studio_7.0\7.0\Packs\atmel\ATmega_DFP\1.2.209\gcc\dev\atmega32u4"  
	@echo Finished building target: $@
	"C:\Programmieren\Atmel Studio_7.0\Atmel Studio_7.0\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "Motoransteuerung.elf" "Motoransteuerung.hex"
	"C:\Programmieren\Atmel Studio_7.0\Atmel Studio_7.0\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "Motoransteuerung.elf" "Motoransteuerung.eep" || exit 0
	"C:\Programmieren\Atmel Studio_7.0\Atmel Studio_7.0\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "Motoransteuerung.elf" > "Motoransteuerung.lss"
	"C:\Programmieren\Atmel Studio_7.0\Atmel Studio_7.0\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "Motoransteuerung.elf" "Motoransteuerung.srec"
	"C:\Programmieren\Atmel Studio_7.0\Atmel Studio_7.0\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-size.exe" "Motoransteuerung.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "Motoransteuerung.elf" "Motoransteuerung.a" "Motoransteuerung.hex" "Motoransteuerung.lss" "Motoransteuerung.eep" "Motoransteuerung.map" "Motoransteuerung.srec" "Motoransteuerung.usersignatures"
	