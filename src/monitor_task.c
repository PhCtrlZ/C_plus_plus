#include "monitor_task.h"
#include "w25qxx.h"

// TODO: Wire these to your actual console (USB CDC / UART).
// For now, they are weak stubs to let the project compile if you implement them elsewhere.
__attribute__((weak)) int console_getchar(void) { return -1; }
__attribute__((weak)) void console_puts(const char *s) { (void)s; }
__attribute__((weak)) int console_printf(const char *fmt, ...) { (void)fmt; return 0; }

static void print_menu(void) {
    console_puts(
        "\r\n=== W25Qxx Monitor ===\r\n"
        "s: read status (SR1/SR2)\r\n"
        "i: read Manufacturer/Device ID\r\n"
        "d: dump 256 bytes from address 0x000000\r\n"
        "p: program demo pattern to address 0x000000\r\n"
        "e: erase 4K sector at address 0x000000\r\n"
        "m: show menu\r\n"
    );
}

void monitor_task(void *arg) {
    (void)arg;
    print_menu();

    static uint8_t buf[256];

    for (;;) {
        int c = console_getchar();
        if (c < 0) {
            vTaskDelay(pdMS_TO_TICKS(20));
            continue;
        }

        switch ((char)c) {
        case 'm':
            print_menu();
            break;

        case 's': {
            uint8_t sr1 = w25qxx_read_sr1();
            uint8_t sr2 = w25qxx_read_sr2();
            console_printf("SR1=0x%02X  SR2=0x%02X\r\n", sr1, sr2);
        } break;

        case 'i': {
            uint16_t id = w25qxx_read_manuf_device_id();
            console_printf("Manuf/Device ID: 0x%04X\r\n", id);
        } break;

        case 'd': {
            w25qxx_read(0x000000, buf, sizeof(buf));
            console_puts("Dump 256 bytes @0x000000:\r\n");
            for (int i = 0; i < 256; i++) {
                if ((i % 16) == 0) console_printf("\r\n%06X: ", i);
                console_printf("%02X ", buf[i]);
            }
            console_puts("\r\n");
        } break;

        case 'p': {
            for (int i = 0; i < 256; i++) buf[i] = (uint8_t)i;
            w25qxx_write(0x000000, buf, sizeof(buf));
            console_puts("Programmed pattern 0..255 at 0x000000\r\n");
        } break;

        case 'e': {
            w25qxx_erase_sector_4k(0x000000);
            console_puts("Erased 4K sector at 0x000000\r\n");
        } break;

        default:
            console_puts("Unknown key. Press 'm' for menu.\r\n");
            break;
        }
    }
}
