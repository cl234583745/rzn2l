/***********************************************************************************************************************
 * File Name    : usb_phid_application.c
 * Description  : usb_phid_main
 * jerry.chen@wtmec.com
 * 2024、4、9
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include <stdio.h>
#include "hal_data.h"
#include "usr_hal_data.h"
#include "r_usb_basic.h"
#include "r_usb_extern.h"
#include "r_usb_basic_api.h"
#include "r_usb_phid_api.h"
#include "usb_phid_application.h"

#include"logger.h"
static enum LOG_LEVEL logger=LOG_LEVEL_ALL;
/******************************************************************************
 * Macro definitions
 ******************************************************************************/

#define USB_PHID_USEGPT     1//for test
#define USB_PHID_USECB      1//for test

#if !USB_PHID_USECB
#define APL_NUM_USB_EVENT   (32U)
#endif//#if !USB_PHID_USECB


/******************************************************************************
 * Exported global variables (to be accessed by other files)
 ******************************************************************************/
extern uint8_t g_apl_configuration[];
extern uint8_t g_apl_report[];

/******************************************************************************
 * Private global variables and functions
 ******************************************************************************/
static void usb_enumeration(void);
static void usb_status_update(void);
static void deinit_usb(void);
static void init_event_setup(void);

void usb_apl_callback(usb_callback_args_t * p_event);

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/
static uint8_t send_data[32];
static uint8_t *p_idle_value = NULL;
static uint8_t g_idle = 0;
static uint8_t g_buf[64]  = {0}; /* HID NULL data */
static uint16_t g_numlock = 0;

#if !USB_PHID_USEGPT
static uint8_t testbuf[64] = {0};
#endif//#if !USB_PHID_USEGPT

#if USB_PHID_USECB
static uint8_t send_buf[64] =   {0};
#endif//#if USB_PHID_USECB

#if !USB_PHID_USECB
static usb_callback_args_t g_apl_usb_event_buf[APL_NUM_USB_EVENT];
static volatile uint8_t g_apl_usb_event_wp = 0;//must volatile
static volatile uint8_t g_apl_usb_event_rp = 0;//must volatile
#endif//#if !USB_PHID_USECB

static usb_event_info_t event_info;//setup.request_type Must be obtained from event, ctrl is empty
static usb_event_setup_t event_setup;

static usb_setup_t usb_setup;
/***********************************************************************************************************************
 * Global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/
static void init_event_setup(void)
{
    event_setup.noneFlg = 0;
    event_setup.configedFlg = 0;
    event_setup.requestFlg = 0;
    event_setup.requestCompleteFlg = 0;
    event_setup.writeCompleteFlg = 0;
    event_setup.readCompleteFlg = 0;
    event_setup.suspendFlg = 0;
    event_setup.resumeFlg = 0;
    event_setup.detachFlg = 0;

    event_setup.getDescriptorFlg = 0;
    event_setup.readyToWorkFlg = 0;
    event_setup.readyToReadFlg = 0;
}
/*******************************************************************************************************************//**
 * main() is generated by the RA Configuration editor and is used to generate threads if an RTOS is used.  This function
 * is called by main() when no RTOS is used.
 **********************************************************************************************************************/
