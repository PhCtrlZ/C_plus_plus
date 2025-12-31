#include "platform.h"
#include "spi.h"
#include "w25qxx.h"
#include "monitor_task.h"

static void clock_setup(void) {
    // 72MHz from 8MHz HSE (Blue Pill typical)
    rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);
}

static void gpio_setup(void) {
    rcc_periph_clock_enable(RCC_GPIOC);
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ,
                  GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
    gpio_set(GPIOC, GPIO13); // LED off (BluePill PC13 is inverted)
}

int main(void) {
    clock_setup();
    gpio_setup();

    spi1_setup_for_flash();
    w25qxx_init();

    // Create monitor task
    xTaskCreate(monitor_task, "monitor", 512, NULL, tskIDLE_PRIORITY + 2, NULL);

    vTaskStartScheduler();

    // Should never reach here
    for (;;) {
        __asm__("nop");
    }
}
