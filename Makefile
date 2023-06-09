# ******************************************************************************************
#   Filename    : Makefile
#
#   Author      : Chalandi Amine
#
#   Owner       : Chalandi Amine
#
#   Date        : 22.11.2022
#                 15.04.2023 Adapted for Osek_pi_crunch_cm3 (ckormanyos)
#
#   Description : Build system
#
# ******************************************************************************************

############################################################################################
# Defines
############################################################################################

PRJ_NAME   = Osek_pi_crunch_cm3

OUTPUT_DIR = $(CURDIR)/Output
OBJ_DIR    = $(CURDIR)/Tmp/Obj
SRC_DIR    = $(CURDIR)

CC_ERR_FORMAT_SCRIPT = CompilerErrorFormater.py
LD_SCRIPT            = $(SRC_DIR)/Memory_Map.ld

############################################################################################
# Toolchain
############################################################################################

TOOLCHAIN = arm-none-eabi

AS        = $(TOOLCHAIN)-g++
CC        = $(TOOLCHAIN)-g++
CPP       = $(TOOLCHAIN)-g++
CPPFILT   = $(TOOLCHAIN)-c++filt
LD        = $(TOOLCHAIN)-g++
NM        = $(TOOLCHAIN)-nm
OBJDUMP   = $(TOOLCHAIN)-objdump
OBJCOPY   = $(TOOLCHAIN)-objcopy
READELF   = $(TOOLCHAIN)-readelf

PYTHON    = python3

############################################################################################
# C Compiler flags
############################################################################################

OPS_BASE     = -Wall                                          \
               -Wextra                                        \
               -Wpedantic                                     \
               -Wconversion                                   \
               -Wsign-conversion                              \
               -Wshadow                                       \
               -Wno-variadic-macros                           \
               -O2                                            \
               -mcpu=cortex-m3                                \
               -mtune=cortex-m3                               \
               -mthumb                                        \
               -mfloat-abi=soft                               \
               -mno-unaligned-access                          \
               -mno-long-calls                                \
               -fno-exceptions                                \
               -gdwarf-2

COPS         = -x c                                           \
               -fno-inline-functions                          \
               $(OPS_BASE)                                    \
               -std=c99

############################################################################################
# C++ Compiler flags
############################################################################################

CPPOPS       = -x c++                                               \
               -std=c++14                                           \
               $(OPS_BASE)                                          \
               -fno-rtti                                            \
               -fno-use-cxa-atexit                                  \
               -fno-nonansi-builtins                                \
               -fno-threadsafe-statics                              \
               -fno-enforce-eh-specs                                \
               -finline-functions                                   \
               -finline-limit=32                                    \
               -ftemplate-depth=128                                 \
               -Wzero-as-null-pointer-constant

############################################################################################
# Assembler flags
############################################################################################

ASOPS        = -x assembler                                         \
               $(OPS_BASE)

############################################################################################
# Linker flags
############################################################################################

LOPS         = -x none                                              \
               -nostartfiles                                        \
               -nostdlib                                            \
               -specs=nano.specs                                    \
               -specs=nosys.specs                                   \
               -e SysStartup_Init                                   \
               $(OPS_BASE)                                          \
               -Wl,--print-memory-usage                             \
               -Wl,-Map,$(OUTPUT_DIR)/$(PRJ_NAME).map               \
               -T $(LD_SCRIPT)


############################################################################################
# Source Files
############################################################################################
SRC_FILES := $(SRC_DIR)/Application/Appli                                                 \
             $(SRC_DIR)/Application/IntVect                                               \
             $(SRC_DIR)/Application/SysStartup                                            \
             $(SRC_DIR)/Application/MCAL/SysTickTimer                                     \
             $(SRC_DIR)/Application/pi_spigot/pi_spigot                                   \
             $(SRC_DIR)/Application/ref_app/src/mcal/mcal                                 \
             $(SRC_DIR)/Application/ref_app/src/mcal/mcal_gcc_cxx_completion              \
             $(SRC_DIR)/Application/ref_app/src/mcal/stm32f100/mcal_gpt                   \
             $(SRC_DIR)/Application/ref_app/src/mcal/stm32f100/mcal_led                   \
             $(SRC_DIR)/Application/ref_app/src/mcal/stm32f100/mcal_memory_sram           \
             $(SRC_DIR)/Application/ref_app/src/mcal/stm32f100/mcal_port                  \
             $(SRC_DIR)/Application/ref_app/src/mcal/stm32f100/mcal_spi                   \
             $(SRC_DIR)/Application/OS/OS                                                 \
             $(SRC_DIR)/Application/OS/OsAlarm                                            \
             $(SRC_DIR)/Application/OS/OsEvt                                              \
             $(SRC_DIR)/Application/OS/OsAsm                                              \
             $(SRC_DIR)/Application/OS/OsTask                                             \
             $(SRC_DIR)/Application/OS/TCB                                                \

