/*******************************************************************************
 * Copyright (c) 2009, Rockwell Automation, Inc.
 * All rights reserved.
 *
 ******************************************************************************/

/**
 * @file cipidentity.c
 *
 * CIP Identity Object
 * ===================
 *
 * Implemented Attributes
 * ----------------------
 * - Attribute 1: VendorID
 * - Attribute 2: Device Type
 * - Attribute 3: Product Code
 * - Attribute 4: Revision
 * - Attribute 5: Status
 * - Attribute 6: Serial Number
 * - Attribute 7: Product Name
 * - Attribute 8: State
 *
 * Implemented Services
 * --------------------
 */

#include "cipidentity.h"

#include <string.h>

#include "opener_user_conf.h"
#include "cipcommon.h"
#include "cipmessagerouter.h"
#include "ciperror.h"
#include "endianconv.h"
#include "opener_api.h"
#include "trace.h"

/** @brief The device's configuration data for the Identity Object */
#include "devicedata.h"

/** @brief Definition of the global Identity Object */
CipIdentityObject g_identity =
{
  .vendor_id = OPENER_DEVICE_VENDOR_ID, /* Attribute 1: Vendor ID */
  .device_type = OPENER_DEVICE_TYPE, /* Attribute 2: Device Type */
  .product_code = OPENER_DEVICE_PRODUCT_CODE, /* Attribute 3: Product Code */
  .revision = { /* Attribute 4: Revision / CipUsint Major, CipUsint Minor */
    OPENER_DEVICE_MAJOR_REVISION,
    OPENER_DEVICE_MINOR_REVISION
  },
  .status = 0, /* Attribute 5: Status */
  .ext_status = kSelftestingUnknown,  /* Attribute 5: Extended Device Status field */
  .serial_number = 0, /* Attribute 6: Serial Number */
  .product_name = { /* Attribute 7: Product Name */
    sizeof(OPENER_DEVICE_NAME) - 1,
    (EipByte *)OPENER_DEVICE_NAME
  },
  .state = 255,
};


/* The Doxygen comment is with the function's prototype in opener_api.h. */
void SetDeviceSerialNumber(const EipUint32 serial_number) {
  g_identity.serial_number = serial_number;
}

/* The Doxygen comment is with the function's prototype in opener_api.h. */
void SetDeviceStatus(const CipWord status) {
  g_identity.status = status;
  g_identity.ext_status = status & kExtStatusMask;
}

static inline void MergeStatusAndExtStatus(void) {
  CipWord status_flags = g_identity.status & (~kExtStatusMask);
  CipWord ext_status = g_identity.ext_status & kExtStatusMask;

  /* Any major fault will override the current extended status with kMajorFault.
     See comment on Major Fault at Vol. 1, Table 5A-2.4. */
  if(0 !=
     (status_flags & (kMajorRecoverableFault | kMajorUnrecoverableFault) ) ) {
    ext_status = kMajorFault;
  }
  g_identity.status = status_flags | ext_status;
}

/** @brief Set status flags of the device's Status word
 *
 * @param status_flags  flags to set in the Status word
 *
 *  This function sets status flags of the device's Status word and combines
 *  the flag values with the internal ext_status member into a new Status
 *  value.
 */
void CipIdentitySetStatusFlags(const CipWord status_flags) {
  g_identity.status |= status_flags & (~kExtStatusMask);
  MergeStatusAndExtStatus();
}

/** @brief Clear status flags of the device's Status word
 *
 * @param status_flags  flags to clear in the Status word
 *
 *  This function clears status flags of the device's Status word and combines
 *  the flag values with the internal ext_status member into a new Status
 *  value.
 */
void CipIdentityClearStatusFlags(const CipWord status_flags) {
  g_identity.status &= ~(status_flags & (~kExtStatusMask) );
  MergeStatusAndExtStatus();
}

/** @brief Set the device's Extended Device Status field in the Status word
 *
 * @param   extended_status Extended Device Status field
 *
 *  This function sets the internal ext_status member of the Identity object
 *  and combines its value depending on the other Status flags into a new
 *  Status value.
 */
void CipIdentitySetExtendedDeviceStatus(
  CipIdentityExtendedStatus extended_status) {
  OPENER_TRACE_INFO("Setting extended status: %x\n", extended_status);
  g_identity.ext_status = extended_status & kExtStatusMask;
  MergeStatusAndExtStatus();
}

/** @brief Reset service
 *
 * @param instance
 * @param message_router_request
 * @param message_router_response
 * @returns Currently always kEipOkSend is returned
 */
