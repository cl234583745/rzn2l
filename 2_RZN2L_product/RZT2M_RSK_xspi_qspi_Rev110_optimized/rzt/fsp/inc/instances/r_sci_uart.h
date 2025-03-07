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

#ifndef R_SCI_UART_H
#define R_SCI_UART_H

/*******************************************************************************************************************//**
 * @addtogroup SCI_UART
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
#include "r_uart_api.h"
#include "r_sci_uart_cfg.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define SCI_UART_CODE_VERSION_MAJOR    (1U) // DEPRECATED
#define SCI_UART_CODE_VERSION_MINOR    (2U) // DEPRECATED

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** Enumeration for SCI clock source */
typedef enum e_sci_clk_src
{
    SCI_UART_CLOCK_INT,                      ///< Use internal clock for baud generation
    SCI_UART_CLOCK_INT_WITH_BAUDRATE_OUTPUT, ///< Use internal clock for baud generation and output on SCK
    SCI_UART_CLOCK_EXT8X,                    ///< Use external clock 8x baud rate
    SCI_UART_CLOCK_EXT16X                    ///< Use external clock 16x baud rate
} sci_clk_src_t;

/** UART communication mode definition */
typedef enum e_uart_mode
{
    UART_MODE_RS232    = 0U,           ///< Enables RS232 communication mode
    UART_MODE_RS485_HD = 1U,           ///< Enables RS485 half duplex communication mode
    UART_MODE_RS485_FD = 2U,           ///< Enables RS485 full duplex communication mode
} uart_mode_t;

/** UART instance control block. */
typedef struct st_sci_uart_instance_ctrl
{
    /* Parameters to control UART peripheral device */
    uint8_t  fifo_depth;               // FIFO depth of the UART channel
    uint8_t  rx_transfer_in_progress;  // Set to 1 if a receive transfer is in progress, 0 otherwise
    uint8_t  data_bytes         : 2;   // 1 byte for 7 or 8 bit data, 2 bytes for 9 bit data
    uint8_t  bitrate_modulation : 1;   // 1 if bit rate modulation is enabled, 0 otherwise
    uint32_t open;                     // Used to determine if the channel is configured

    bsp_io_port_pin_t flow_pin;

    /* Source buffer pointer used to fill hardware FIFO from transmit ISR. */
    uint8_t const * p_tx_src;

    /* Size of source buffer pointer used to fill hardware FIFO from transmit ISR. */
    uint32_t tx_src_bytes;

    /* Destination buffer pointer used for receiving data. */
    uint8_t const * p_rx_dest;

    /* Size of destination buffer pointer used for receiving data. */
    uint32_t rx_dest_bytes;

    /* Pointer to the configuration block. */
    uart_cfg_t const * p_cfg;

    /* Base register for this channel */
    R_SCI0_Type * p_reg;

    void (* p_callback)(uart_callback_args_t *); // Pointer to callback that is called when a uart_event_t occurs.
    uart_callback_args_t * p_callback_memory;    // Pointer to non-secure memory that can be used to pass arguments to a callback in non-secure memory.

    /* Pointer to context to be passed into callback function */
    void const * p_context;
} sci_uart_instance_ctrl_t;

/** Receive FIFO trigger configuration. */
typedef enum e_sci_uart_rx_fifo_trigger
{
    SCI_UART_RX_FIFO_TRIGGER_1   = 0x1, ///< Callback after each byte is received without buffering
    SCI_UART_RX_FIFO_TRIGGER_MAX = 0xF, ///< Callback when FIFO is full or after 15 bit times with no data (fewer interrupts)
} sci_uart_rx_fifo_trigger_t;

/** Asynchronous Start Bit Edge Detection configuration. */
typedef enum e_sci_uart_start_bit
{
    SCI_UART_START_BIT_LOW_LEVEL    = 0x0, ///< Detect low level on RXDn pin as start bit
    SCI_UART_START_BIT_FALLING_EDGE = 0x1, ///< Detect falling level on RXDn pin as start bit
} sci_uart_start_bit_t;

/** Noise cancellation configuration. */
typedef enum e_sci_uart_noise_cancellation
{
    SCI_UART_NOISE_CANCELLATION_DISABLE                 = 0x0, ///< Disable noise cancellation
    SCI_UART_NOISE_CANCELLATION_ENABLE                  = 0x1, ///< Enable noise cancellation, The base clock signal divided by 1
    SCI_UART_NOISE_CANCELLATION_ENABLE_FILTER_CKS_DIV_1 = 0x2, ///< Enable noise cancellation, The on-chip baud rate generator source clock divided by 1
    SCI_UART_NOISE_CANCELLATION_ENABLE_FILTER_CKS_DIV_2 = 0x3, ///< Enable noise cancellation, The on-chip baud rate generator source clock divided by 2
    SCI_UART_NOISE_CANCELLATION_ENABLE_FILTER_CKS_DIV_4 = 0x4, ///< Enable noise cancellation, The on-chip baud rate generator source clock divided by 4
    SCI_UART_NOISE_CANCELLATION_ENABLE_FILTER_CKS_DIV_8 = 0x5, ///< Enable noise cancellation, The on-chip baud rate generator source clock divided by 8
} sci_uart_noise_cancellation_t;

