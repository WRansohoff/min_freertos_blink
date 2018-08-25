TARGET = main

# Default target chip.
# Currently only the STM32F103C8 is supported.
# (TODO: Support some F0/L0 chips)
#MCU ?= STM32F030K6
#MCU ?= STM32F031K6
MCU ?= STM32F103C8
#MCU ?= STM32F303K8
#MCU ?= STM32L031K6

ifeq ($(MCU), STM32F030K6)
	MCU_FILES  = STM32F030K6T6
	ST_MCU_DEF = STM32F030x6
	MCU_CLASS  = F0
else ifeq ($(MCU), STM32F031K6)
	MCU_FILES  = STM32F031K6T6
	ST_MCU_DEF = STM32F031x6
	MCU_CLASS  = F0
else ifeq ($(MCU), STM32F103C8)
	MCU_FILES  = STM32F103C8T6
	ST_MCU_DEF = STM32F103xB
	MCU_CLASS  = F1
else ifeq ($(MCU), STM32F303K8)
	MCU_FILES  = STM32F303K8T6
	ST_MCU_DEF = STM32F303x8
	MCU_CLASS  = F3
else ifeq ($(MCU), STM32L031K6)
	MCU_FILES  = STM32L031K6T6
	ST_MCU_DEF = STM32L031xx
	MCU_CLASS  = L0
endif

# Define the chip architecture.
LD_SCRIPT = $(MCU_FILES).ld
ifeq ($(MCU_CLASS), F0)
	MCU_SPEC = cortex-m0
	FREERTOS_PORT_I = ./freertos/Source/portable/GCC/ARM_CM0
else ifeq ($(MCU_CLASS), F1)
	MCU_SPEC = cortex-m3
	FREERTOS_PORT_I = ./freertos/Source/portable/GCC/ARM_CM3
else ifeq ($(MCU_CLASS), F3)
	MCU_SPEC = cortex-m4
	FREERTOS_PORT_I = ./freertos/Source/portable/GCC/ARM_CM4F
else ifeq ($(MCU_CLASS), L0)
	MCU_SPEC = cortex-m0plus
	FREERTOS_PORT_I = ./freertos/Source/portable/GCC/ARM_CM0
endif
FREERTOS_PORT_C = $(FREERTOS_PORT_I)/port.c

# Toolchain definitions (ARM bare metal defaults)
# Note: To compile for Cortex-M0 and -M0+ chips,
# you'll need a newer version of arm-none-eabi-gcc
# than the 4.9 which is the default in many repos.
TOOLCHAIN = /usr/bin
CC = $(TOOLCHAIN)/arm-none-eabi-gcc
AS = $(TOOLCHAIN)/arm-none-eabi-as
LD = $(TOOLCHAIN)/arm-none-eabi-ld
OC = $(TOOLCHAIN)/arm-none-eabi-objcopy
OD = $(TOOLCHAIN)/arm-none-eabi-objdump
OS = $(TOOLCHAIN)/arm-none-eabi-size

# Assembly directives.
ASFLAGS += -mcpu=$(MCU_SPEC)
ASFLAGS += -mthumb

# C compilation directives
CFLAGS += -mcpu=$(MCU_SPEC)
CFLAGS += -mthumb
ifeq ($(MCU_CLASS), F3)
	CFLAGS += -mhard-float
	CFLAGS += -mfloat-abi=hard
	CFLAGS += -mfpu=fpv4-sp-d16
else
	CFLAGS += -msoft-float
	CFLAGS += -mfloat-abi=soft
endif
CFLAGS += -Wall
CFLAGS += -g
CFLAGS += -Os
CFLAGS += -std=c99
CFLAGS += -fmessage-length=0 -fno-common
CFLAGS += -ffunction-sections -fdata-sections
#CFLAGS += --specs=nosys.specs
CFLAGS += -D$(ST_MCU_DEF)
CFLAGS += -DVVC_$(MCU_CLASS)
CFLAGS += -DVVC_$(MCU)

# Linker directives.
LSCRIPT = ./ld/$(LD_SCRIPT)
LFLAGS += -mcpu=$(MCU_SPEC)
LFLAGS += -mthumb
ifeq ($(MCU_CLASS), F3)
	LFLAGS += -mhard-float
	LFLAGS += -mfloat-abi=hard
	LFLAGS += -mfpu=fpv4-sp-d16
else
	LFLAGS += -msoft-float
	LFLAGS += -mfloat-abi=soft
endif
LFLAGS += -Wall
#LFLAGS += --specs=nosys.specs
LFLAGS += --static
LFLAGS += -Wl,-Map=$(TARGET).map
LFLAGS += -Wl,--gc-sections
LFLAGS += -lgcc
LFLAGS += -lc
#LFLAGS += -lnosys
LFLAGS += -T$(LSCRIPT)

# Source files.
AS_SRC   =  ./boot_s/$(MCU_FILES)_boot.S
AS_SRC   += ./vector_tables/$(MCU_FILES)_vt.S
C_SRC    =  ./src/main.c
C_SRC    += ./src/util.c
C_SRC    += ./src/global.c
C_SRC    += ./freertos/Source/portable/MemMang/heap_4.c
C_SRC    += $(FREERTOS_PORT_C)
C_SRC    += ./freertos/Source/list.c
C_SRC    += ./freertos/Source/tasks.c
C_SRC    += ./freertos/Source/queue.c

INCLUDE  += -I./
INCLUDE  += -I./src
INCLUDE  += -I./device_headers
INCLUDE  += -I./freertos/Source/include
INCLUDE  += -I$(FREERTOS_PORT_I)

OBJS  = $(C_SRC:.c=.o)
OBJS += $(AS_SRC:.S=.o)

.PHONY: all
all: $(TARGET).bin

%.o: %.S
	$(AS) $(ASFLAGS) -c $< -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $(INCLUDE) $< -o $@

$(TARGET).elf: $(OBJS)
	$(CC) $^ $(LFLAGS) -o $@

$(TARGET).bin: $(TARGET).elf
	$(OC) -S -O binary $< $@
	$(OS) $<

.PHONY: clean
clean:
	rm -f $(OBJS)
	rm -f $(TARGET).elf
	rm -f $(TARGET).bin
	rm -f $(TARGET).map
