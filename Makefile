#
#  Copyright Christopher Kormanyos 2023 - 2024.
#  Distributed under the Boost Software License,
#  Version 1.0. (See accompanying file LICENSE_1_0.txt
#  or copy at http://www.boost.org/LICENSE_1_0.txt)
#

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
# operating system
############################################################################################

ifeq ($(TYP_OS),)

ifeq ($(OS),Windows_NT)
TYP_OS       := WIN
else
TYP_OS       := NIX
endif

endif

############################################################################################
# Defines
############################################################################################

ifeq ($(PI_SPIGOT_DIGITS),)
PI_SPIGOT_DIGITS  := 10000
endif

PRJ_NAME   = Osek_pi_crunch_cm3

OUTPUT_DIR = $(CURDIR)/Output
OBJ_DIR    = $(CURDIR)/Tmp/Obj
SRC_DIR    = $(CURDIR)

CC_ERR_FORMAT_SCRIPT = CompilerErrorFormater.py
LD_SCRIPT            = $(SRC_DIR)/Memory_Map.ld

############################################################################################
# Toolchain
############################################################################################

ifeq ($(TYP_OS),WIN)
EXEEXT        := .exe
endif

ifeq ($(TYP_OS),NIX)
EXEEXT        :=
endif

TOOLCHAIN      = arm-none-eabi

AS            := $(TOOLCHAIN)-g++
CC            := $(TOOLCHAIN)-g++
CPP           := $(TOOLCHAIN)-g++
CPPFILT       := $(TOOLCHAIN)-c++filt
LD            := $(TOOLCHAIN)-g++
NM            := $(TOOLCHAIN)-nm
OBJDUMP       := $(TOOLCHAIN)-objdump
OBJCOPY       := $(TOOLCHAIN)-objcopy
READELF       := $(TOOLCHAIN)-readelf
GNUECHO       := echo
MKDIR         := mkdir
RM            := rm
SED           := sed

ifeq ($(TYP_OS),WIN)

EXEEXT        := .exe

PATH_TOOLS    := $(CURDIR)/build/tools

PATH_CC       := $(PATH_TOOLS)/Util/msys64/usr/local/gcc-13.3.1-arm-none-eabi/bin
PATH_UTILS    := $(PATH_TOOLS)/UnxUtils/usr/local/wbin

AS            := $(subst /,\,$(PATH_CC)/$(AS)$(EXEEXT))
CC            := $(subst /,\,$(PATH_CC)/$(CC)$(EXEEXT))
CPP           := $(subst /,\,$(PATH_CC)/$(CPP)$(EXEEXT))
CPPFILT       := $(subst /,\,$(PATH_CC)/$(CPPFILT)$(EXEEXT))
LD            := $(subst /,\,$(PATH_CC)/$(LD)$(EXEEXT))
NM            := $(subst /,\,$(PATH_CC)/$(NM)$(EXEEXT))
OBJDUMP       := $(subst /,\,$(PATH_CC)/$(OBJDUMP)$(EXEEXT))
OBJCOPY       := $(subst /,\,$(PATH_CC)/$(OBJCOPY)$(EXEEXT))
READELF       := $(subst /,\,$(PATH_CC)/$(READELF)$(EXEEXT))
GNUECHO       := $(subst /,\,$(PATH_UTILS)/$(GNUECHO)$(EXEEXT))
MKDIR         := $(subst /,\,$(PATH_UTILS)/$(MKDIR)$(EXEEXT))
RM            := $(subst /,\,$(PATH_UTILS)/$(RM)$(EXEEXT))
SED           := $(subst /,\,$(PATH_UTILS)/$(SED)$(EXEEXT))

endif

############################################################################################
# OS-independent abstratcion of the null-device
############################################################################################

ifeq ($(TYP_OS),WIN)
NULL_DEVICE     := NUL
$(NULL_DEVICE)  := NUL
endif

ifeq ($(TYP_OS),NIX)
NULL_DEVICE     := /dev/null
$(NULL_DEVICE)  := /dev/null
endif

############################################################################################
# C Compiler flags
############################################################################################

