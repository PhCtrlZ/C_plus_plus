#include "w25qxx.h"
#include "spi.h"

// NOTE: This driver uses manual CS (PA4) + SPI1.
// Add a mutex if SPI is shared by multiple tasks.

static inline void w25_select(void)   { flash_cs_low(); }
static inline void w25_deselect(void) { flash_cs_high(); }

void w25qxx_init(void) {
    // Ensure CS high
    w25_deselect();
}

static void w25_send_addr(uint32_t addr) {
    spi1_xfer((uint8_t)(addr >> 16));
    spi1_xfer((uint8_t)(addr >> 8));
    spi1_xfer((uint8_t)(addr >> 0));
}

uint8_t w25qxx_read_sr1(void) {
    uint8_t sr1;
    w25_select();
    spi1_xfer(W25_CMD_READ_SR1);
    sr1 = spi1_xfer(W25_DUMMY);
    w25_deselect();
    return sr1;
}

uint8_t w25qxx_read_sr2(void) {
    uint8_t sr2;
    w25_select();
    spi1_xfer(W25_CMD_READ_SR2);
    sr2 = spi1_xfer(W25_DUMMY);
    w25_deselect();
    return sr2;
}

void w25qxx_wait_ready(void) {
    while (w25qxx_read_sr1() & W25_SR1_BUSY) {
        taskYIELD();
    }
}

void w25qxx_write_enable(bool en) {
    w25qxx_wait_ready();
    w25_select();
    spi1_xfer(en ? W25_CMD_WRITE_EN : W25_CMD_WRITE_DI);
    w25_deselect();
    w25qxx_wait_ready();
}

uint16_t w25qxx_read_manuf_device_id(void) {
    uint16_t info;
    w25qxx_wait_ready();

    w25_select();
    spi1_xfer(W25_CMD_MANUF_DEVICE_ID);
    spi1_xfer(W25_DUMMY);
    spi1_xfer(W25_DUMMY);
    spi1_xfer(0x00); // address byte (0)
    info  = (uint16_t)spi1_xfer(W25_DUMMY) << 8;
    info |= (uint16_t)spi1_xfer(W25_DUMMY);
    w25_deselect();

    return info;
}

uint32_t w25qxx_read(uint32_t addr, uint8_t *out, uint32_t len) {
    w25qxx_wait_ready();

    w25_select();
    spi1_xfer(W25_CMD_FAST_READ);
    w25_send_addr(addr);
    spi1_xfer(W25_DUMMY); // dummy byte for FAST_READ

    for (uint32_t i = 0; i < len; i++) {
        out[i] = spi1_xfer(W25_DUMMY);
    }
    w25_deselect();
    return addr + len;
}

// Writes across pages (256B). Stops and re-issues command on page boundary.
uint32_t w25qxx_write(uint32_t addr, const uint8_t *data, uint32_t len) {
    while (len > 0) {
        w25qxx_write_enable(true);
        w25qxx_wait_ready();

        // How many bytes remain in current page
        uint32_t page_off = addr & 0xFFu;
        uint32_t chunk = 256u - page_off;
        if (chunk > len) chunk = len;

        w25_select();
        spi1_xfer(W25_CMD_PAGE_PROGRAM);
        w25_send_addr(addr);

        for (uint32_t i = 0; i < chunk; i++) {
            spi1_xfer(data[i]);
        }

        w25_deselect();
        w25qxx_wait_ready();

        addr += chunk;
        data += chunk;
        len  -= chunk;
    }
    return addr;
}

static uint32_t align_down(uint32_t addr, uint32_t size) {
    return addr & ~(size - 1u);
}

void w25qxx_erase_sector_4k(uint32_t addr) {
    addr = align_down(addr, 4096u);
    w25qxx_write_enable(true);
    w25qxx_wait_ready();

    w25_select();
    spi1_xfer(W25_CMD_SECTOR_ERASE_4K);
    w25_send_addr(addr);
    w25_deselect();

    w25qxx_wait_ready();
}

void w25qxx_erase_block_32k(uint32_t addr) {
    addr = align_down(addr, 32768u);
    w25qxx_write_enable(true);
    w25qxx_wait_ready();

    w25_select();
    spi1_xfer(W25_CMD_BLOCK_ERASE_32K);
    w25_send_addr(addr);
    w25_deselect();

    w25qxx_wait_ready();
}

void w25qxx_erase_block_64k(uint32_t addr) {
    addr = align_down(addr, 65536u);
    w25qxx_write_enable(true);
    w25qxx_wait_ready();

    w25_select();
    spi1_xfer(W25_CMD_BLOCK_ERASE_64K);
    w25_send_addr(addr);
    w25_deselect();

    w25qxx_wait_ready();
}

void w25qxx_chip_erase(void) {
    w25qxx_write_enable(true);
    w25qxx_wait_ready();

    w25_select();
    spi1_xfer(W25_CMD_CHIP_ERASE);
    w25_deselect();

    // Chip erase can take a long time; keep yielding.
    w25qxx_wait_ready();
}
