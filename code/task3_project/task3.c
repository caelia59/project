#include "stm32f1xx_hal.h"
#include <stdint.h>
static void gpio_init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_AFIO_CLK_ENABLE();

    __HAL_AFIO_REMAP_SWJ_NOJTAG();
    GPIO_InitTypeDef gpio = {0};
    gpio.Pin = GPIO_PIN_6 |
               GPIO_PIN_7 |
               GPIO_PIN_8 |
               GPIO_PIN_9;

    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &gpio);
    gpio.Pin = GPIO_PIN_15;
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &gpio);
}
static void motor_forward(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
}
static void motor_reverse(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
}
static uint8_t button_pressed(void)
{
    return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15) == GPIO_PIN_RESET;
}
int main(void)
{
    HAL_Init();
    gpio_init();
    uint8_t reverse = 0;
    motor_forward();
    while (1)
    {
        if (button_pressed())
        {
            HAL_Delay(20);

            if (button_pressed())
            {
                reverse = !reverse;

                if (reverse)
                {
                    motor_reverse();
                }
                else
                {
                    motor_forward();
                }

                while (button_pressed())
                {
                    HAL_Delay(10);
                }
            }
        }
    }
}
