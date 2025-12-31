#include "spi.h"

void spi1_setup_for_flash(void) {
    // Clocks
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_SPI1);

    // SPI pins: PA5 SCK, PA6 MISO, PA7 MOSI
    // SCK + MOSI as AF push-pull, MISO as input floating
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
                  GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
                  GPIO5 | GPIO7);

    gpio_set_mode(GPIOA, GPIO_MODE_INPUT,
                  GPIO_CNF_INPUT_FLOAT, GPIO6);

    // Manual CS: PA4 output push-pull, idle high
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
                  GPIO_CNF_OUTPUT_PUSHPULL, FLASH_CS_PIN);
    flash_cs_high();

    // Reset SPI
    rcc_periph_reset_pulse(RST_SPI1);

    // Configure SPI as master, 8-bit, mode 0, MSB first
    spi_init_master(FLASH_SPI,
                    SPI_CR1_BAUDRATE_FPCLK_DIV_256,
                    SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE,
                    SPI_CR1_CPHA_CLK_TRANSITION_1,
                    SPI_CR1_DFF_8BIT,
                    SPI_CR1_MSBFIRST);

    // Software slave management ON (since we use manual CS)
    spi_enable_software_slave_management(FLASH_SPI);
    spi_set_nss_high(FLASH_SPI);

    spi_enable(FLASH_SPI);
}

uint8_t spi1_xfer(uint8_t data) {
    spi_send(FLASH_SPI, data);
    return (uint8_t)spi_read(FLASH_SPI);
}
