/* generated HAL source file - do not edit */
#include "hal_data.h"
xspi_qspi_instance_ctrl_t g_qspi0_ctrl;

static const spi_flash_erase_command_t g_qspi0_erase_command_list[] =
{
#if 4096 > 0
  { .command = 0x20, .size = 4096 },
#endif
#if 32768 > 0
  { .command = 0x52, .size = 32768 },
#endif
#if 65536 > 0
  { .command = 0xD8, .size = 65536 },
#endif
#if 0xC7 > 0
  { .command = 0xC7, .size = SPI_FLASH_ERASE_SIZE_CHIP_ERASE },
#endif
        };

static xspi_qspi_timing_setting_t g_qspi0_timing_settings =
{ .command_to_command_interval = XSPI_QSPI_COMMAND_INTERVAL_CLOCKS_7, .cs_pullup_lag =
          XSPI_QSPI_CS_PULLUP_CLOCKS_NO_EXTENSION,
  .cs_pulldown_lead = XSPI_QSPI_CS_PULLDOWN_CLOCKS_NO_EXTENSION };

static const xspi_qspi_extended_cfg_t g_qspi0_extended_cfg =
{ .unit = 0, .chip_select = XSPI_QSPI_CHIP_SELECT_0, .memory_size = XSPI_QSPI_MEMORY_SIZE_64MB, .p_timing_settings =
          &g_qspi0_timing_settings,
#if 0 == 0
  .prefetch_en = (xspi_qspi_prefetch_function_t) XSPI_QSPI_CFG_UNIT_0_PREFETCH_FUNCTION,
#else
    .prefetch_en       = (xspi_qspi_prefetch_function_t) XSPI_QSPI_CFG_UNIT_1_PREFETCH_FUNCTION,
#endif
        };
const spi_flash_cfg_t g_qspi0_cfg =
{ .spi_protocol = SPI_FLASH_PROTOCOL_1S_1S_1S,
  .address_bytes = SPI_FLASH_ADDRESS_BYTES_3,
  .dummy_clocks = SPI_FLASH_DUMMY_CLOCKS_8,
  .read_command = 0x03,
  .page_program_command = 0x02,
  .write_enable_command = 0x06,
  .status_command = 0x05,
  .write_status_bit = 0,
  .xip_enter_command = 0x20,
  .xip_exit_command = 0xFF,
  .p_erase_command_list = &g_qspi0_erase_command_list[0],
  .erase_command_list_length = sizeof(g_qspi0_erase_command_list) / sizeof(g_qspi0_erase_command_list[0]),
  .p_extend = &g_qspi0_extended_cfg, };
/** This structure encompasses everything that is needed to use an instance of this interface. */
const spi_flash_instance_t g_qspi0 =
{ .p_ctrl = &g_qspi0_ctrl, .p_cfg = &g_qspi0_cfg, .p_api = &g_spi_flash_on_xspi_qspi, };
crc_instance_ctrl_t g_crc0_ctrl;
const crc_cfg_t g_crc0_cfg =
{ .channel = 0,
  .polynomial = CRC_POLYNOMIAL_CRC_32C,
  .bit_order = CRC_BIT_ORDER_LMS_MSB,
  .snoop_address = 0x00,
  .p_extend = NULL, };

/* Instance structure to use this module. */
const crc_instance_t g_crc0 =
{ .p_ctrl = &g_crc0_ctrl, .p_cfg = &g_crc0_cfg, .p_api = &g_crc_on_crc };
usb_instance_ctrl_t g_basic0_ctrl;

#if !defined(g_usb_descriptor)
extern usb_descriptor_t g_usb_descriptor;
#endif
#define FSP_NOT_DEFINED (1)
const usb_cfg_t g_basic0_cfg =
{ .usb_mode = USB_MODE_PERI, .usb_speed = USB_SPEED_HS, .module_number = 0, .type = USB_CLASS_PCDC,
#if defined(g_usb_descriptor)
                .p_usb_reg = g_usb_descriptor,
#else
  .p_usb_reg = &g_usb_descriptor,
#endif
  .usb_complience_cb = NULL,
#if defined(VECTOR_NUMBER_USB_FI)
                .irq       = VECTOR_NUMBER_USB_FI,
#elif defined(VECTOR_NUMBER_USB_HI)
                .irq       = VECTOR_NUMBER_USB_HI,
#else
  .irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_USBFS_RESUME)
                .irq_r     = VECTOR_NUMBER_USBFS_RESUME,
#else
  .irq_r = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_USB_FDMA0)
                .irq_d0    = VECTOR_NUMBER_USB_FDMA0,
#else
  .irq_d0 = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_USB_FDMA1)
                .irq_d1    = VECTOR_NUMBER_USB_FDMA1,
#else
  .irq_d1 = FSP_INVALID_VECTOR,
#endif
  .hsirq = FSP_INVALID_VECTOR,
  .hsirq_d0 = FSP_INVALID_VECTOR, .hsirq_d1 = FSP_INVALID_VECTOR, .ipl = (12), .ipl_r = (12), .ipl_d0 = (12), .ipl_d1 =
          (12),
  .hsipl = (12), .hsipl_d0 = (12), .hsipl_d1 = (12),
