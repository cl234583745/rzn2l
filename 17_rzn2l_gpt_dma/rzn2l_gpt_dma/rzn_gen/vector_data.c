/* generated vector source file - do not edit */
#include "bsp_api.h"
/* Do not build these data structures if no interrupts are currently allocated because IAR will have build errors. */
#if VECTOR_DATA_IRQ_COUNT > 0
        BSP_DONT_REMOVE const fsp_vector_t g_vector_table[BSP_ICU_VECTOR_MAX_ENTRIES] =
        {
                        [21] = dmac_int_isr, /* DMAC0_INT0 (DMAC0 transfer completion 0) */
            [194] = gpt_counter_overflow_isr, /* GPT8_OVF (GPT8 GTCNT overflow (GTPR compare match)) */
        };
        #endif
