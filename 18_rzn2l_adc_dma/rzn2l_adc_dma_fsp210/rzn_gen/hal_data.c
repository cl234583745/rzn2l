/* generated HAL source file - do not edit */
#include "hal_data.h"
/* Macros to tie dynamic ELC links to ADC_TRIGGER_SYNC_ELC option in adc_trigger_t. */
#define ADC_TRIGGER_ADC0_A      ADC_TRIGGER_SYNC_ELC
#define ADC_TRIGGER_ADC0_B      ADC_TRIGGER_SYNC_ELC
#define ADC_TRIGGER_ADC1_A      ADC_TRIGGER_SYNC_ELC
#define ADC_TRIGGER_ADC1_B      ADC_TRIGGER_SYNC_ELC
#define ADC_TRIGGER_ADC2_A      ADC_TRIGGER_SYNC_ELC
#define ADC_TRIGGER_ADC2_B      ADC_TRIGGER_SYNC_ELC
dmac_instance_ctrl_t g_transfer2_ctrl;

transfer_info_t g_transfer2_info =
{ .dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED, .src_addr_mode = TRANSFER_ADDR_MODE_FIXED, .mode =
          TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL, .p_src = (void const*) NULL, .length = 0, .src_size = TRANSFER_SIZE_1_BYTE, .dest_size =
          TRANSFER_SIZE_1_BYTE,
  .p_next1_src = (void const*) NULL, .p_next1_dest = (void*) NULL, .next1_length = 1 };

#ifndef NULL
extern dmac_link_cfg_t NULL;
#endif

const dmac_extended_cfg_t g_transfer2_extend =
{ .unit = 0, .channel = 2,
#if defined(VECTOR_NUMBER_DMAC0_INT2)
    .dmac_int_irq         = VECTOR_NUMBER_DMAC0_INT2,
#else
  .dmac_int_irq = FSP_INVALID_VECTOR,
#endif
  .dmac_int_ipl = (11),
  .dmac_int_irq_detect_type = (0),

  .activation_source = ELC_EVENT_SCI0_RXI,

  .ack_mode = DMAC_ACK_MODE_BUS_CYCLE_MODE,
  .detection_mode = (dmac_detection_t) ((0) << 2 | (1) << 1 | (0) << 0), .activation_request_source_select =
          DMAC_REQUEST_DIRECTION_SOURCE_MODULE,

  .next_register_operation = DMAC_REGISTER_SELECT_REVERSE_DISABLE,

  .dmac_mode = DMAC_MODE_SELECT_REGISTER,
#ifndef NULL
  .p_descriptor = &NULL,
#else
    .p_descriptor           = NULL,
#endif

  .transfer_interval = 0,
#if 0 == 0
#if 7 >= 2
  .channel_scheduling = (dmac_channel_scheduling_t) DMAC_CFG_CHANNEL_PRIORITY_UNIT0_CHANNEL0_7,
#else
    .channel_scheduling     = (dmac_channel_scheduling_t) DMAC_CFG_CHANNEL_PRIORITY_UNIT0_CHANNEL8_15,
 #endif
#else
 #if 7 >= 2
    .channel_scheduling     = (dmac_channel_scheduling_t) DMAC_CFG_CHANNEL_PRIORITY_UNIT1_CHANNEL0_7,
 #else
    .channel_scheduling     = (dmac_channel_scheduling_t) DMAC_CFG_CHANNEL_PRIORITY_UNIT1_CHANNEL8_15,
 #endif
#endif
  .p_callback = g_uart0_rx_transfer_callback,
  .p_context = NULL, };
const transfer_cfg_t g_transfer2_cfg =
{ .p_info = &g_transfer2_info, .p_extend = &g_transfer2_extend, };
/* Instance structure to use this module. */
const transfer_instance_t g_transfer2 =
{ .p_ctrl = &g_transfer2_ctrl, .p_cfg = &g_transfer2_cfg, .p_api = &g_transfer_on_dmac };
dmac_instance_ctrl_t g_transfer1_ctrl;

transfer_info_t g_transfer1_info =
{ .dest_addr_mode = TRANSFER_ADDR_MODE_FIXED, .src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED, .mode =
          TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL, .p_src = (void const*) NULL, .length = 0, .src_size = TRANSFER_SIZE_1_BYTE, .dest_size =
          TRANSFER_SIZE_1_BYTE,
  .p_next1_src = (void const*) NULL, .p_next1_dest = (void*) NULL, .next1_length = 1 };

