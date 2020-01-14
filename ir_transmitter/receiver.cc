#include "driver/gpio.h"
#include "driver/rmt.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/ringbuf.h"

void app_main() {
    // Setup RMT
    rmt_config_t rmt_rx;
    rmt_rx.channel = RMT_CHANNEL_0;
    rmt_rx.rmt_mode = RMT_MODE_RX;
    rmt_rx.gpio_num = 19;
    rmt_rx.mem_block_num = 1;
    rmt_rx.clk_div = 80;
    rmt_rx.rx_config.filter_en = 0;
    rmt_rx.rx_config.idle_threshold = 10000;
    rmt_config(&rmt_rx);
    rmt_driver_install(rmt_rx.channel, 1000, 0);
    ESP_LOGI("IR", "RMT driver installed");

    // Get RMT buffer handle
    RingbufHandle_t rb = NULL;
    rmt_get_ringbuf_handle(rmt_rx.channel, &rb);

    rmt_rx_start(rmt_rx.channel, 1);
    ESP_LOGI("IR", "RMT started");

    size_t rx_size = 0;
    uint32_t duration, level;
    while (1) {
        rmt_item32_t *item = (rmt_item32_t *)xRingbufferReceive(rb, &rx_size, 1000);
        ESP_LOGI("IR", "%d", (uintptr_t)item);
	ESP_LOGI("IR", "%X", (uintptr_t)item);
	
        if (item) {
           vRingbufferReturnItem(rb, (void *)item);
        }
    }
}
