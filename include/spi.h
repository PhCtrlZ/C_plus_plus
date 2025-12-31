#pragma once
#include "platform.h"

// SPI instance
#define FLASH_SPI SPI1

// Manual CS on PA4 (safer than relying on HW NSS in many setups)
#define FLASH_CS_PORT GPIOA
#define FLASH_CS_PIN  GPIO4

void spi1_setup_for_flash(void);
uint8_t spi1_xfer(uint8_t data);

// Manual CS helpers
static inline void flash_cs_low(void)  { gpio_clear(FLASH_CS_PORT, FLASH_CS_PIN); }
static inline void flash_cs_high(void) { gpio_set(FLASH_CS_PORT, FLASH_CS_PIN); }
