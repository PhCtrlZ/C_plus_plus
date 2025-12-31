# W25Qxx + SPI1 + FreeRTOS (STM32F103C8T6)


- Cấu hình SPI1 trên STM32F103 (PA4 NSS, PA5 SCK, PA6 MISO, PA7 MOSI)
- Driver cơ bản cho W25Qxx (đọc status, chờ bận, đọc ID, đọc/ghi, xóa)
- Task FreeRTOS `monitor_task` dạng “khung” (menu terminal)
- Mẫu Makefile tối giản (bạn cần tự nối/cấu hình toolchain + libopencm3 + FreeRTOS)

## Các file

- `src/main.c` – cấu hình clock, GPIO, khởi tạo SPI, tạo task
- `src/spi.c`, `include/spi.h`
- `src/w25qxx.c`, `include/w25qxx.h`
- `src/monitor_task.c`, `include/monitor_task.h`
- `Makefile` – file mẫu
- `include/platform.h` – các include chung / tuỳ chọn compile

## Ghi chú / TODO

- Phần I/O terminal (`std_getchar`, `printf`...) đang để ở dạng **stub (khung giả)**. Bạn cần nối vào console USB CDC hoặc UART của bạn.
- Chân Chip Select (NSS): bạn có thể dùng **NSS phần cứng** HOẶC **CS GPIO tự điều khiển**. Bộ khung này giả định dùng *CS tự điều khiển* qua GPIOA4 cho an toàn.
- Khi dùng thực tế, nên bổ sung:
  - Kiểm tra W25Q “Write Enable Latch” (bit WEL) và xử lý lỗi chuẩn
  - Ghi theo biên trang (page-aligned) và xóa theo biên sector (sector-aligned) bằng cách mask địa chỉ
  - Dùng mutex bảo vệ SPI nếu có nhiều task cùng dùng SPI

## Build

Sửa các đường dẫn trong `Makefile`:

- `LIBOPENCM3_DIR`
- `FREERTOS_DIR`
- `PREFIX` (arm-none-eabi-)

Sau đó chạy:

- `make`
- `make flash` (st-flash)