#if (BSP_CFG_RTOS == 2)
                .p_usb_apl_callback = NULL,
#else
  .p_usb_apl_callback = NULL,
#endif
#if defined(NULL)
                .p_context = NULL,
#else
  .p_context = &NULL,
#endif
        };
#undef FSP_NOT_DEFINED

/* Instance structure to use this module. */
const usb_instance_t g_basic0 =
{ .p_ctrl = &g_basic0_ctrl, .p_cfg = &g_basic0_cfg, .p_api = &g_usb_on_usb, };

sci_uart_instance_ctrl_t g_uart0_ctrl;

#define FSP_NOT_DEFINED (1)
#if (FSP_NOT_DEFINED) != (FSP_NOT_DEFINED)

            /* If the transfer module is DMAC, define a DMAC transfer callback. */
            extern void sci_uart_tx_dmac_callback(sci_uart_instance_ctrl_t * p_instance_ctrl);

            void g_uart0_tx_transfer_callback (transfer_callback_args_t * p_args)
            {
                FSP_PARAMETER_NOT_USED(p_args);
                sci_uart_tx_dmac_callback(&g_uart0_ctrl);
            }
            #endif

#if (FSP_NOT_DEFINED) != (FSP_NOT_DEFINED)

            /* If the transfer module is DMAC, define a DMAC transfer callback. */
            extern void sci_uart_rx_dmac_callback(sci_uart_instance_ctrl_t * p_instance_ctrl);

            void g_uart0_rx_transfer_callback (transfer_callback_args_t * p_args)
            {
                FSP_PARAMETER_NOT_USED(p_args);
                sci_uart_rx_dmac_callback(&g_uart0_ctrl);
            }
            #endif
#undef FSP_NOT_DEFINED

sci_baud_setting_t g_uart0_baud_setting =
        {
        /* Baud rate calculated with 0.160% error. */.baudrate_bits_b.abcse = 0,
          .baudrate_bits_b.abcs = 0, .baudrate_bits_b.bgdm = 1, .baudrate_bits_b.cks = 0, .baudrate_bits_b.brr = 51, .baudrate_bits_b.mddr =
                  (uint8_t) 256,
          .baudrate_bits_b.brme = false };

/** UART extended configuration for UARTonSCI HAL driver */
const sci_uart_extended_cfg_t g_uart0_cfg_extend =
{ .clock = SCI_UART_CLOCK_INT, .rx_edge_start = SCI_UART_START_BIT_FALLING_EDGE, .noise_cancel =
          SCI_UART_NOISE_CANCELLATION_DISABLE,
  .rx_fifo_trigger = SCI_UART_RX_FIFO_TRIGGER_MAX, .p_baud_setting = &g_uart0_baud_setting,
#if 1
  .clock_source = SCI_UART_CLOCK_SOURCE_SCI0ASYNCCLK,
#else
                .clock_source           = SCI_UART_CLOCK_SOURCE_PCLKM,
#endif
  .flow_control = SCI_UART_FLOW_CONTROL_RTS,
#if 0xFF != 0xFF
                .flow_control_pin       = BSP_IO_PORT_FF_PIN_0xFF,
                #else
  .flow_control_pin = (bsp_io_port_pin_t) UINT16_MAX,
#endif
  .rs485_setting =
  { .enable = SCI_UART_RS485_DISABLE, .polarity = SCI_UART_RS485_DE_POLARITY_HIGH, .assertion_time = 1, .negation_time =
            1, }, };

/** UART interface configuration */
const uart_cfg_t g_uart0_cfg =
{ .channel = 0, .data_bits = UART_DATA_BITS_8, .parity = UART_PARITY_OFF, .stop_bits = UART_STOP_BITS_1, .p_callback =
          user_uart_callback,
  .p_context = NULL, .p_extend = &g_uart0_cfg_extend, .p_transfer_tx = g_uart0_P_TRANSFER_TX, .p_transfer_rx =
          g_uart0_P_TRANSFER_RX,
  .rxi_ipl = (12), .txi_ipl = (12), .tei_ipl = (12), .eri_ipl = (12),
#if defined(VECTOR_NUMBER_SCI0_RXI)
                .rxi_irq             = VECTOR_NUMBER_SCI0_RXI,
#else
  .rxi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI0_TXI)
                .txi_irq             = VECTOR_NUMBER_SCI0_TXI,
#else
  .txi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI0_TEI)
                .tei_irq             = VECTOR_NUMBER_SCI0_TEI,
#else
  .tei_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI0_ERI)
                .eri_irq             = VECTOR_NUMBER_SCI0_ERI,
#else
  .eri_irq = FSP_INVALID_VECTOR,
#endif
        };

/* Instance structure to use this module. */
const uart_instance_t g_uart0 =
{ .p_ctrl = &g_uart0_ctrl, .p_cfg = &g_uart0_cfg, .p_api = &g_uart_on_sci };
void g_hal_init(void)
{
    g_common_init ();
}
