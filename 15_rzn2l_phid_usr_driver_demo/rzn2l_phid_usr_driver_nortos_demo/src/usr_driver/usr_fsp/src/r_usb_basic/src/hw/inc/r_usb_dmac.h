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

#ifndef R_USB_DMAC_H
#define R_USB_DMAC_H

#if ((USB_CFG_DTC == USB_CFG_ENABLE) || (USB_CFG_DMA == USB_CFG_ENABLE))
 #include "r_usb_basic_define.h"
 #if !defined(BSP_MCU_GROUP_RZT2M) && !defined(BSP_MCU_GROUP_RZT2L) && !defined(BSP_MCU_GROUP_RZN2L)
  #include "r_dmac.h"
 #endif

/*******************************************************************************
 * Macro definitions
 *******************************************************************************/
 #define USB_DMA_USE_CH_MAX            (4)                             /* MAX USE DMAC CH for USB */
 #define USB_DMA_CH_PRI                (12)                            /* DMACmI interrupt priority level for USB Pipe : DxFIFO->buff */
 #define USB_DMA_CH2_PRI               (12)                            /* DMACmI interrupt priority level for USB Pipe : buff->DxFIFO */

 #define USB_FIFO_ACCESS_TYPE_32BIT    (0)                             /* FIFO port 32bit access */
 #define USB_FIFO_ACCESS_TYPE_16BIT    (1)                             /* FIFO port 16bit access */
 #define USB_FIFO_ACCESS_TYPE_8BIT     (2)                             /* FIFO port 8bit access */

 #define USB_FIFO_ACCSESS_TYPE_NUM     (USB_FIFO_ACCESS_TYPE_8BIT + 1) /*  */

 #define USB_BIT_MBW32                 (3UL)                           /* Mod 4(4Byte=32Bit) */
 #define USB_BIT_MBW16                 (1UL)                           /* Mod 2(2Byte=16Bit) */

 #define USB_BIT_32_WIDTH              (4)
 #define USB_BIT_16_WIDTH              (2)

 #if (USB_CFG_DMA == USB_CFG_ENABLE)
  #define USB_DMAC_PRV_REG(ch)    ((R_DMAC0_Type *) ((R_DMAC1 - R_DMAC0) * ch + R_DMAC0))
 #endif                                /* #if (USB_CFG_DMA == USB_CFG_ENABLE) */

 #if defined(BSP_MCU_GROUP_RZT2M) || defined(BSP_MCU_GROUP_RZT2L) || defined(BSP_MCU_GROUP_RZN2L)

  #define USB_CFG_USE_USBIP        (USB_CFG_IP0)
  #define USB_FIFO_TYPE_D0DMA      (0)                       /* D0FIFO Select */
  #define USB_FIFO_TYPE_D1DMA      (1)                       /* D1FIFO Select */
  #define USB_DMA_FIFO_TYPE_NUM    (USB_FIFO_TYPE_D1DMA + 1) /*  */

typedef enum e_usb_dmaca_chcfg_fifo_select
{
    USB_DMACA_CONFIG_SEL_D0FIFO = (0x0 << 0),
    USB_DMACA_CONFIG_SEL_D1FIFO = (0x1 << 0)
} usb_dmaca_chcfg_fifo_select_t;

/* Channel Configuration Register .b03:REQD bit set */
typedef enum e_usb_dmaca_chcfg_dmareq_select
{
    USB_DMACA_CONFIG_USB_SRC = (0x0 << 3),
    USB_DMACA_CONFIG_USB_DES = (0x1 << 3)
} usb_dmaca_chcfg_dmareq_select_t;

/* Channel Configuration Register .b20:SAD bit set */
typedef enum e_usb_dmaca_chcfg_src_dir
{
    USB_DMACA_CONFIG_SRC_ADDRESS_INC = (0x0 << 20),
    USB_DMACA_CONFIG_SRC_ADDRESS_FIX = (0x1 << 20)
} usb_dmaca_chcfg_src_dir_t;

/* Channel Configuration Register .b21:DAD bit set */
typedef enum e_usb_dmaca_chcfg_des_dir
{
    USB_DMACA_CONFIG_DES_ADDRESS_INC = (0x0 << 21),
    USB_DMACA_CONFIG_DES_ADDRESS_FIX = (0x1 << 21)
} usb_dmaca_chcfg_des_dir_t;

/* Channel Configuration Register .b24:DEM bit set */
typedef enum e_usb_dmaca_chcfg_int_msk
{
    USB_DMACA_CONFIG_DMAEND_INT_MASK_OFF = (0x0 << 24),
    USB_DMACA_CONFIG_DMAEND_INT_MASK_ON  = (0x1 << 24)
} usb_dmaca_chcfg_int_msk_t;

