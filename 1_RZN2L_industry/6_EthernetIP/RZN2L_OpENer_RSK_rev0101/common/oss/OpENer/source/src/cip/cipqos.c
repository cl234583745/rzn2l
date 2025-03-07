/*******************************************************************************
 * Copyright (c) 2009/, Rockwell Automation, Inc.
 * All rights reserved.
 *
 ******************************************************************************/

#include "cipqos.h"

#include "opener_user_conf.h"
#include "cipcommon.h"
#include "cipmessagerouter.h"
#include "ciperror.h"
#include "endianconv.h"
#include "cipethernetlink.h"
#include "opener_api.h"
#include "trace.h"

#define DEFAULT_DSCP_EVENT 59U
#define DEFAULT_DSCP_GENERAL 47U
#define DEFAULT_DSCP_URGENT 55U
#define DEFAULT_DSCP_SCHEDULED 47U
#define DEFAULT_DSCP_HIGH 43U
#define DEFAULT_DSCP_LOW 31U
#define DEFAULT_DSCP_EXPLICIT 27U

/** @brief The QoS object
 *
 *  The global instance of the QoS object
 */
CipQosObject g_qos = {
  .q_frames_enable = false,
  .dscp.event = DEFAULT_DSCP_EVENT,
  .dscp.general = DEFAULT_DSCP_GENERAL,
  .dscp.urgent = DEFAULT_DSCP_URGENT,
  .dscp.scheduled = DEFAULT_DSCP_SCHEDULED,
  .dscp.high = DEFAULT_DSCP_HIGH,
  .dscp.low = DEFAULT_DSCP_LOW,
  .dscp.explicit_msg = DEFAULT_DSCP_EXPLICIT
};

/** @brief Active set of DSCP data inherits its data from the QoS object on boot-up
 *
 *  The QoS DSCP values can be changed from the EIP network but the changes should come
 *  into effect only after a restart. Values are initialized with the default values.
 *  Changes are activated via the Identity Reset function
 */
static CipQosDscpValues s_active_dscp = {
  .event = DEFAULT_DSCP_EVENT,
  .general = DEFAULT_DSCP_GENERAL,
  .urgent = DEFAULT_DSCP_URGENT,
  .scheduled = DEFAULT_DSCP_SCHEDULED,
  .high = DEFAULT_DSCP_HIGH,
  .low = DEFAULT_DSCP_LOW,
  .explicit_msg = DEFAULT_DSCP_EXPLICIT
};

/************** Functions ****************************************/
EipStatus SetAttributeSingleQoS(
  CipInstance *instance,
  CipMessageRouterRequest *message_router_request,
  CipMessageRouterResponse *message_router_response,
  const struct sockaddr *originator_address,
  const int encapsulation_session) {

  CipAttributeStruct *attribute = GetCipAttribute(
    instance, message_router_request->request_path.attribute_number);
  (void) instance;   /*Suppress compiler warning */
  EipUint16 attribute_number =
    message_router_request->request_path.attribute_number;
  uint8_t set_bit_mask = (instance->cip_class->set_bit_mask[CalculateIndex(
                                                              attribute_number)
                          ]);

  if( NULL != attribute &&
      ( set_bit_mask & ( 1 << ( (attribute_number) % 8 ) ) ) ) {
    CipUsint attribute_value_received = GetUsintFromMessage(
      &(message_router_request->data) );

    if( attribute_value_received < 64U ) {
      OPENER_TRACE_INFO(" setAttribute %d\n", attribute_number);

      if(NULL != attribute->data) {
        /* Call the PreSetCallback if enabled. */
        if ( (attribute->attribute_flags & kPreSetFunc)
             && NULL != instance->cip_class->PreSetCallback ) {
          instance->cip_class->PreSetCallback(instance, attribute,
                                              message_router_request->service);
        }

        CipUsint *data = (CipUsint *) attribute->data;
        *(data) = attribute_value_received;

        /* Call the PostSetCallback if enabled. */
        if ( (attribute->attribute_flags & (kPostSetFunc | kNvDataFunc) )
             && NULL != instance->cip_class->PostSetCallback ) {
          instance->cip_class->PostSetCallback(instance, attribute,
                                               message_router_request->service);
        }

        message_router_response->general_status = kCipErrorSuccess;
      } else {
        message_router_response->general_status = kCipErrorNotEnoughData;
        OPENER_TRACE_INFO("CIP QoS not enough data\n");
      }
    } else {
      message_router_response->general_status = kCipErrorInvalidAttributeValue;
    }
  } else {
    /* we don't have this attribute */
    message_router_response->general_status = kCipErrorAttributeNotSupported;
  }

  message_router_response->size_of_additional_status = 0;
  InitializeENIPMessage(&message_router_response->message);
  message_router_response->reply_service = (0x80
                                            | message_router_request->service);

  return kEipStatusOkSend;
}

