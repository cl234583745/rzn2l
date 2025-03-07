/* generated vector header file - do not edit */
#ifndef VECTOR_DATA_H
#define VECTOR_DATA_H
#include "bsp_api.h"
/* Number of interrupts allocated */
#ifndef VECTOR_DATA_IRQ_COUNT
#define VECTOR_DATA_IRQ_COUNT    (3)
#endif
/* ISR prototypes */
void dmac_int_isr(void);
void gpt_capture_a_isr(void);
void gpt_counter_overflow_isr(void);

/* Vector table allocations */
#define VECTOR_NUMBER_DMAC0_INT0 ((IRQn_Type) 21) /* DMAC0_INT0 (DMAC0 transfer completion 0) */
#define VECTOR_NUMBER_GPT1_CCMPA ((IRQn_Type) 125) /* GPT1_CCMPA (GPT1 GTCCRA input capture/compare match) */
#define VECTOR_NUMBER_GPT1_OVF ((IRQn_Type) 131) /* GPT1_OVF (GPT1 GTCNT overflow (GTPR compare match)) */
typedef enum IRQn
{
    SoftwareGeneratedInt0 = -32,
    SoftwareGeneratedInt1 = -31,
    SoftwareGeneratedInt2 = -30,
    SoftwareGeneratedInt3 = -29,
    SoftwareGeneratedInt4 = -28,
    SoftwareGeneratedInt5 = -27,
    SoftwareGeneratedInt6 = -26,
    SoftwareGeneratedInt7 = -25,
    SoftwareGeneratedInt8 = -24,
    SoftwareGeneratedInt9 = -23,
    SoftwareGeneratedInt10 = -22,
    SoftwareGeneratedInt11 = -21,
    SoftwareGeneratedInt12 = -20,
    SoftwareGeneratedInt13 = -19,
    SoftwareGeneratedInt14 = -18,
    SoftwareGeneratedInt15 = -17,
    DebugCommunicationsChannelInt = -10,
    PerformanceMonitorCounterOverflowInt = -9,
    CrossTriggerInterfaceInt = -8,
    VritualCPUInterfaceMaintenanceInt = -7,
    HypervisorTimerInt = -6,
    VirtualTimerInt = -5,
    NonSecurePhysicalTimerInt = -2,
    DMAC0_INT0_IRQn = 21, /* DMAC0_INT0 (DMAC0 transfer completion 0) */
    GPT1_CCMPA_IRQn = 125, /* GPT1_CCMPA (GPT1 GTCCRA input capture/compare match) */
    GPT1_OVF_IRQn = 131, /* GPT1_OVF (GPT1 GTCNT overflow (GTPR compare match)) */
    SHARED_PERIPHERAL_INTERRUPTS_MAX_ENTRIES = BSP_VECTOR_TABLE_MAX_ENTRIES
} IRQn_Type;
#endif /* VECTOR_DATA_H */