############################################################################################
# Include Paths
############################################################################################
INC_FILES := $(SRC_DIR)/Application/OS/HwPlatform/STM32                                  \
             $(SRC_DIR)/Application/OS                                                   \
             $(SRC_DIR)/Application/MCAL                                                 \
             $(SRC_DIR)/Application                                                      \
             $(SRC_DIR)/Application/ref_app/src/mcal/stm32f100                           \
             $(SRC_DIR)/Application/ref_app/src

############################################################################################
# Rules
############################################################################################

VPATH := $(subst \,/,$(sort $(dir $(SRC_FILES)) $(OBJ_DIR)))

FILES_O := $(addprefix $(OBJ_DIR)/, $(notdir $(addsuffix .o, $(SRC_FILES))))


ifeq ($(MAKECMDGOALS),build)
-include $(subst .o,.d,$(FILES_O))
endif

all : clean $(OUTPUT_DIR)/$(PRJ_NAME).elf


.PHONY : clean
clean :
	@-rm -f $(OBJ_DIR)/*.o            2>/dev/null || true
	@-rm -f $(OBJ_DIR)/*.err          2>/dev/null || true
	@-rm -rf $(OUTPUT_DIR)            2>/dev/null || true
	@-mkdir -p $(subst \,/,$(OBJ_DIR))
	@-mkdir -p $(subst \,/,$(OUTPUT_DIR))

$(OBJ_DIR)/%.o : %.c
	@-echo +++ compile: $(subst \,/,$<) to $(subst \,/,$@)
	@-$(CC) $(COPS) $(addprefix -I, $(INC_FILES)) -c $< -o $(OBJ_DIR)/$(basename $(@F)).o 2> $(OBJ_DIR)/$(basename $(@F)).err
	@-$(PYTHON) $(CC_ERR_FORMAT_SCRIPT) $(OBJ_DIR)/$(basename $(@F)).err -COLOR

$(OBJ_DIR)/%.o : %.s
	@-echo +++ compile: $(subst \,/,$<) to $(subst \,/,$@)
	@$(AS) $(ASOPS) -c $< -o $(OBJ_DIR)/$(basename $(@F)).o 2> $(OBJ_DIR)/$(basename $(@F)).err >$(OBJ_DIR)/$(basename $(@F)).lst
	@-$(PYTHON) $(CC_ERR_FORMAT_SCRIPT) $(OBJ_DIR)/$(basename $(@F)).err -COLOR

$(OBJ_DIR)/%.o : %.cpp
	@-echo +++ compile: $(subst \,/,$<) to $(subst \,/,$@)
	@$(CPP) $(CPPOPS) $(addprefix -I, $(INC_FILES)) -c $< -o $(OBJ_DIR)/$(basename $(@F)).o 2> $(OBJ_DIR)/$(basename $(@F)).err
	@-$(PYTHON) $(CC_ERR_FORMAT_SCRIPT) $(OBJ_DIR)/$(basename $(@F)).err -COLOR

$(OUTPUT_DIR)/$(PRJ_NAME).elf : $(FILES_O) $(LD_SCRIPT)
	@$(LD) $(LOPS) $(FILES_O) -o $(OUTPUT_DIR)/$(PRJ_NAME).elf
	@$(OBJCOPY) $(OUTPUT_DIR)/$(PRJ_NAME).elf -O ihex $(OUTPUT_DIR)/$(PRJ_NAME).hex
	@$(NM) --numeric-sort --print-size $(OUTPUT_DIR)/$(PRJ_NAME).elf | $(CPPFILT) > $(OUTPUT_DIR)/$(PRJ_NAME)_cppfilt.txt