/** CTS/RTS function of the SSn pin. */
typedef enum e_sci_uart_ctsrts_config
{
    SCI_UART_CTSRTS_RTS_OUTPUT = 0x0,  ///< Disable CTS function (RTS output function is enabled)
    SCI_UART_CTSRTS_CTS_INPUT  = 0x1,  ///< Enable CTS function
} sci_uart_ctsrts_config_t;

/** Synchronizer circuit configuration. */
typedef enum e_sci_uart_synchronizer
{
    SCI_UART_SYNCHRONIZER_NOT_BYPASS = 0x0, ///< Synchronizer bypass disable
    SCI_UART_SYNCHRONIZER_BYPASS     = 0x1  ///< Synchronizer bypass enable
} sci_uart_synchronizer_t;

/** Register settings to achieve a desired baud rate and modulation duty. */
typedef struct st_baud_setting_t
{
    union
    {
        uint32_t ccr2_baudrate_bits;

        struct
        {
            uint8_t       : 4;
            uint8_t bgdm  : 1;         ///< Baud Rate Generator Double-Speed Mode Select
            uint8_t abcs  : 1;         ///< Asynchronous Mode Base Clock Select
            uint8_t abcse : 1;         ///< Asynchronous Mode Extended Base Clock Select 1
            uint8_t       : 1;
            uint8_t brr   : 8;         ///< Bit Rate Register setting
            uint8_t brme  : 1;         ///< Bit Rate Modulation Enable
            uint8_t       : 3;
            uint8_t cks   : 2;         ///< CKS  value to get divisor (CKS = N)
            uint8_t       : 2;
            uint8_t mddr  : 8;         ///< Modulation Duty Register setting
        };
    };
} baud_setting_t;

/** UART on SCI device Configuration */
typedef struct st_sci_uart_extended_cfg
{
    sci_clk_src_t                 clock;         ///< The source clock for the baud-rate generator. If internal optionally output baud rate on SCK
    sci_uart_start_bit_t          rx_edge_start; ///< Start reception on falling edge
    sci_uart_noise_cancellation_t noise_cancel;  ///< Noise cancellation setting

    baud_setting_t * p_baud_setting;             ///< Register settings for a desired baud rate.

    sci_uart_rx_fifo_trigger_t rx_fifo_trigger;  ///< Receive FIFO trigger level, unused if channel has no FIFO or if DMAC is used.

    uart_mode_t              uart_mode;          ///< UART communication mode selection
    bsp_io_port_pin_t        flow_control_pin;   ///< UART Driver Enable pin
    sci_uart_ctsrts_config_t ctsrts_en;          ///< CTS/RTS function of the SSn pin
    sci_uart_synchronizer_t  sync_bypass;        ///< Clock synchronizer selection
} sci_uart_extended_cfg_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const uart_api_t g_uart_on_sci;

/** @endcond */

fsp_err_t R_SCI_UART_Open(uart_ctrl_t * const p_api_ctrl, uart_cfg_t const * const p_cfg);
fsp_err_t R_SCI_UART_Read(uart_ctrl_t * const p_api_ctrl, uint8_t * const p_dest, uint32_t const bytes);
fsp_err_t R_SCI_UART_Write(uart_ctrl_t * const p_api_ctrl, uint8_t const * const p_src, uint32_t const bytes);
fsp_err_t R_SCI_UART_BaudSet(uart_ctrl_t * const p_api_ctrl, void const * const p_baud_setting);
fsp_err_t R_SCI_UART_InfoGet(uart_ctrl_t * const p_api_ctrl, uart_info_t * const p_info);
fsp_err_t R_SCI_UART_Close(uart_ctrl_t * const p_api_ctrl);
fsp_err_t R_SCI_UART_VersionGet(fsp_version_t * p_version);
fsp_err_t R_SCI_UART_Abort(uart_ctrl_t * const p_api_ctrl, uart_dir_t communication_to_abort);
fsp_err_t R_SCI_UART_BaudCalculate(uint32_t               baudrate,
                                   bool                   bitrate_modulation,
                                   uint32_t               baud_rate_error_x_1000,
                                   baud_setting_t * const p_baud_setting);
fsp_err_t R_SCI_UART_CallbackSet(uart_ctrl_t * const          p_api_ctrl,
                                 void (                     * p_callback)(uart_callback_args_t *),
                                 void const * const           p_context,
                                 uart_callback_args_t * const p_callback_memory);

/*******************************************************************************************************************//**
 * @} (end addtogroup SCI_UART)
 **********************************************************************************************************************/

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif
