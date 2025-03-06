/* generated vector header file - do not edit */
#ifndef VECTOR_DATA_H
#define VECTOR_DATA_H
#include "bsp_api.h"

/** Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/* Number of interrupts allocated */
#ifndef VECTOR_DATA_IRQ_COUNT
#define VECTOR_DATA_IRQ_COUNT    (8)
#endif
/* ISR prototypes */
void dmac_int_isr(void);
void sci_uart_eri_isr(void);
void sci_uart_rxi_isr(void);
void sci_uart_txi_isr(void);
void sci_uart_tei_isr(void);
void adc_scan_end_isr(void);

/* Vector table allocations */
#define VECTOR_NUMBER_DMAC0_INT0 ((IRQn_Type) 21) /* DMAC0_INT0 (DMAC0 transfer completion 0) */
#define VECTOR_NUMBER_DMAC0_INT1 ((IRQn_Type) 22) /* DMAC0_INT1 (DMAC0 transfer completion 1) */
#define VECTOR_NUMBER_DMAC0_INT2 ((IRQn_Type) 23) /* DMAC0_INT2 (DMAC0 transfer completion 2) */
#define VECTOR_NUMBER_SCI0_ERI ((IRQn_Type) 288) /* SCI0_ERI (SCI0 Receive error) */
#define VECTOR_NUMBER_SCI0_RXI ((IRQn_Type) 289) /* SCI0_RXI (SCI0 Receive data full) */
#define VECTOR_NUMBER_SCI0_TXI ((IRQn_Type) 290) /* SCI0_TXI (SCI0 Transmit data empty) */
#define VECTOR_NUMBER_SCI0_TEI ((IRQn_Type) 291) /* SCI0_TEI (SCI0 Transmit end) */
#define VECTOR_NUMBER_ADC1_ADI ((IRQn_Type) 350) /* ADC1_ADI (ADC1 A/D scan end interrupt) */
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
    DMAC0_INT1_IRQn = 22, /* DMAC0_INT1 (DMAC0 transfer completion 1) */
    DMAC0_INT2_IRQn = 23, /* DMAC0_INT2 (DMAC0 transfer completion 2) */
    SCI0_ERI_IRQn = 288, /* SCI0_ERI (SCI0 Receive error) */
    SCI0_RXI_IRQn = 289, /* SCI0_RXI (SCI0 Receive data full) */
    SCI0_TXI_IRQn = 290, /* SCI0_TXI (SCI0 Transmit data empty) */
    SCI0_TEI_IRQn = 291, /* SCI0_TEI (SCI0 Transmit end) */
    ADC1_ADI_IRQn = 350, /* ADC1_ADI (ADC1 A/D scan end interrupt) */
    SHARED_PERIPHERAL_INTERRUPTS_MAX_ENTRIES = BSP_VECTOR_TABLE_MAX_ENTRIES
} IRQn_Type;

/** Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif /* VECTOR_DATA_H */