static EipStatus Reset(CipInstance *instance,
/* pointer to instance*/
                       CipMessageRouterRequest *message_router_request,
                       /* pointer to message router request*/
                       CipMessageRouterResponse *message_router_response,  /* pointer to message router response*/
                       const struct sockaddr *originator_address,
                       const int encapsulation_session) {
  (void) instance;

  EipStatus eip_status = kEipStatusOkSend;

  message_router_response->reply_service = (0x80
                                            | message_router_request->service);
  message_router_response->size_of_additional_status = 0;
  message_router_response->general_status = kCipErrorSuccess;

  if (message_router_request->request_path_size > 1) {
    message_router_response->general_status = kCipErrorTooMuchData;
  }
  else {
    CipOctet reset_type = 0;  /* The default type if type parameter was omitted. */
    if (message_router_request->request_path_size == 1) {
      reset_type = message_router_request->data[0];
    }
    switch (reset_type) {
      case 0: /* Reset type 0 -> Emulate power cycle */
        if ( kEipStatusError == ResetDevice() ) {
          message_router_response->general_status = kCipErrorInvalidParameter;
        }
        break;

      case 1: /* Reset type 1 -> Return to factory defaults & power cycle*/
        if ( kEipStatusError == ResetDeviceToInitialConfiguration() ) {
          message_router_response->general_status = kCipErrorInvalidParameter;
        }
        break;

      /* case 2: Not supported Reset type 2 ->
         Return to factory defaults except communications parameters & power cycle*/

      default:
        message_router_response->general_status = kCipErrorInvalidParameter;
        break;
    }
  }

  InitializeENIPMessage(&message_router_response->message);
  return eip_status;
}

void InitializeCipIdentity(CipClass *class) {


  CipClass *meta_class = class->class_instance.cip_class;

  InsertAttribute( (CipInstance *) class, 1, kCipUint,
                   EncodeCipUint,
                   (void *) &class->revision,
                   kGetableSingleAndAll );                 /* revision */
  InsertAttribute( (CipInstance *) class, 2, kCipUint,
                   EncodeCipUint,
                   (void *) &class->number_of_instances, kGetableSingleAndAll ); /*  largest instance number */
  InsertAttribute( (CipInstance *) class, 3, kCipUint,
                   EncodeCipUint,
                   (void *) &class->number_of_instances, kGetableSingle ); /* number of instances currently existing*/
  InsertAttribute( (CipInstance *) class, 4, kCipUint, EncodeCipUint,
                   (void *) &kCipUintZero,
                   kNotSetOrGetable ); /* optional attribute list - default = 0 */
  InsertAttribute( (CipInstance *) class, 5, kCipUint, EncodeCipUint,
                   (void *) &kCipUintZero,
                   kNotSetOrGetable ); /* optional service list - default = 0 */
  InsertAttribute( (CipInstance *) class, 6, kCipUint, EncodeCipUint,
                   (void *) &meta_class->highest_attribute_number,
                   kGetableSingleAndAll );                 /* max class attribute number*/
  InsertAttribute( (CipInstance *) class, 7, kCipUint, EncodeCipUint,
                   (void *) &class->highest_attribute_number,
                   kGetableSingleAndAll );                 /* max instance attribute number*/

  InsertService(meta_class, kGetAttributeAll, &GetAttributeAll,
                "GetAttributeAll");                         /* bind instance services to the metaclass*/
  InsertService(meta_class, kGetAttributeSingle, &GetAttributeSingle,
                "GetAttributeSingle");

}

void EncodeRevision(const void *const data,
                    ENIPMessage *const outgoing_message) {
  CipRevision *revision = (CipRevision *) data;
  AddSintToMessage(revision->major_revision, outgoing_message);
  AddSintToMessage(revision->minor_revision, outgoing_message);
}

EipStatus CipIdentityInit() {

  CipClass *class = CreateCipClass(kCipIdentityClassCode,
                                   0, /* # of non-default class attributes */
                                   7, /* # highest class attribute number*/
                                   2, /* # of class services*/
                                   7, /* # of instance attributes*/
                                   7, /* # highest instance attribute number*/
                                   3, /* # of instance services*/
                                   1, /* # of instances*/
                                   "identity", /* # class name (for debug)*/
                                   1, /* # class revision*/
                                   &InitializeCipIdentity); /* # function pointer for initialization*/

  if (class == 0) {
    return kEipStatusError;
  }

  CipInstance *instance = GetCipInstance(class, 1);
  InsertAttribute(instance,
                  1,
                  kCipUint,
                  EncodeCipUint,
                  &g_identity.vendor_id,
                  kGetableSingleAndAll);
  InsertAttribute(instance,
                  2,
                  kCipUint,
                  EncodeCipUint,
                  &g_identity.device_type,
                  kGetableSingleAndAll);
  InsertAttribute(instance,
                  3,
                  kCipUint,
                  EncodeCipUint,
                  &g_identity.product_code,
                  kGetableSingleAndAll);
  InsertAttribute(instance,
                  4,
                  kCipUsintUsint,
                  EncodeRevision,
                  &g_identity.revision,
                  kGetableSingleAndAll);
  InsertAttribute(instance,
                  5,
                  kCipWord,
                  EncodeCipWord,
                  &g_identity.status,
                  kGetableSingleAndAll);
  InsertAttribute(instance,
                  6,
                  kCipUdint,
                  EncodeCipUdint,
                  &g_identity.serial_number,
                  kGetableSingleAndAll);
  InsertAttribute(instance,
                  7,
                  kCipShortString,
                  EncodeCipShortString,
                  &g_identity.product_name,
                  kGetableSingleAndAll);

  InsertService(class, kGetAttributeSingle, &GetAttributeSingle,
                "GetAttributeSingle");
  InsertService(class, kGetAttributeAll, &GetAttributeAll, "GetAttributeAll");
  InsertService(class, kReset, &Reset, "Reset");

  return kEipStatusOk;
}