CipUsint CipQosGetDscpPriority(ConnectionObjectPriority priority) {

  CipUsint priority_value;
  switch (priority) {
    case kConnectionObjectPriorityLow:
      priority_value = s_active_dscp.low;
      break;
    case kConnectionObjectPriorityHigh:
      priority_value = s_active_dscp.high;
      break;
    case kConnectionObjectPriorityScheduled:
      priority_value = s_active_dscp.scheduled;
      break;
    case kConnectionObjectPriorityUrgent:
      priority_value = s_active_dscp.urgent;
      break;
    case kConnectionObjectPriorityExplicit: /* fall through */
    default:
      priority_value = s_active_dscp.explicit_msg;
      break;
  }
  return priority_value;
}

EipStatus CipQoSInit() {

  CipClass *qos_class = NULL;

  if( ( qos_class = CreateCipClass(kCipQoSClassCode,
                                   7, /* # class attributes */
                                   7, /* # highest class attribute number */
                                   2, /* # class services */
                                   8, /* # instance attributes */
                                   8, /* # highest instance attribute number */
                                   2, /* # instance services */
                                   1, /* # instances */
                                   "Quality of Service",
                                   1, /* # class revision */
                                   NULL /* # function pointer for initialization */
                                   ) ) == 0 ) {

    return kEipStatusError;
  }

  CipInstance *instance = GetCipInstance(qos_class, 1); /* bind attributes to the instance #1 that was created above */

  InsertAttribute(instance,
                  1,
                  kCipUsint,
                  EncodeCipUsint,
                  (void *) &g_qos.q_frames_enable,
                  kNotSetOrGetable);
  InsertAttribute(instance,
                  2,
                  kCipUsint,
                  EncodeCipUsint,
                  (void *) &g_qos.dscp.event,
                  kNotSetOrGetable);
  InsertAttribute(instance,
                  3,
                  kCipUsint,
                  EncodeCipUsint,
                  (void *) &g_qos.dscp.general,
                  kNotSetOrGetable);
  InsertAttribute(instance,
                  4,
                  kCipUsint,
                  EncodeCipUsint,
                  (void *) &g_qos.dscp.urgent,
                  kGetableSingle | kSetable | kNvDataFunc);
  InsertAttribute(instance,
                  5,
                  kCipUsint,
                  EncodeCipUsint,
                  (void *) &g_qos.dscp.scheduled,
                  kGetableSingle | kSetable | kNvDataFunc);
  InsertAttribute(instance,
                  6,
                  kCipUsint,
                  EncodeCipUsint,
                  (void *) &g_qos.dscp.high,
                  kGetableSingle | kSetable | kNvDataFunc);
  InsertAttribute(instance,
                  7,
                  kCipUsint,
                  EncodeCipUsint,
                  (void *) &g_qos.dscp.low,
                  kGetableSingle | kSetable | kNvDataFunc);
  InsertAttribute(instance,
                  8,
                  kCipUsint,
                  EncodeCipUsint,
                  (void *) &g_qos.dscp.explicit_msg,
                  kGetableSingle | kSetable | kNvDataFunc);

  InsertService(qos_class, kGetAttributeSingle, &GetAttributeSingle,
                "GetAttributeSingle");
  InsertService(qos_class, kSetAttributeSingle, &SetAttributeSingleQoS,
                "SetAttributeSingleQoS");

  return kEipStatusOk;
}

void CipQosUpdateUsedSetQosValues(void) {
  s_active_dscp = g_qos.dscp;
}

void CipQosResetAttributesToDefaultValues(void) {
  static const CipQosDscpValues kDefaultValues = {
    .event = DEFAULT_DSCP_EVENT,
    .general = DEFAULT_DSCP_GENERAL,
    .urgent = DEFAULT_DSCP_URGENT,
    .scheduled = DEFAULT_DSCP_SCHEDULED,
    .high = DEFAULT_DSCP_HIGH,
    .low = DEFAULT_DSCP_LOW,
    .explicit_msg = DEFAULT_DSCP_EXPLICIT
  };
  g_qos.q_frames_enable = false;
  g_qos.dscp = kDefaultValues;
}