#ifndef NULL
extern dmac_link_cfg_t NULL;
#endif

const dmac_extended_cfg_t g_transfer1_extend =
{ .unit = 0, .channel = 1,
#if defined(VECTOR_NUMBER_DMAC0_INT1)
    .dmac_int_irq         = VECTOR_NUMBER_DMAC0_INT1,
#else
  .dmac_int_irq = FSP_INVALID_VECTOR,
#endif
  .dmac_int_ipl = (11),
  .dmac_int_irq_detect_type = (0),

  .activation_source = ELC_EVENT_SCI0_TXI,

  .ack_mode = DMAC_ACK_MODE_BUS_CYCLE_MODE,
  .detection_mode = (dmac_detection_t) ((0) << 2 | (1) << 1 | (0) << 0), .activation_request_source_select =
          DMAC_REQUEST_DIRECTION_DESTINATION_MODULE,

  .next_register_operation = DMAC_REGISTER_SELECT_REVERSE_DISABLE,

  .dmac_mode = DMAC_MODE_SELECT_REGISTER,
#ifndef NULL
  .p_descriptor = &NULL,
#else
    .p_descriptor           = NULL,
#endif

  .transfer_interval = 0,
#if 0 == 0
#if 7 >= 1
  .channel_scheduling = (dmac_channel_scheduling_t) DMAC_CFG_CHANNEL_PRIORITY_UNIT0_CHANNEL0_7,
#else
    .channel_scheduling     = (dmac_channel_scheduling_t) DMAC_CFG_CHANNEL_PRIORITY_UNIT0_CHANNEL8_15,
 #endif
#else
 #if 7 >= 1
    .channel_scheduling     = (dmac_channel_scheduling_t) DMAC_CFG_CHANNEL_PRIORITY_UNIT1_CHANNEL0_7,
 #else
    .channel_scheduling     = (dmac_channel_scheduling_t) DMAC_CFG_CHANNEL_PRIORITY_UNIT1_CHANNEL8_15,
 #endif
#endif
  .p_callback = g_uart0_tx_transfer_callback,
  .p_context = NULL, };
const transfer_cfg_t g_transfer1_cfg =
{ .p_info = &g_transfer1_info, .p_extend = &g_transfer1_extend, };
/* Instance structure to use this module. */
const transfer_instance_t g_transfer1 =
{ .p_ctrl = &g_transfer1_ctrl, .p_cfg = &g_transfer1_cfg, .p_api = &g_transfer_on_dmac };
sci_uart_instance_ctrl_t g_uart0_ctrl;

#define FSP_NOT_DEFINED (1)
#if (FSP_NOT_DEFINED) != (g_transfer1)

/* If the transfer module is DMAC, define a DMAC transfer callback. */
extern void sci_uart_tx_dmac_callback(sci_uart_instance_ctrl_t *p_instance_ctrl);

void g_uart0_tx_transfer_callback(transfer_callback_args_t *p_args)
{
    FSP_PARAMETER_NOT_USED (p_args);
    sci_uart_tx_dmac_callback (&g_uart0_ctrl);
}
#endif

#if (FSP_NOT_DEFINED) != (g_transfer2)

/* If the transfer module is DMAC, define a DMAC transfer callback. */
extern void sci_uart_rx_dmac_callback(sci_uart_instance_ctrl_t *p_instance_ctrl);

