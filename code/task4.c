#include "stm32f1xx_hal.h"
#include <stdint.h>

static const uint8_t number_table[10] =
{
    0x3F,
    0x06,
    0x5B,
    0x4F,
    0x66,
    0x6D,
    0x7D,
    0x07,
    0x7F,
    0x6F
};
static void gpio_init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef gpio = {0};
    gpio.Pin = GPIO_PIN_0 |
               GPIO_PIN_1 |
               GPIO_PIN_2 |
               GPIO_PIN_3 |
               GPIO_PIN_4 |
               GPIO_PIN_5 |
               GPIO_PIN_6 |
               GPIO_PIN_7;

    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &gpio);
    HAL_GPIO_WritePin(
        GPIOA,
        GPIO_PIN_0 |
        GPIO_PIN_1 |
        GPIO_PIN_2 |
        GPIO_PIN_3 |
        GPIO_PIN_4 |
        GPIO_PIN_5 |
        GPIO_PIN_6 |
        GPIO_PIN_7,
        GPIO_PIN_SET
    );
    gpio.Pin = GPIO_PIN_12 | GPIO_PIN_13;
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &gpio);
}
static void show_number(uint8_t number)
{
    uint8_t data = number_table[number];
    for (uint8_t i = 0; i < 7; i++)
    {
        if (data & (1 << i))
        {
            HAL_GPIO_WritePin(GPIOA, (uint16_t)(1 << i), GPIO_PIN_RESET);
        }
        else
        {
            HAL_GPIO_WritePin(GPIOA, (uint16_t)(1 << i), GPIO_PIN_SET);
        }
    }
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
}
int main(void)
{
    HAL_Init();
    gpio_init();
    while (1)
    {
        uint8_t sensor1 = 0;
        uint8_t sensor2 = 0;
        if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == GPIO_PIN_SET)
        {
            sensor1 = 1;
        }
        if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13) == GPIO_PIN_SET)
        {
            sensor2 = 1;
        }
        uint8_t state = sensor1 + sensor2 * 2;
        show_number(state);
        HAL_Delay(20);
    }
}