void usb_phid_main(void)
{
    /* TODO: add your own code here */
    fsp_err_t err = FSP_SUCCESS;

    usb_status_t event;

    err = R_USB_Open(&g_basic0_ctrl, &g_basic0_cfg);
    init_event_setup();

    if (FSP_SUCCESS != err)
    {
        log_fatal(logger,"R_USB_Open failed");
        return;
    }

#if USB_PHID_USEGPT
    R_GPT_Open(&g_timer0_ctrl, &g_timer0_cfg);
    R_GPT_Start(&g_timer0_ctrl);
#endif//#if USB_PHID_USEGPT

#if USB_PHID_USECB
    for(uint8_t i = 0; i < 64; i++)
    {
        send_buf[i] = i;
    }
#endif

    while (true)
    {

#if !USB_PHID_USEGPT

        R_USB_EventGet (&g_basic0_ctrl, &event);
        /* check for usb event */
        switch (event)
        {
            /* Configured event */
            case USB_STATUS_CONFIGURED:
            {
                log_info(logger,"USB Status Configured Successful");
                break;
            }
            /* Receive Class Request */
            case USB_STATUS_REQUEST:
            {
                usb_enumeration();
                log_info(logger,"USB_STATUS_REQUEST");
                break;
            }
            /* Complete Class Request */
            case USB_STATUS_REQUEST_COMPLETE:
            {
                usb_status_update();
                log_info(logger,"USB_STATUS_REQUEST_COMPLETE");
                break;
            }
            /* Write complete event */
            case USB_STATUS_WRITE_COMPLETE:
            {
                log_info(logger,"USB_WRITE_COMPLETE");
                event_setup.writeCompleteFlg = 1;

                break;
            }
            /*Read complete event*/
            case USB_STATUS_READ_COMPLETE:
            {
                log_info(logger,"USB_READ_COMPLETE");
                event_setup.readCompleteFlg = 1;

                break;
            }
            /* Suspend event */
            case USB_STATUS_SUSPEND:
            {
                log_info(logger,"USB STATUS : SUSPEND");
                break;
            }
            case USB_STATUS_RESUME :
            {
                log_info(logger,"USB STATUS : RESUME");
                break;
            }
            /* detach event */
            case USB_STATUS_DETACH:
            {
                log_info(logger,"USB STATUS : DETACH");
                break;
            }
            case USB_STATUS_NONE:
            {

                break;
            }

            default:
            {
                break;
            }
        }

        /*read 64bit data from PC*/
        if(event_setup.writeCompleteFlg)
        {
            event_setup.writeCompleteFlg = 0;

            log_info(logger,"USB_Read");
            err = R_USB_Read(&g_basic0_ctrl, (uint8_t *)testbuf, 64, USB_CLASS_PHID);
            if (FSP_SUCCESS != err)
            {
                log_err(logger,"R_USB_Read failed err=%d", err);
                deinit_usb();

                return;
            }

        }

        /*write 64bit data to PC after receiving data from PC*/
        if(event_setup.readCompleteFlg)
        {
            event_setup.readCompleteFlg = 0;

            log_info(logger,"USB_Write");
            err = R_USB_Write(&g_basic0_ctrl, (uint8_t *)testbuf, 64, USB_CLASS_PHID);
            if (FSP_SUCCESS != err)
            {
                log_err(logger,"R_USB_Write failed err=%d", err);
                deinit_usb();

                return;
            }

        }

#else

#if !USB_PHID_USECB
        if (g_apl_usb_event_wp != g_apl_usb_event_rp)
        {
            event = g_apl_usb_event_buf[g_apl_usb_event_rp].event;
            event_info = g_apl_usb_event_buf[g_apl_usb_event_rp];

            log_info(logger,"wp=%d, rp=%d, event=%d, request_type=0x%x", g_apl_usb_event_wp, g_apl_usb_event_rp,g_apl_usb_event_buf[g_apl_usb_event_rp].event, g_apl_usb_event_buf[g_apl_usb_event_rp].setup.request_type);
            g_apl_usb_event_rp++;
            g_apl_usb_event_rp %= APL_NUM_USB_EVENT;

        }
        else
        {
            event = USB_STATUS_NONE;
        }

        switch (event)
        {
            /* Configured event */
            case USB_STATUS_CONFIGURED:
            {
                log_info(logger,"USB Status Configured Successful");
                break;
            }

            /* Receive Class Request */
            case USB_STATUS_REQUEST:
            {
                usb_enumeration();
                log_info(logger,"USB_STATUS_REQUEST");
                break;
            }
            /* Complete Class Request */
            case USB_STATUS_REQUEST_COMPLETE:
            {
                usb_status_update();
                log_info(logger,"USB_STATUS_REQUEST_COMPLETE");
                event_setup.requestCompleteFlg = 1;
            break;
            }
            /* Write complete event */
            case USB_STATUS_WRITE_COMPLETE:
            {
                event_setup.writeCompleteFlg = 1;
            break;
            }
            /*Read complete event*/
            case USB_STATUS_READ_COMPLETE:
            {
                event_setup.readCompleteFlg = 1;

            break;
            }
            /* Suspend event */
            case USB_STATUS_SUSPEND:
            {
                log_info(logger,"USB STATUS : SUSPEND");
                break;
            }
            case USB_STATUS_RESUME :
            {
                log_info(logger,"USB STATUS : RESUME");
                break;
            }
            /* detach event */
            case USB_STATUS_DETACH:
            {
                log_info(logger,"USB STATUS : DETACH");
                break;
            }
            case USB_STATUS_NONE:
            {

                break;
            }

            default:
            {
                break;
            }
        }
#else

        if(event_setup.configedFlg)
        {
            event_setup.configedFlg = 0;
            log_info(logger,"USB STATUS : USB Status Configured Successful");

        }
        if(event_setup.requestFlg)
        {
            event_setup.requestFlg = 0;
            usb_enumeration();
            log_info(logger,"USB STATUS : USB_STATUS_REQUEST");

        }
        if(event_setup.requestCompleteFlg)
        {
            event_setup.requestCompleteFlg = 0;
            usb_status_update();
            log_info(logger,"USB STATUS : USB_STATUS_REQUEST_COMPLETE");

        }
        if(event_setup.suspendFlg)
        {
            event_setup.suspendFlg = event_setup.readyToWorkFlg = 0;
            log_info(logger,"USB STATUS : SUSPEND");
        }
        if(event_setup.resumeFlg)
        {
            event_setup.resumeFlg = 0;
            log_info(logger,"USB STATUS : RESUME");
        }
        if(event_setup.detachFlg)
        {
            event_setup.detachFlg = event_setup.readyToWorkFlg = event_setup.getDescriptorFlg = 0;
            log_info(logger,"USB STATUS : DETACH");
        }
        if(event_setup.readCompleteFlg)
        {
            event_setup.readCompleteFlg = 0;
            log_info(logger,"USB STATUS : USB_READ_COMPLETE");
            err = R_USB_Read(&g_basic0_ctrl, (uint8_t *) &send_buf, 64, USB_CLASS_PHID);
            if (FSP_SUCCESS != err)
            {
                log_info(logger,"R_USB_Read failed err=%d",err);
            }
        }
//        if(event_setup.writeCompleteFlg)
//        {
//            log_info(logger,"USB STATUS : USB_WRITE_COMPLETE");
//        }

        //if(0)
        if(event_setup.readyToWorkFlg)
        {
            R_BSP_SoftwareDelay (100, BSP_DELAY_UNITS_MILLISECONDS);

#if 1//write api
            err = R_USB_Write(&g_basic0_ctrl, send_buf, 64, USB_CLASS_PHID);
            if (FSP_SUCCESS != err)
            {
                log_info(logger,"R_USB_Write failed err=%d", err);
                event_setup.readyToWorkFlg = 0;

            }
            while(!event_setup.writeCompleteFlg);
            event_setup.writeCompleteFlg = 0;
#endif


#if 0//poll usb read
            if(event_setup.readyToReadFlg)
            {
                err = R_USB_Read(&g_basic0_ctrl, (uint8_t *) &send_buf, 64, USB_CLASS_PHID);
                if (FSP_SUCCESS != err)
                {
                    log_info(logger,"R_USB_Read failed err=%d", err);

                }
                event_setup.readyToReadFlg = 0;
            }

#endif
        }


#endif//#if !USB_PHID_USECB

#endif//#if !USB_PHID_USEGPT

    }

}

