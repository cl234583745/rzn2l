/* generated vector source file - do not edit */
#include "bsp_api.h"
/* Do not build these data structures if no interrupts are currently allocated because IAR will have build errors. */
#if VECTOR_DATA_IRQ_COUNT > 0
        BSP_DONT_REMOVE const fsp_vector_t g_vector_table[BSP_ICU_VECTOR_MAX_ENTRIES] =
        {
                        [21] = dmac_int_isr, /* DMAC0_INT0 (DMAC0 transfer completion 0) */
            [22] = dmac_int_isr, /* DMAC0_INT1 (DMAC0 transfer completion 1) */
            [23] = dmac_int_isr, /* DMAC0_INT2 (DMAC0 transfer completion 2) */
            [288] = sci_uart_eri_isr, /* SCI0_ERI (SCI0 Receive error) */
            [289] = sci_uart_rxi_isr, /* SCI0_RXI (SCI0 Receive data full) */
            [290] = sci_uart_txi_isr, /* SCI0_TXI (SCI0 Transmit data empty) */
            [291] = sci_uart_tei_isr, /* SCI0_TEI (SCI0 Transmit end) */
            [350] = adc_scan_end_isr, /* ADC1_ADI (ADC1 A/D scan end interrupt) */
        };
        #if (1 == BSP_FEATURE_BSP_IRQ_CR52_SEL_SUPPORTED)
        const bsp_interrupt_event_t g_interrupt_event_link_select[BSP_ICU_VECTOR_MAX_ENTRIES] =
        {
            [21] = BSP_PRV_CR52_SEL_ENUM(EVENT_DMAC0_INT0), /* DMAC0_INT0 (DMAC0 transfer completion 0) */
            [22] = BSP_PRV_CR52_SEL_ENUM(EVENT_DMAC0_INT1), /* DMAC0_INT1 (DMAC0 transfer completion 1) */
            [23] = BSP_PRV_CR52_SEL_ENUM(EVENT_DMAC0_INT2), /* DMAC0_INT2 (DMAC0 transfer completion 2) */
            [288] = BSP_PRV_CR52_SEL_ENUM(EVENT_SCI0_ERI), /* SCI0_ERI (SCI0 Receive error) */
            [289] = BSP_PRV_CR52_SEL_ENUM(EVENT_SCI0_RXI), /* SCI0_RXI (SCI0 Receive data full) */
            [290] = BSP_PRV_CR52_SEL_ENUM(EVENT_SCI0_TXI), /* SCI0_TXI (SCI0 Transmit data empty) */
            [291] = BSP_PRV_CR52_SEL_ENUM(EVENT_SCI0_TEI), /* SCI0_TEI (SCI0 Transmit end) */
            [350] = BSP_PRV_CR52_SEL_ENUM(EVENT_ADC1_ADI), /* ADC1_ADI (ADC1 A/D scan end interrupt) */
        };
        #endif
        #endif
