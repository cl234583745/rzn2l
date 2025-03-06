/***********************************************************************************************************************
 * Copyright [2020-2023] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
 *
 * This software and documentation are supplied by Renesas Electronics Corporation and/or its affiliates and may only
 * be used with products of Renesas Electronics Corp. and its affiliates ("Renesas").  No other uses are authorized.
 * Renesas products are sold pursuant to Renesas terms and conditions of sale.  Purchasers are solely responsible for
 * the selection and use of Renesas products and Renesas assumes no liability.  No license, express or implied, to any
 * intellectual property right is granted by Renesas.  This software is protected under all applicable laws, including
 * copyright laws. Renesas reserves the right to change or discontinue this software and/or this documentation.
 * THE SOFTWARE AND DOCUMENTATION IS DELIVERED TO YOU "AS IS," AND RENESAS MAKES NO REPRESENTATIONS OR WARRANTIES, AND
 * TO THE FULLEST EXTENT PERMISSIBLE UNDER APPLICABLE LAW, DISCLAIMS ALL WARRANTIES, WHETHER EXPLICITLY OR IMPLICITLY,
 * INCLUDING WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT, WITH RESPECT TO THE
 * SOFTWARE OR DOCUMENTATION.  RENESAS SHALL HAVE NO LIABILITY ARISING OUT OF ANY SECURITY VULNERABILITY OR BREACH.
 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT WILL RENESAS BE LIABLE TO YOU IN CONNECTION WITH THE SOFTWARE OR
 * DOCUMENTATION (OR ANY PERSON OR ENTITY CLAIMING RIGHTS DERIVED FROM YOU) FOR ANY LOSS, DAMAGES, OR CLAIMS WHATSOEVER,
 * INCLUDING, WITHOUT LIMITATION, ANY DIRECT, CONSEQUENTIAL, SPECIAL, INDIRECT, PUNITIVE, OR INCIDENTAL DAMAGES; ANY
 * LOST PROFITS, OTHER ECONOMIC DAMAGE, PROPERTY DAMAGE, OR PERSONAL INJURY; AND EVEN IF RENESAS HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH LOSS, DAMAGES, CLAIMS OR COSTS.
 **********************************************************************************************************************/

#include "hal_data.h"

void R_BSP_WarmStart(bsp_warm_start_event_t event) BSP_PLACE_IN_SECTION(".warm_start");

extern bsp_leds_t g_bsp_leds;


uint32_t buf_left[13]={1000,400, 600, 700, 800, 900, 1000, 1100, 400, 400, 600,800,1000};
uint32_t buf_des[10]={0};

uint32_t next1_des[10]={0};
uint32_t next1_src[13]={400,500, 600, 700, 800, 900, 1000, 1100, 400, 400, 600,800,1000};
uint32_t next1_stop[1]={0xff};

/*******************************************************************************************************************//**
 * @brief  Blinky example application
 *
 * Blinks all leds at a rate of 1 second using the software delay function provided by the BSP.
 *
 **********************************************************************************************************************/
void hal_entry (void)
{
    /* Define the units to be used with the software delay function */
    const bsp_delay_units_t bsp_delay_units = BSP_DELAY_UNITS_MILLISECONDS;

    /* Set the blink frequency (must be <= bsp_delay_units */
    const uint32_t freq_in_hz = 2;

    /* Calculate the delay in terms of bsp_delay_units */
    const uint32_t delay = bsp_delay_units / freq_in_hz;

    /* LED type structure */
    bsp_leds_t leds = g_bsp_leds;

    /* If this board has no LEDs then trap here */
    if (0 == leds.led_count)
    {
        while (1)
        {
            ;                          // There are no LEDs on this board
        }
    }

    /* This code uses BSP IO functions to show how it is used.*/
    /* Turn off LEDs */
    for (uint32_t i = 0; i < leds.led_count; i++)
    {
        R_BSP_PinClear(BSP_IO_REGION_SAFE, (bsp_io_port_pin_t) leds.p_leds[i]);
    }

    __asm volatile ("cpsie i");

    g_timer1.p_api->open(g_timer1.p_ctrl, g_timer1.p_cfg);

    g_transfer0.p_api->open(g_transfer0.p_ctrl, g_transfer0.p_cfg);

    g_transfer0_ctrl.p_cfg->p_info->p_src = (void*)&buf_left[0];
    g_transfer0_ctrl.p_cfg->p_info->p_dest = (void*)&g_timer1_ctrl.p_reg->GTPBR;//(void*)&buf_des[0];//(void*)&g_timer0_ctrl.p_reg->GTPBR;
    g_transfer0_ctrl.p_cfg->p_info->length = 48;

    extern dmac_register_set_setting_t g_transfer0_next1_register_setting;
    g_transfer0_next1_register_setting.p_src = (void*)&next1_stop[0];
    g_transfer0_next1_register_setting.p_dest = (void*)&g_timer1_ctrl.p_reg->GTSTP;//(void*)&g_timer1_ctrl.p_reg->GTPBR;
    g_transfer0_next1_register_setting.length = 4;

    g_transfer0.p_api->reconfigure(g_transfer0.p_ctrl, g_transfer0_ctrl.p_cfg->p_info);

    g_transfer0.p_api->enable(g_transfer0.p_ctrl);
    g_transfer0.p_api->softwareStart(g_transfer0.p_ctrl, (transfer_start_mode_t)0);


    g_timer1.p_api->enable(g_timer1.p_ctrl);
    g_timer1.p_api->start(g_timer1.p_ctrl);


    while (1)
    {
        /* Toggle board LEDs */
        for (uint32_t i = 0; i < leds.led_count; i++)
        {
            R_BSP_PinToggle(BSP_IO_REGION_SAFE, (bsp_io_port_pin_t) leds.p_leds[i]);
        }

        /* Delay */
        R_BSP_SoftwareDelay(delay, bsp_delay_units);
    }
}

static uint32_t dmacnt = 0;
void g_transfer0CB(dmac_callback_args_t *p_args);
void g_transfer0CB(dmac_callback_args_t *p_args)
{
    dmacnt++;//g_timer1.p_api->stop(g_timer1.p_ctrl);
}

/*******************************************************************************************************************//**
 * This function is called at various points during the startup process.  This implementation uses the event that is
 * called right before main() to set up the pins.
 *
 * @param[in]  event    Where at in the start up process the code is currently at
 **********************************************************************************************************************/
void R_BSP_WarmStart (bsp_warm_start_event_t event)
{
    if (BSP_WARM_START_RESET == event)
    {
        /* Pre clock initialization */
    }

    if (BSP_WARM_START_POST_C == event)
    {
        /* C runtime environment and system clocks are setup. */

        /* Configure pins. */
        R_IOPORT_Open(&g_ioport_ctrl, &g_bsp_pin_cfg);
    }
}
