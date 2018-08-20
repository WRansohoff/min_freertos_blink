TARGET = main

# Default target chip.
# Currently only the STM32F103C8 is supported.
# (TODO: Support some F0/L0 chips)
#MCU ?= STM32F030K6
#MCU ?= STM32F031K6
#MCU ?= STM32L031K6
MCU ?= STM32F103C8

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
else ifeq ($(MCU), STM32L031K6)
	MCU_FILES  = STM32L031K6T6
	ST_MCU_DEF = STM32L031xx
	MCU_CLASS  = L0
endif

# Define the chip architecture.
LD_SCRIPT = $(MCU_FILES).ld
ifeq ($(MCU_CLASS), F0)
	MCU_SPEC = cortex-m0
	#RTOS_PORT_C = ./freertos/Source/portable/GCC/ARM_CM0/port.c
else ifeq ($(MCU_CLASS), F1)
	MCU_SPEC = cortex-m3
	RTOS_PORT_C = ./freertos/Source/portable/GCC/ARM_CM3/port.c
else ifeq ($(MCU_CLASS), L0)
	MCU_SPEC = cortex-m0plus
	#RTOS_PORT_C = ./freertos/Source/portable/GCC/ARM_CM0/port.c
endif

# Toolchain definitions (ARM bare metal defaults)
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
CFLAGS += -msoft-float
CFLAGS += -Wall
CFLAGS += -g
CFLAGS += -Os
CFLAGS += -std=c99
CFLAGS += -fmessage-length=0 -fno-common
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += --specs=nosys.specs
CFLAGS += -D$(ST_MCU_DEF)
CFLAGS += -DVVC_$(MCU_CLASS)
CFLAGS += -DVVC_$(MCU)

# Linker directives.
LSCRIPT = ./ld/$(LD_SCRIPT)
LFLAGS += -mcpu=$(MCU_SPEC)
LFLAGS += -mthumb
LFLAGS += -msoft-float
LFLAGS += -Wall
LFLAGS += --specs=nosys.specs
LFLAGS += --static -nostartfiles
LFLAGS += -Wl,-Map=$(TARGET).map
LFLAGS += -Wl,--gc-sections
LFLAGS += -lgcc
LFLAGS += -lc
LFLAGS += -lnosys
LFLAGS += -T$(LSCRIPT)

# Source files.
AS_SRC   =  ./boot_s/$(MCU_FILES)_boot.S
AS_SRC   += ./vector_tables/$(MCU_FILES)_vt.S
C_SRC    =  ./src/main.c
C_SRC    += ./src/util.c
C_SRC    += ./freertos/Source/portable/MemMang/heap_4.c
C_SRC    += $(RTOS_PORT_C)
C_SRC    += ./freertos/Source/list.c
C_SRC    += ./freertos/Source/tasks.c
C_SRC    += ./freertos/Source/queue.c

INCLUDE  =  -I./
INCLUDE  += -I./src
INCLUDE  += -I./device_headers
INCLUDE  += -I./freertos/Source/include
INCLUDE  += -I./freertos/Source/portable/GCC/ARM_CM3

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
