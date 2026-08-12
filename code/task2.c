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
static void Seg7_GPIO_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
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
}
static void Seg7_Show(uint8_t number)
{
    if (number > 9)
    {
        return;
    }
    uint8_t data = number_table[number];

    for (uint8_t i = 0; i < 7; i++)
    {
        uint16_t pin = (uint16_t)(GPIO_PIN_0 << i);

        if (data & (1U << i))
        {
            HAL_GPIO_WritePin(GPIOA, pin, GPIO_PIN_RESET);
        }
        else
        {
            HAL_GPIO_WritePin(GPIOA, pin, GPIO_PIN_SET);
        }
    }

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
}
int main(void)
{
    HAL_Init();
    Seg7_GPIO_Init();
    uint8_t number = 0;
    while (1)
    {
        Seg7_Show(number);
        HAL_Delay(1000);
        number++;
        if (number > 9)
        {
            number = 0;
        }
    }
}