/*******************************************************************************************************************//**
 * @brief       usb enumeration
 * @param[IN]   None
 * @retval      None
 **********************************************************************************************************************/
static void usb_enumeration(void)
{
    fsp_err_t err = FSP_SUCCESS;

    //R_USB_SetupGet(&g_basic0_ctrl, &usb_setup);
    //log_info(logger,"request_type=0x%x, 0x%x",g_basic0_ctrl.setup.request_type, usb_setup.request_type);
    //event_info.setup.request_type

    //log_info(logger,"USB_SET_REPORT:setup.request_type=0x%x", event_info.setup.request_type);
    /* check for request type */
    if (USB_SET_REPORT == (event_info.setup.request_type & USB_BREQUEST))
    {
        /* Get the NumLock data */
        err = R_USB_Read(&g_basic0_ctrl, (uint8_t *) &g_numlock, SIZE_NUM, USB_CLASS_PHID);
        if (FSP_SUCCESS != err)
        {
            log_err(logger,"R_USB_Read failed err=%d", err);
            deinit_usb();
            return;
        }
        log_info(logger,"USB_SET_REPORT:setup.request_type=0x%x", event_info.setup.request_type);
    }
    else if (USB_GET_DESCRIPTOR == (event_info.setup.request_type & USB_BREQUEST))
    {
        /* check for request value */
        if (USB_GET_REPORT_DESCRIPTOR == event_info.setup.request_value)
        {
            err = R_USB_PeriControlDataSet (&g_basic0_ctrl,(uint8_t *)g_apl_report, USB_RECEIVE_REPORT_DESCRIPTOR);
            if (FSP_SUCCESS != err)
            {
                log_err(logger,"R_USB_PeriControlDataSet failed err=%d",err);
                deinit_usb();
                return;
            }
            event_setup.getDescriptorFlg = 1;
            log_info(logger,"USB_GET_DESCRIPTOR---USB_GET_REPORT_DESCRIPTOR:setup.request_type=0x%x, setup.request_value=0x%x", event_info.setup.request_type, event_info.setup.request_value);
        }
        else if (USB_GET_HID_DESCRIPTOR == event_info.setup.request_value)
        {
            for (uint8_t hid_des = 0; hid_des < USB_RECEIVE_HID_DESCRIPTOR; hid_des++)
            {
                send_data[hid_des] = g_apl_configuration[CD_LENGTH + hid_des];
            }

            /* Configuration Descriptor address set. */
            err = R_USB_PeriControlDataSet(&g_basic0_ctrl, send_data, USB_RECEIVE_HID_DESCRIPTOR);
            if (FSP_SUCCESS != err)
            {
                log_info(logger,"R_USB_PeriControlDataSet failed err=%d", err);
                deinit_usb();
                return;
            }
            log_info(logger,"USB_GET_DESCRIPTOR---USB_GET_HID_DESCRIPTOR:setup.request_type=0x%x, setup.request_value=0x%x",event_info.setup.request_type, event_info.setup.request_value);

        }
        else
        {
            /* None */
        }
    }
    else if (USB_SET_IDLE == (event_info.setup.request_type & USB_BREQUEST))
    {
        /* Get SetIdle value */
        p_idle_value = (uint8_t *) &event_info.setup.request_value;

        g_idle = p_idle_value[IDLE_VAL_INDEX];

        err = R_USB_PeriControlStatusSet(&g_basic0_ctrl, USB_SETUP_STATUS_ACK);
        if (FSP_SUCCESS != err)
        {
            log_err(logger,"R_USB_PeriControlStatusSet failed err=%d", err);
            deinit_usb();
            return;
        }
        log_info(logger,"USB_SET_IDLE:setup.request_type=0x%x, g_idle=0x%x", event_info.setup.request_type, g_idle);

        if(event_setup.getDescriptorFlg)
        {
            event_setup.readyToWorkFlg = 1;
            event_setup.readCompleteFlg = 1;
        }

    }
    else
    {
        /* None */
    }
}


