#pragma once
#include "platform.h"

#ifdef __cplusplus
extern "C" {
#endif

// Common W25Qxx commands
#define W25_CMD_WRITE_EN         0x06
#define W25_CMD_WRITE_DI         0x04
#define W25_CMD_READ_SR1         0x05
#define W25_CMD_READ_SR2         0x35
#define W25_CMD_FAST_READ        0x0B
#define W25_CMD_PAGE_PROGRAM     0x02
#define W25_CMD_SECTOR_ERASE_4K  0x20
#define W25_CMD_BLOCK_ERASE_32K  0x52
#define W25_CMD_BLOCK_ERASE_64K  0xD8
#define W25_CMD_CHIP_ERASE       0xC7
#define W25_CMD_MANUF_DEVICE_ID  0x90

#define W25_DUMMY                0x00

// SR1 bits
#define W25_SR1_BUSY             (1u << 0)
#define W25_SR1_WEL              (1u << 1)

void     w25qxx_init(void);

uint8_t  w25qxx_read_sr1(void);
uint8_t  w25qxx_read_sr2(void);
void     w25qxx_wait_ready(void);

void     w25qxx_write_enable(bool en);
uint16_t w25qxx_read_manuf_device_id(void);

uint32_t w25qxx_read(uint32_t addr, uint8_t *out, uint32_t len);
uint32_t w25qxx_write(uint32_t addr, const uint8_t *data, uint32_t len);

void     w25qxx_erase_sector_4k(uint32_t addr);
void     w25qxx_erase_block_32k(uint32_t addr);
void     w25qxx_erase_block_64k(uint32_t addr);
void     w25qxx_chip_erase(void);

#ifdef __cplusplus
}
#endif
