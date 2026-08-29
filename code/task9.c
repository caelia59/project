#include "stm32f1xx_hal.h"
#include <stdint.h>

#define BLACK_LEVEL GPIO_PIN_SET

#define PWM_PERIOD 999

static TIM_HandleTypeDef htim4;

static float Kp = 24.0f;
static float Ki = 0.10f;
static float Kd = 12.0f;

static int16_t BaseSpeed = 55;

static int16_t Clamp(int16_t value, int16_t min, int16_t max)
{
    if (value < min)
        return min;

    if (value > max)
        return max;

    return value;
}

static void GPIO_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_AFIO_CLK_ENABLE();

    __HAL_AFIO_REMAP_SWJ_NOJTAG();

    GPIO_InitTypeDef gpio = {0};

    gpio.Pin = GPIO_PIN_6 | GPIO_PIN_7 |
               GPIO_PIN_8 | GPIO_PIN_9;
    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &gpio);

    gpio.Pin = GPIO_PIN_12 | GPIO_PIN_13;
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &gpio);

    gpio.Pin = GPIO_PIN_15;
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &gpio);
}

static void PWM_Init(void)
{
    __HAL_RCC_TIM4_CLK_ENABLE();

    htim4.Instance = TIM4;
    htim4.Init.Prescaler = 7;
    htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim4.Init.Period = PWM_PERIOD;
    htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    HAL_TIM_PWM_Init(&htim4);

    TIM_OC_InitTypeDef config = {0};

    config.OCMode = TIM_OCMODE_PWM1;
    config.Pulse = 0;
    config.OCPolarity = TIM_OCPOLARITY_HIGH;
    config.OCFastMode = TIM_OCFAST_DISABLE;

    HAL_TIM_PWM_ConfigChannel(&htim4, &config, TIM_CHANNEL_1);
    HAL_TIM_PWM_ConfigChannel(&htim4, &config, TIM_CHANNEL_2);
    HAL_TIM_PWM_ConfigChannel(&htim4, &config, TIM_CHANNEL_3);
    HAL_TIM_PWM_ConfigChannel(&htim4, &config, TIM_CHANNEL_4);

    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
}

static void SetPWM(uint32_t channel, uint16_t speed)
{
    if (speed > 100)
        speed = 100;

    uint32_t value = ((PWM_PERIOD + 1) * speed) / 100;

    if (value > PWM_PERIOD)
        value = PWM_PERIOD;

    __HAL_TIM_SET_COMPARE(&htim4, channel, value);
}

static void Motor_Set(int16_t left, int16_t right)
{
    left = Clamp(left, 0, 100);
    right = Clamp(right, 0, 100);

    SetPWM(TIM_CHANNEL_1, left);
    SetPWM(TIM_CHANNEL_2, 0);

    SetPWM(TIM_CHANNEL_3, right);
    SetPWM(TIM_CHANNEL_4, 0);
}

static void Motor_Stop(void)
{
    SetPWM(TIM_CHANNEL_1, 0);
    SetPWM(TIM_CHANNEL_2, 0);
    SetPWM(TIM_CHANNEL_3, 0);
    SetPWM(TIM_CHANNEL_4, 0);
}

static uint8_t Button_Click(void)
{
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15) != GPIO_PIN_RESET)
        return 0;

    HAL_Delay(20);

    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15) != GPIO_PIN_RESET)
        return 0;

    while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15) == GPIO_PIN_RESET)
    {
        HAL_Delay(5);
    }

    return 1;
}

int main(void)
{
    HAL_Init();

    GPIO_Init();
    PWM_Init();

    uint8_t running = 0;

    float error = 0.0f;
    float last_error = 0.0f;
    float integral = 0.0f;

    while (1)
    {
        if (Button_Click())
        {
            running = !running;
            error = 0.0f;
            last_error = 0.0f;
            integral = 0.0f;
        }

        if (!running)
        {
            Motor_Stop();
            HAL_Delay(5);
            continue;
        }

        uint8_t left =
            HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == BLACK_LEVEL;

        uint8_t right =
            HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13) == BLACK_LEVEL;

        if (!left && !right)
        {
            error = 0.0f;
        }
        else if (left && !right)
        {
            error = -1.0f;
        }
        else if (!left && right)
        {
            error = 1.0f;
        }
        else
        {
            if (last_error < 0)
                error = -2.0f;
            else if (last_error > 0)
                error = 2.0f;
            else
                error = 0.0f;
        }

        integral += error;

        if (integral > 20.0f)
            integral = 20.0f;

        if (integral < -20.0f)
            integral = -20.0f;

        float derivative = error - last_error;

        float output =
            Kp * error +
            Ki * integral +
            Kd * derivative;

        if (output > 45.0f)
            output = 45.0f;

        if (output < -45.0f)
            output = -45.0f;

        int16_t left_speed =
            BaseSpeed + (int16_t)output;

        int16_t right_speed =
            BaseSpeed - (int16_t)output;

        left_speed = Clamp(left_speed, 0, 100);
        right_speed = Clamp(right_speed, 0, 100);

        Motor_Set(left_speed, right_speed);

        last_error = error;

        HAL_Delay(10);
    }
}
