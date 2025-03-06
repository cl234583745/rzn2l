/* generated HAL source file - do not edit */
#include "hal_data.h"

dmac_instance_ctrl_t g_transfer0_ctrl;

dmac_register_set_setting_t g_transfer0_next1_register_setting =
{ .p_dest = NULL, .p_src = NULL, .length = 1 };

dmac_extended_info_t g_transfer0_extend_info =
{ .src_size = DMAC_TRANSFER_SIZE_4_BYTE, .dest_size = DMAC_TRANSFER_SIZE_4_BYTE, .p_next1_register_setting =
          &g_transfer0_next1_register_setting, };

transfer_info_t g_transfer0_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED, .transfer_settings_word_b.repeat_area =
          (transfer_repeat_area_t) 0, // Unused
  .transfer_settings_word_b.irq = (transfer_irq_t) 0, // Unused
  .transfer_settings_word_b.chain_mode = (transfer_chain_mode_t) 0, // Unused
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.size = (transfer_size_t) 0, // Unused
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0, // Unused
  .length = 0,
  .p_extend = &g_transfer0_extend_info, };

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
  .dmac_int_ipl = (12),
  .dmac_int_irq_detect_type = (0),

  .activation_source = ELC_EVENT_GPT1_CCMPA,

  .ack_mode = DMAC_ACK_MODE_BUS_CYCLE_MODE,
  .detection_mode = (dmac_detection_t) ((0) << 2 | (0) << 1 | (1) << 0), .activation_request_source_select =
          DMAC_REQUEST_DIRECTION_SOURCE_MODULE,

  .next_register_operaion = DMAC_REGISTER_SELECT_REVERSE_ENABLE_PERFORM_ACCORDINGLY,

  .dmac_mode = DMAC_MODE_SELECT_REGISTER,
#ifndef NULL
  .p_descriptor = &NULL,
#else
    .p_descriptor           = NULL,
#endif

  .transfer_interval = 0,
  .channel_scheduling = DMAC_CHANNEL_SCHEDULING_FIXED,

  .p_callback = g_transfer0CB,
  .p_context = NULL,

  .p_peripheral_module_handler = NULL, };
const transfer_cfg_t g_transfer0_cfg =
{ .p_info = &g_transfer0_info, .p_extend = &g_transfer0_extend, };
/* Instance structure to use this module. */
const transfer_instance_t g_transfer0 =
{ .p_ctrl = &g_transfer0_ctrl, .p_cfg = &g_transfer0_cfg, .p_api = &g_transfer_on_dmac };
gpt_instance_ctrl_t g_timer1_ctrl;
#if 0
const gpt_extended_pwm_cfg_t g_timer1_pwm_extend =
{
    .trough_ipl          = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT1_UDF)
    .trough_irq          = VECTOR_NUMBER_GPT1_UDF,
#else
    .trough_irq          = FSP_INVALID_VECTOR,
#endif
    .poeg_link           = GPT_POEG_LINK_POEG0,
    .output_disable      =  GPT_OUTPUT_DISABLE_NONE,
    .adc_trigger         =  GPT_ADC_TRIGGER_NONE,
    .dead_time_count_up  = 0,
    .dead_time_count_down = 0,
    .adc_a_compare_match = 0,
    .adc_b_compare_match = 0,
    .interrupt_skip_source = GPT_INTERRUPT_SKIP_SOURCE_NONE,
    .interrupt_skip_count  = GPT_INTERRUPT_SKIP_COUNT_0,
    .interrupt_skip_adc    = GPT_INTERRUPT_SKIP_ADC_NONE,
    .gtioca_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
    .gtiocb_disable_setting = GPT_GTIOC_DISABLE_PROHIBITED,
    .interrupt_skip_source_ext1 = GPT_INTERRUPT_SKIP_SOURCE_NONE,
    .interrupt_skip_count_ext1  = GPT_INTERRUPT_SKIP_COUNT_0,
    .interrupt_skip_source_ext2 = GPT_INTERRUPT_SKIP_SOURCE_NONE,
    .interrupt_skip_count_ext2  = GPT_INTERRUPT_SKIP_COUNT_0,
    .interrupt_skip_func_ovf    = GPT_INTERRUPT_SKIP_SELECT_NONE,
    .interrupt_skip_func_unf    = GPT_INTERRUPT_SKIP_SELECT_NONE,
    .interrupt_skip_func_adc_a  = GPT_INTERRUPT_SKIP_SELECT_NONE,
    .interrupt_skip_func_adc_b  = GPT_INTERRUPT_SKIP_SELECT_NONE,
};
#endif
const gpt_extended_cfg_t g_timer1_extend =
        { .gtioca =
        { .output_enabled = true, .stop_level = GPT_PIN_LEVEL_LOW },
          .gtiocb =
          { .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW },
          .start_source = (gpt_source_t) (GPT_SOURCE_NONE), .stop_source = (gpt_source_t) (GPT_SOURCE_NONE), .clear_source =
                  (gpt_source_t) (GPT_SOURCE_NONE),
          .count_up_source = (gpt_source_t) (GPT_SOURCE_NONE), .count_down_source = (gpt_source_t) (GPT_SOURCE_NONE), .capture_a_source =
                  (gpt_source_t) (GPT_SOURCE_NONE),
          .capture_b_source = (gpt_source_t) (GPT_SOURCE_NONE), .capture_a_ipl = (12), .capture_b_ipl =
                  (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT1_CCMPA)
    .capture_a_irq       = VECTOR_NUMBER_GPT1_CCMPA,
#else
          .capture_a_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_GPT1_CCMPB)
    .capture_b_irq       = VECTOR_NUMBER_GPT1_CCMPB,
#else
          .capture_b_irq = FSP_INVALID_VECTOR,
#endif
          .capture_filter_gtioca = GPT_CAPTURE_FILTER_NONE,
          .capture_filter_gtiocb = GPT_CAPTURE_FILTER_NONE,
#if 0
    .p_pwm_cfg                   = &g_timer1_pwm_extend,
#else
          .p_pwm_cfg = NULL,
#endif
          .dead_time_ipl = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT1_DTE)
    .dead_time_irq       = VECTOR_NUMBER_GPT1_DTE,
#else
          .dead_time_irq = FSP_INVALID_VECTOR,
#endif
          .icds = 0, };
const timer_cfg_t g_timer1_cfg =
{ .mode = TIMER_MODE_PWM,
  /* Actual period: 0.000001 seconds. Actual duty: 50%. */.period_counts = (uint32_t) 0x190,
  .duty_cycle_counts = 0xc8,
  .source_div = (timer_source_div_t) 0,
  .channel = 1,
  .p_callback = NULL,
  .p_context = NULL,
  .p_extend = &g_timer1_extend,
  .cycle_end_ipl = (12),
#if defined(VECTOR_NUMBER_GPT1_OVF)
    .cycle_end_irq       = VECTOR_NUMBER_GPT1_OVF,
#else
  .cycle_end_irq = FSP_INVALID_VECTOR,
#endif
        };
/* Instance structure to use this module. */
const timer_instance_t g_timer1 =
{ .p_ctrl = &g_timer1_ctrl, .p_cfg = &g_timer1_cfg, .p_api = &g_timer_on_gpt };
void g_hal_init(void)
{
    g_common_init ();
}