void g_uart0_rx_transfer_callback(transfer_callback_args_t *p_args)
{
    FSP_PARAMETER_NOT_USED (p_args);
    sci_uart_rx_dmac_callback (&g_uart0_ctrl);
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
dmac_instance_ctrl_t g_transfer0_ctrl;

transfer_info_t g_transfer0_info =
{ .dest_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED, .src_addr_mode = TRANSFER_ADDR_MODE_FIXED, .mode =
          TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL, .p_src = (void const*) NULL, .length = 1, .src_size = TRANSFER_SIZE_2_BYTE, .dest_size =
          TRANSFER_SIZE_2_BYTE,
  .p_next1_src = (void const*) NULL, .p_next1_dest = (void*) NULL, .next1_length = 1 };

#ifndef NULL
extern dmac_link_cfg_t NULL;
#endif

const dmac_extended_cfg_t g_transfer0_extend =
{ .unit = 0, .channel = 0,
#if defined(VECTOR_NUMBER_DMAC0_INT0)
    .dmac_int_irq         = VECTOR_NUMBER_DMAC0_INT0,
#else
  .dmac_int_irq = FSP_INVALID_VECTOR,
#endif
  .dmac_int_ipl = (11),
  .dmac_int_irq_detect_type = (0),

  .activation_source = ELC_EVENT_ADC1_ADI,

  .ack_mode = DMAC_ACK_MODE_BUS_CYCLE_MODE,
  .detection_mode = (dmac_detection_t) ((0) << 2 | (0) << 1 | (1) << 0), .activation_request_source_select =
          DMAC_REQUEST_DIRECTION_SOURCE_MODULE,

  .next_register_operation = DMAC_REGISTER_SELECT_REVERSE_ENABLE_PERFORM_ACCORDINGLY,

  .dmac_mode = DMAC_MODE_SELECT_REGISTER,
#ifndef NULL
  .p_descriptor = &NULL,
#else
    .p_descriptor           = NULL,
#endif

  .transfer_interval = 0,
#if 0 == 0
#if 7 >= 0
  .channel_scheduling = (dmac_channel_scheduling_t) DMAC_CFG_CHANNEL_PRIORITY_UNIT0_CHANNEL0_7,
#else
    .channel_scheduling     = (dmac_channel_scheduling_t) DMAC_CFG_CHANNEL_PRIORITY_UNIT0_CHANNEL8_15,
 #endif
#else
 #if 7 >= 0
    .channel_scheduling     = (dmac_channel_scheduling_t) DMAC_CFG_CHANNEL_PRIORITY_UNIT1_CHANNEL0_7,
 #else
    .channel_scheduling     = (dmac_channel_scheduling_t) DMAC_CFG_CHANNEL_PRIORITY_UNIT1_CHANNEL8_15,
 #endif
#endif
  .p_callback = g_transfer0CB,
  .p_context = NULL, };
const transfer_cfg_t g_transfer0_cfg =
{ .p_info = &g_transfer0_info, .p_extend = &g_transfer0_extend, };
/* Instance structure to use this module. */
const transfer_instance_t g_transfer0 =
{ .p_ctrl = &g_transfer0_ctrl, .p_cfg = &g_transfer0_cfg, .p_api = &g_transfer_on_dmac };
adc_instance_ctrl_t g_adc103_ctrl;
const adc_extended_cfg_t g_adc103_cfg_extend =
{ .add_average_count = ADC_ADD_OFF,
  .clearing = ADC_CLEAR_AFTER_READ_ON,
  .trigger_group_b = ADC_TRIGGER_SYNC_ELC,
  .double_trigger_mode = ADC_DOUBLE_TRIGGER_DISABLED,
  .adc_start_trigger_a = ADC_ACTIVE_TRIGGER_DISABLED,
  .adc_start_trigger_b = ADC_ACTIVE_TRIGGER_DISABLED,
  .adc_start_trigger_c_enabled = 0,
  .adc_start_trigger_c = ADC_ACTIVE_TRIGGER_DISABLED,
  .adc_elc_ctrl = ADC_ELC_SINGLE_SCAN,

#if (1U == BSP_FEATURE_ADC_REGISTER_MASK_TYPE)
#if defined(VECTOR_NUMBER_ADC1_CMPAI)
    .window_a_irq        = VECTOR_NUMBER_ADC1_CMPAI,
#else
    .window_a_irq        = FSP_INVALID_VECTOR,
#endif
    .window_a_ipl        = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_ADC1_CMPBI)
    .window_b_irq      = VECTOR_NUMBER_ADC1_CMPBI,
#else
    .window_b_irq      = FSP_INVALID_VECTOR,
#endif
    .window_b_ipl      = (BSP_IRQ_DISABLED),
#endif
#if (3U == BSP_FEATURE_ADC_REGISTER_MASK_TYPE)
#if defined(VECTOR_NUMBER_ADC121_CMPAI)
    .window_a_irq        = VECTOR_NUMBER_ADC121_CMPAI,
#else
    .window_a_irq        = FSP_INVALID_VECTOR,
#endif
    .window_a_ipl        = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_ADC121_CMPBI)
    .window_b_irq      = VECTOR_NUMBER_ADC121_CMPBI,
