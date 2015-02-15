/*
 * This file is part of Neon.
 *
 * Copyright (C) 2010 - 2015 Nenad Radulovic
 *
 * Neon is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Neon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Neon.  If not, see <http://www.gnu.org/licenses/>.
 *
 * web site:    http://github.com/nradulovic
 * e-mail  :    nenad.b.radulovic@gmail.com
 */

/*=========================================================  INCLUDE FILES  ==*/

#include "shared/debug.h"
#include "mcu/profile.h"
#include "mcu/gpio.h"
#include "mcu/uart.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static void system_clock_config(void);
static void error_handler(void);

static void reader(
    struct nuart_driver *       uart,
    enum nerror                 error,
    void *                      buffer,
    size_t                      size);

static void writer(
    struct nuart_driver *       uart,
    enum nerror                 error,
    const void *                buffer,
    size_t                      size);

/*=======================================================  LOCAL VARIABLES  ==*/

static volatile bool            g_is_received = false;
static volatile bool            g_is_sent = false;

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static void system_clock_config(void)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;

    /* Enable Power Control clock */
    __HAL_RCC_PWR_CLK_ENABLE();

    /* The voltage scaling allows optimizing the power consumption when the device is
       clocked below the maximum system frequency, to update the voltage scaling value
       regarding system frequency refer to product datasheet.  */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /* Enable HSI Oscillator and activate PLL with HSI as source */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = 0x10;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 16;
    RCC_OscInitStruct.PLL.PLLN = 400;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        error_handler();
    }

    /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
    clocks dividers */
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
    {
        error_handler();
    }
}

static void error_handler(void)
{
    for (;;);
}

static void reader(
    struct nuart_driver *       uart,
    enum nerror                 error,
    void *                      buffer,
    size_t                      size)
{
    g_is_received = true;
}

static void writer(
    struct nuart_driver *       uart,
    enum nerror                 error,
    const void *                buffer,
    size_t                      size)
{
    g_is_sent = true;
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/


int main(void)
{
    struct nuart_driver *       uart;
    struct nuart_config         config =
    {
        .flags          =   NUART_MODE_RX_TX  |
                            NUART_PARITY_NONE |
                            NUART_STOPBITS_1  |
                            NUART_WORDLENGTH_8,
        .baud_rate      = 9600,
        .tx_gpio        = NGPIO_PIN_ID(NGPIOA, 2),
        .rx_gpio        = NGPIO_PIN_ID(NGPIOA, 3),
    };
    const uint8_t               tx_data[] = "UART test data";
    uint8_t                     rx_data[sizeof(tx_data) + 1u];

    HAL_Init();
    system_clock_config();

    uart = nuart_open(&g_uart2, &config);
    nuart_set_reader(uart, reader);
    nuart_set_writer(uart, writer);
    nuart_read_start(uart, rx_data, sizeof(tx_data), 0);
    nuart_write_start(uart, tx_data, sizeof(tx_data));

    while (!g_is_received && !g_is_sent);

    for (;;);

    return (0);
}



PORT_C_NORETURN void hook_at_assert(
    const PORT_C_ROM struct ncomponent_info * component_info,
    const PORT_C_ROM char *     fn,
    uint32_t                    line,
    const PORT_C_ROM char *     expr,
    const PORT_C_ROM char *     msg)
{
    for (;;);
}



void assert_failed(uint8_t* file, uint32_t line)
{
    (void)file;
    (void)line;
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//***************************************************************
 * END of main.c
 ******************************************************************************/
