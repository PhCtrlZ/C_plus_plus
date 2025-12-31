# Makefile (template) – STM32F103C8T6 + libopencm3 + FreeRTOS
# You MUST edit paths below.

PREFIX      ?= arm-none-eabi-
CC          := $(PREFIX)gcc
OBJCOPY     := $(PREFIX)objcopy
SIZE        := $(PREFIX)size

LIBOPENCM3_DIR ?= ../libopencm3
FREERTOS_DIR   ?= ../FreeRTOS-Kernel

PROJECT = w25q_skeleton
BUILD   = build

DEVICE  = stm32f1
FAMILY  = STM32F1
OPENCM3_LIB = opencm3_stm32f1

CFLAGS  += -Os -g3 -Wall -Wextra -Werror
CFLAGS  += -mcpu=cortex-m3 -mthumb
CFLAGS  += -ffunction-sections -fdata-sections
CFLAGS  += -Iinclude
CFLAGS  += -I$(LIBOPENCM3_DIR)/include
# Add your FreeRTOS include paths as needed:
# CFLAGS += -I$(FREERTOS_DIR)/include -I$(FREERTOS_DIR)/portable/GCC/ARM_CM3

LDFLAGS += -nostartfiles
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -T$(LIBOPENCM3_DIR)/lib/stm32/f1/stm32f103c8t6.ld
LDFLAGS += -L$(LIBOPENCM3_DIR)/lib
LDFLAGS += -l$(OPENCM3_LIB)

SRCS = \
  src/main.c \
  src/spi.c \
  src/w25qxx.c \
  src/monitor_task.c

OBJS = $(SRCS:%.c=$(BUILD)/%.o)

all: $(BUILD)/$(PROJECT).bin

$(BUILD)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/$(PROJECT).elf: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@
	$(SIZE) $@

$(BUILD)/$(PROJECT).bin: $(BUILD)/$(PROJECT).elf
	$(OBJCOPY) -O binary $< $@

flash: $(BUILD)/$(PROJECT).bin
	st-flash write $(BUILD)/$(PROJECT).bin 0x08000000

clean:
	rm -rf $(BUILD)

clobber: clean

.PHONY: all flash clean clobber