#else
    .window_b_irq      = FSP_INVALID_VECTOR,
#endif
    .window_b_ipl      = (BSP_IRQ_DISABLED),
#endif
        };
const adc_cfg_t g_adc103_cfg =
{ .unit = 1, .mode = ADC_MODE_CONTINUOUS_SCAN, .resolution = ADC_RESOLUTION_12_BIT, .alignment =
          (adc_alignment_t) ADC_ALIGNMENT_RIGHT,
  .trigger = ADC_TRIGGER_SOFTWARE, .p_callback = adc_sample_callback, .p_context = NULL, .p_extend =
          &g_adc103_cfg_extend,
#if (1U == BSP_FEATURE_ADC_REGISTER_MASK_TYPE)
#if defined(VECTOR_NUMBER_ADC1_ADI)
    .scan_end_irq        = VECTOR_NUMBER_ADC1_ADI,
#else
    .scan_end_irq        = FSP_INVALID_VECTOR,
#endif
    .scan_end_ipl        = (11),
#if defined(VECTOR_NUMBER_ADC1_GBADI)
    .scan_end_b_irq      = VECTOR_NUMBER_ADC1_GBADI,
#else
    .scan_end_b_irq      = FSP_INVALID_VECTOR,
#endif
    .scan_end_b_ipl      = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_ADC1_GCADI)
    .scan_end_c_irq      = VECTOR_NUMBER_ADC1_GCADI,
#else
    .scan_end_c_irq      = FSP_INVALID_VECTOR,
#endif
    .scan_end_c_ipl      = (BSP_IRQ_DISABLED),
#endif
#if (3U == BSP_FEATURE_ADC_REGISTER_MASK_TYPE)
#if defined(VECTOR_NUMBER_ADC121_ADI)
    .scan_end_irq        = VECTOR_NUMBER_ADC121_ADI,
#else
    .scan_end_irq        = FSP_INVALID_VECTOR,
#endif
    .scan_end_ipl        = (11),
#if defined(VECTOR_NUMBER_ADC121_GBADI)
    .scan_end_b_irq      = VECTOR_NUMBER_ADC121_GBADI,
#else
    .scan_end_b_irq      = FSP_INVALID_VECTOR,
#endif
    .scan_end_b_ipl      = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_ADC121_GCADI)
    .scan_end_c_irq      = VECTOR_NUMBER_ADC121_GCADI,
#else
    .scan_end_c_irq      = FSP_INVALID_VECTOR,
#endif
    .scan_end_c_ipl      = (BSP_IRQ_DISABLED),
#endif
        };

#if ((0) | (0))
const adc_window_cfg_t g_adc103_window_cfg =
{
    .compare_mask        =  0,
    .compare_mode_mask   =  0,
    .compare_cfg         = (0) | (0) | (0),
    .compare_ref_low     = 0,
    .compare_ref_high    = 0,
    .compare_b_channel   = (ADC_WINDOW_B_CHANNEL_0),
    .compare_b_mode      = (ADC_WINDOW_B_MODE_LESS_THAN_OR_OUTSIDE),
    .compare_b_ref_low   = 0,
    .compare_b_ref_high  = 0,
};
#endif
const adc_channel_cfg_t g_adc103_channel_cfg =
{ .scan_mask = ADC_MASK_CHANNEL_3 | 0, .scan_mask_group_b = 0, .priority_group_a = ADC_GROUP_A_PRIORITY_OFF, .add_mask =
          0,
  .sample_hold_mask = 0, .sample_hold_states = 24, .scan_mask_group_c = 0,
#if ((0) | (0))
    .p_window_cfg        = (adc_window_cfg_t *) &g_adc103_window_cfg,
#else
  .p_window_cfg = NULL,
#endif
        };
/* Instance structure to use this module. */
const adc_instance_t g_adc103 =
{ .p_ctrl = &g_adc103_ctrl, .p_cfg = &g_adc103_cfg, .p_channel_cfg = &g_adc103_channel_cfg, .p_api = &g_adc_on_adc };
void g_hal_init(void)
{
    g_common_init ();
}
