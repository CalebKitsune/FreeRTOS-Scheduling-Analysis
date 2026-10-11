// Caleb Dufour
// RTEN TEST
// CREATED: OCT 3, 2026

// DOCS USED:
// https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/system/freertos_idf.html
// https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/gpio.html
// create unpinned tasks
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#define FOR_LOOP_WAIT 2 << 22

// help struct
typedef struct
{
    uint32_t ms;
    gpio_num_t pin;
} task_args_t;

// GLOBAL
static const gpio_num_t LED_100 = GPIO_NUM_2;
static const gpio_num_t LED_500 = GPIO_NUM_3;
static const gpio_num_t LED_700 = GPIO_NUM_4;

static const task_args_t args_100 = {
    .ms = 100,
    .pin = GPIO_NUM_2};

static const task_args_t args_500 = {
    .ms = 500,
    .pin = GPIO_NUM_3};

static const task_args_t args_700 = {
    .ms = 700,
    .pin = GPIO_NUM_4};

// functions
void set_config_output(gpio_num_t pin);
void task_xxxMS(void *pvParameters);

void app_main(void)
{
    BaseType_t result;
    int PRIORITY[] = {3, 2, 1}; // higher time = higher priority
    // INIT GPIO
    set_config_output(LED_100);
    set_config_output(LED_500);
    set_config_output(LED_700);

    result = xTaskCreate(
        task_xxxMS,
        "100MS",
        2048,
        (void *)&args_100,
        PRIORITY[0],
        NULL);
    if (result != pdPASS)
    {
        ESP_LOGE("TASK", "Failed to create 100ms task!");
    }
    result = xTaskCreate(
        task_xxxMS,
        "500MS",
        2048,
        (void *)&args_500,
        PRIORITY[1],
        NULL);
    if (result != pdPASS)
    {
        ESP_LOGE("TASK", "Failed to create 100ms task!");
    }
    result = xTaskCreate(
        task_xxxMS,
        "700MS",
        2048,
        (void *)&args_700,
        PRIORITY[2],
        NULL);
    if (result != pdPASS)
    {
        ESP_LOGE("TASK", "Failed to create 100ms task!");
    }
    // design
    //  ISR ->
    //  QUEUE
    //  TASkS
}

void set_config_output(gpio_num_t pin)
{
    gpio_config_t io_config = {
        .pin_bit_mask = (1ULL << pin),
        .mode = GPIO_MODE_OUTPUT,
    };
    gpio_config(&io_config);
}

void task_xxxMS(void *pvParameters)
{
    const task_args_t *args = (const task_args_t *)pvParameters;

    uint32_t ms = args->ms;
    gpio_num_t pin = args->pin;

    bool pin_state = false;
    for (;;)
    {
        for (int i = 0; i < FOR_LOOP_WAIT; i++)
        {
        }
        // no atomic toggle
        // read, then BSR/BSRR

        pin_state = !pin_state;
        gpio_set_level(pin, pin_state);

        // ESP_LOGI("TASK", "GPIO %d -> %d, delay = %lu ms",
        //          pin, pin_state, ms);

        // toggle pin state
        vTaskDelay(pdMS_TO_TICKS(ms)); // wait ms
    }
}