OPS_BASE     = -Wall                                                          \
               -Wextra                                                        \
               -Wpedantic                                                     \
               -Wconversion                                                   \
               -Wsign-conversion                                              \
               -Wshadow                                                       \
               -Wno-variadic-macros                                           \
               -Os                                                            \
               -mcpu=cortex-m3                                                \
               -mtune=cortex-m3                                               \
               -mthumb                                                        \
               -mfloat-abi=soft                                               \
               -mno-unaligned-access                                          \
               -mno-long-calls                                                \
               -fno-exceptions                                                \
               -gdwarf-2                                                      \
               -DPI_CRUNCH_METAL_DISABLE_IOSTREAM                             \
               -DPI_CRUNCH_METAL_PI_SPIGOT_DIGITS=$(PI_SPIGOT_DIGITS)


COPS         = -x c                                                 \
               -fno-inline-functions                                \
               $(OPS_BASE)                                          \
               -std=c17

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


ifeq ($(TYP_OS),WIN)
LOPS        += -Wl,--no-warn-rwx-segments
endif


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
C_INCLUDES := $(SRC_DIR)/Application/OS/HwPlatform/STM32                                  \
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
	@-$(GNUECHO)
	@-$(GNUECHO) +++ cleaning all
	@-$(MKDIR) -p $(OUTPUT_DIR)
	@-$(MKDIR) -p $(OBJ_DIR)
	@-$(RM) -r $(OUTPUT_DIR) 2>$(NULL_DEVICE)
	@-$(RM) -r $(OBJ_DIR) 2>$(NULL_DEVICE)
	@-$(MKDIR) -p $(OUTPUT_DIR)
	@-$(MKDIR) -p $(OBJ_DIR)


$(OBJ_DIR)/%.o : %.c
	@-$(GNUECHO) +++ compile: $(subst \,/,$<) to $(subst \,/,$@)
	@-$(CC) $(COPS) $(addprefix -I, $(C_INCLUDES)) -c $< -o $(OBJ_DIR)/$(basename $(@F)).o 2> $(OBJ_DIR)/$(basename $(@F)).err
	@-$(SED) -e 's|.h:\([0-9]*\),|.h(\1) :|' -e 's|.c:\([0-9]*\),|.c(\1) :|' $(OBJ_DIR)/$(basename $(@F)).err

$(OBJ_DIR)/%.o : %.s
	@-$(GNUECHO) +++ compile: $(subst \,/,$<) to $(subst \,/,$@)
	@$(AS) $(ASOPS) -c $< -o $(OBJ_DIR)/$(basename $(@F)).o 2> $(OBJ_DIR)/$(basename $(@F)).err >$(OBJ_DIR)/$(basename $(@F)).lst
	@-$(SED) -e 's|:\([0-9]*\):|(\1) :|' $(OBJ_DIR)/$(basename $(@F)).err

$(OBJ_DIR)/%.o : %.cpp
	@-$(GNUECHO) +++ compile: $(subst \,/,$<) to $(subst \,/,$@)
	@$(CPP) $(CPPOPS) $(addprefix -I, $(C_INCLUDES)) -c $< -o $(OBJ_DIR)/$(basename $(@F)).o 2> $(OBJ_DIR)/$(basename $(@F)).err
	@-$(SED) -e 's|.h:\([0-9]*\),|.h(\1) :|' -e 's|.hpp:\([0-9]*\),|.hpp(\1) :|' -e 's|.cpp:\([0-9]*\),|.cpp(\1) :|' $(OBJ_DIR)/$(basename $(@F)).err

$(OUTPUT_DIR)/$(PRJ_NAME).elf : $(FILES_O) $(LD_SCRIPT)
	@$(LD) $(LOPS) $(FILES_O) -o $(OUTPUT_DIR)/$(PRJ_NAME).elf
	@$(OBJCOPY) $(OUTPUT_DIR)/$(PRJ_NAME).elf -O ihex $(OUTPUT_DIR)/$(PRJ_NAME).hex
	@$(NM) --numeric-sort --print-size $(OUTPUT_DIR)/$(PRJ_NAME).elf | $(CPPFILT) > $(OUTPUT_DIR)/$(PRJ_NAME)_cppfilt.txt