/*******************************************************************************************************************//**
 * @brief       usb status update
 * @param[IN]   None
 * @retval      None
 **********************************************************************************************************************/
static void usb_status_update(void)
{
    fsp_err_t err = FSP_SUCCESS;

    //R_USB_SetupGet(&g_basic0_ctrl, &usb_setup);
    /* check for request type */
    if (USB_SET_IDLE == (event_info.setup.request_type & USB_BREQUEST))
    {
        p_idle_value = (uint8_t *) &event_info.setup.request_value;

        g_idle = p_idle_value[IDLE_VAL_INDEX];
        log_info(logger,"USB_SET_IDLE:p_idle_value = 0x%x\r\n", p_idle_value);
    }
    else if (USB_SET_PROTOCOL == (event_info.setup.request_type & USB_BREQUEST))
    {
        /* None */
    }
    else
    {
        /* Sending the zero data (8 bytes) */
        err = R_USB_Write(&g_basic0_ctrl, g_buf, 64, USB_CLASS_PHID);
        if (FSP_SUCCESS != err)
        {
            log_err(logger,"R_USB_Write failed err=%d", err);
            deinit_usb();
            return;
        }

        log_info(logger,"usb_status_update: Sending the zero data (8 bytes)");
    }
}


/*******************************************************************************************************************//**
 * @brief       usb write operation
 * @param[IN]   None
 * @retval      None
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief       de-initialize USB
 * @param[IN]   None
 * @retval      None
 **********************************************************************************************************************/
