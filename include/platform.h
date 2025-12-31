#pragma once

// Common includes for libopencm3 + FreeRTOS project.
// Adjust as needed for your exact project layout.

#include <stdint.h>
#include <stdbool.h>

// libopencm3
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>

// FreeRTOS (add correct include paths in Makefile)
#include "FreeRTOS.h"
#include "task.h"

// If you have a console (USB CDC / UART), include it here and map the functions:
// - int console_getchar(void);
// - void console_puts(const char*);
// - int console_printf(const char*, ...);