/* Channel Configuration Register .b25:TCM bit set */
typedef enum e_usb_dmaca_chcfg_tc_msk
{
    USB_DMACA_CONFIG_DMATC_MASK_OFF = (0x0 << 25),
    USB_DMACA_CONFIG_DMATC_MASK_ON  = (0x1 << 25)
} usb_dmaca_chcfg_tc_msk_t;

/* Channel Interval Register set */
typedef enum e_usb_dmaca_ch_interval
{
    USB_DMACA_INTERVAL_MIN = 0x0000U,
    USB_DMACA_INTERVAL_MAX = 0xffffU
} usb_dmaca_ch_interval_t;

/* DMA Control Register .b00:PR bit set */
typedef enum e_usb_dmaca_dctrl_priority
{
    USB_DMACA_DCTRL_PRIORITY_FIX   = (0x0 << 0),
    USB_DMACA_DCTRL_PRIORITY_ROUND = (0x1 << 0)
} usb_dmaca_dctrl_priority_t;
 #endif                                                                 /* defined(BSP_MCU_GROUP_RZT2M) */

/******************************************************************************
 * Exported global functions (to be accessed by other files)
 ******************************************************************************/
extern uint32_t g_usb_cstd_dma_size[USB_NUM_USBIP][USB_DMA_USE_CH_MAX]; /* DMA0 and DMA1 buffer size */
extern uint16_t g_usb_cstd_dma_fifo[USB_NUM_USBIP][USB_DMA_USE_CH_MAX]; /* DMA0 and DMA1 FIFO buffer size */
extern uint16_t g_usb_cstd_dma_pipe[USB_NUM_USBIP][USB_DMA_USE_CH_MAX]; /* DMA0 and DMA1 pipe number */

void usb_cstd_dma_driver(void);

 #if !defined(BSP_MCU_GROUP_RZT2M) && !defined(BSP_MCU_GROUP_RZT2L) && !defined(BSP_MCU_GROUP_RZN2L)
uint16_t usb_cstd_dma_get_crtb(usb_utr_t * p_utr);

 #else                                 /* BSP_MCU_GROUP_RZT2M */
uint32_t usb_cstd_dma_get_crtb(uint8_t dma_ch);

 #endif /* BSP_MCU_GROUP_RZT2M */
uint16_t usb_cstd_dma_get_ir_vect(usb_utr_t * ptr, uint16_t use_port);
void     usb_cstd_dma_clear_ir(usb_utr_t * ptr, uint16_t use_port);
void     usb_cstd_dma_rcv_setting(usb_utr_t * ptr, uint32_t des_addr, uint16_t useport, uint32_t transfer_size);
void     usb_cstd_dma_send_setting(usb_utr_t * ptr, uint32_t src_adr, uint16_t useport, uint32_t transfer_size);
void     usb_cstd_dma_stop(usb_utr_t * p_utr, uint16_t use_port);
void     usb_cstd_dma_status_clr(usb_utr_t * p_utr, uint16_t use_port);
void     usb_cstd_dfifo_end(usb_utr_t * ptr, uint16_t useport);
uint32_t hw_usb_get_dxfifo_adr(usb_utr_t * ptr, uint16_t use_port, uint16_t bit_width);

 #if !defined(BSP_MCU_GROUP_RZT2M) && !defined(BSP_MCU_GROUP_RZT2L) && !defined(BSP_MCU_GROUP_RZN2L)
uint8_t usb_cstd_dma_ref_ch_no(usb_utr_t * p_utr, uint16_t use_port);

 #endif                                /* BSP_MCU_GROUP_RZT2M */
void usb_cstd_dma_send_continue(usb_utr_t * ptr, uint16_t useport);

void usb_cstd_dma_rcv_start(usb_utr_t * ptr, uint16_t pipe, uint16_t useport);
void usb_cstd_dma_send_start(usb_utr_t * ptr, uint16_t pipe, uint16_t useport);
void usb_cstd_dma_send_restart(usb_utr_t * ptr, uint32_t src, uint32_t data_size, uint8_t pipe);
void usb_cstd_dma_send_complete(uint8_t ip_no, uint16_t use_port);

 #if !defined(BSP_MCU_GROUP_RZT2M) && !defined(BSP_MCU_GROUP_RZT2L) && !defined(BSP_MCU_GROUP_RZN2L)
void usb_ip0_d0fifo_callback(dmac_callback_args_t * cb_data);
void usb_ip0_d1fifo_callback(dmac_callback_args_t * cb_data);
void usb_ip1_d0fifo_callback(dmac_callback_args_t * cb_data);
void usb_ip1_d1fifo_callback(dmac_callback_args_t * cb_data);

 #endif

#endif                                 /* ((USB_CFG_DTC == USB_CFG_ENABLE) || (USB_CFG_DMA == USB_CFG_ENABLE)) */

#endif                                 /* R_USB_DMAC_H */

/******************************************************************************
 * End  Of File
 ******************************************************************************/