static void deinit_usb(void)
{
    fsp_err_t err = FSP_SUCCESS;
    /* close usb instance */
    err = R_USB_Close(&g_basic0_ctrl);
    if (FSP_SUCCESS != err)
    {
        log_fatal(logger,"R_USB_Close ");
    }
}


/******************************************************************************
 * Function Name   : usb_apl_callback
 * Description     : Function processes Application events (Interrupt IN Request etc).
 * Arguments       : usb_callback_args_t * p_event    : Pointer for USB event information
 * Return value    : none
 ******************************************************************************/
void usb_apl_callback(usb_callback_args_t * p_event)
{
#if !USB_PHID_USECB
    //__enable_irq();
    //__ISB();

    g_apl_usb_event_buf[g_apl_usb_event_wp] = *p_event;
    g_apl_usb_event_wp++;
    g_apl_usb_event_wp %= APL_NUM_USB_EVENT;

    //__disable_irq();
    //__ISB();
#endif//#if !USB_PHID_USECB

#if USB_PHID_USECB
    switch (p_event->event)
    {
        /* Configured event */
        case USB_STATUS_CONFIGURED:
        {
            event_setup.configedFlg = 1;
            break;
        }

        /* Receive Class Request */
        case USB_STATUS_REQUEST:
        {
            event_setup.requestFlg = 1;
            event_info = *p_event;
            break;
        }
        /* Complete Class Request */
        case USB_STATUS_REQUEST_COMPLETE:
        {
            event_setup.requestCompleteFlg = 1;
            event_setup.readyToReadFlg = 1;
            event_info = *p_event;
        break;
        }
        /* Write complete event */
        case USB_STATUS_WRITE_COMPLETE:
        {
            event_setup.writeCompleteFlg = 1;

        break;
        }
        /*Read complete event*/
        case USB_STATUS_READ_COMPLETE:
        {
            event_setup.readCompleteFlg = 1;

        break;
        }
        /* Suspend event */
        case USB_STATUS_SUSPEND:
        {
            event_setup.suspendFlg = 1;
            break;
        }
        case USB_STATUS_RESUME :
        {
            event_setup.suspendFlg = 1;
            break;
        }
        /* detach event */
        case USB_STATUS_DETACH:
        {
            event_setup.detachFlg = 1;
            break;
        }
        case USB_STATUS_NONE:
        {

            break;
        }

        default:
        {
            break;
        }
    }
#endif//#if USB_PHID_USECB
}

/*
 * g_timer0CB
 */
volatile uint32_t cnt = 0;
void g_timer0CB (timer_callback_args_t * p_args)
{
    //__enable_irq();
    //__ISB();

    R_USB_DriverActivate(&g_basic0_ctrl);    //GR_debug

    //__disable_irq();
    //__ISB();

    cnt++;//for test
    if (TIMER_EVENT_CYCLE_END == p_args->event)
    {
        /* Add application code to be called periodically here. */
    }
}




