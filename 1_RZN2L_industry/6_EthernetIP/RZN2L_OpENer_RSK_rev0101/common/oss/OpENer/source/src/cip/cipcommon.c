/*******************************************************************************
 * Copyright (c) 2009, Rockwell Automation, Inc.
 * All rights reserved.
 *
 ******************************************************************************/
#include <string.h>
#include <stdio.h>

#include "cipcommon.h"

#include "opener_user_conf.h"
#include "opener_api.h"
#include "cipidentity.h"
#include "ciptcpipinterface.h"
#include "cipethernetlink.h"
#include "cipconnectionmanager.h"
#include "endianconv.h"
#include "encap.h"
#include "ciperror.h"
#include "cipassembly.h"
#include "cipmessagerouter.h"
#if defined(OPENER_IS_DLR_DEVICE) && 0 != OPENER_IS_DLR_DEVICE
  #include "cipdlr.h"
#endif
#include "cipqos.h"
#include "cpf.h"
#include "trace.h"
#include "appcontype.h"
#include "cipepath.h"
#include "stdlib.h"

/* private functions*/
void EncodeEPath(CipEpath *epath,
                 ENIPMessage *message);

EipStatus CipStackInit(const EipUint16 unique_connection_id) {
  /* The message router is the first CIP object be initialized!!! */
  EipStatus eip_status = CipMessageRouterInit();
  OPENER_ASSERT(kEipStatusOk == eip_status);
  eip_status = CipIdentityInit();
  OPENER_ASSERT(kEipStatusOk == eip_status);
  eip_status = CipTcpIpInterfaceInit();
  OPENER_ASSERT(kEipStatusOk == eip_status);
  eip_status = CipEthernetLinkInit();
  OPENER_ASSERT(kEipStatusOk == eip_status);
  eip_status = ConnectionManagerInit(unique_connection_id);
  OPENER_ASSERT(kEipStatusOk == eip_status);
  eip_status = CipAssemblyInitialize();
  OPENER_ASSERT(kEipStatusOk == eip_status);
#if defined(OPENER_IS_DLR_DEVICE) && 0 != OPENER_IS_DLR_DEVICE
  eip_status = CipDlrInit();
  OPENER_ASSERT(kEipStatusOk == eip_status);
#endif
  eip_status = CipQoSInit();
  OPENER_ASSERT(kEipStatusOk == eip_status);
  /* the application has to be initialized at last */
  eip_status = ApplicationInitialization();
  OPENER_ASSERT(kEipStatusOk == eip_status);

  return eip_status;
}

void ShutdownCipStack(void) {
  /* First close all connections */
  CloseAllConnections();
  /* Than free the sockets of currently active encapsulation sessions */
  EncapsulationShutDown();
  /*clean the data needed for the assembly object's attribute 3*/
  ShutdownAssemblies();

  ShutdownTcpIpInterface();

  /*no clear all the instances and classes */
  DeleteAllClasses();
}

EipStatus NotifyClass(const CipClass *RESTRICT const cip_class,
                      CipMessageRouterRequest *const message_router_request,
                      CipMessageRouterResponse *const message_router_response,
                      const struct sockaddr *originator_address,
                      const int encapsulation_session) {

  /* find the instance: if instNr==0, the class is addressed, else find the instance */
  EipUint16 instance_number =
    message_router_request->request_path.instance_number;                           /* get the instance number */
  CipInstance *instance = GetCipInstance(cip_class, instance_number); /* look up the instance (note that if inst==0 this will be the class itself) */
  if(instance) /* if instance is found */
  {
    OPENER_TRACE_INFO("notify: found instance %d%s\n",
                      instance_number,
                      instance_number == 0 ? " (class object)" : "");

    CipServiceStruct *service = instance->cip_class->services; /* get pointer to array of services */
    if(NULL != service) /* if services are defined */
    {
      for(size_t i = 0; i < instance->cip_class->number_of_services; i++) /* seach the services list */
      {
        if(message_router_request->service == service->service_number) /* if match is found */
        {
          /* call the service, and return what it returns */
          OPENER_TRACE_INFO("notify: calling %s service\n", service->name);
          OPENER_ASSERT(NULL != service->service_function);
          return service->service_function(instance,
                                           message_router_request,
                                           message_router_response,
                                           originator_address,
                                           encapsulation_session);
        } else {
          service++;
        }
      }
    } OPENER_TRACE_WARN(
      "notify: service 0x%x not supported\n", message_router_request->service);
    message_router_response->general_status = kCipErrorServiceNotSupported; /* if no services or service not found, return an error reply*/
  } else {
    OPENER_TRACE_WARN("notify: instance number %d unknown\n", instance_number);
    /* if instance not found, return an error reply */
    message_router_response->general_status = kCipErrorPathDestinationUnknown;
    /* according to the test tool this is the correct error flag instead of CIP_ERROR_OBJECT_DOES_NOT_EXIST */
  }

  /* handle error replies*/
  message_router_response->size_of_additional_status = 0; /* fill in the rest of the reply with not much of anything*/
  InitializeENIPMessage(&message_router_response->message);
  message_router_response->reply_service =
    (0x80 | message_router_request->service);                                        /* except the reply code is an echo of the command + the reply flag */

  return kEipStatusOkSend;
}

CipInstance *AddCipInstances(CipClass *RESTRICT const cip_class,
                             const int number_of_instances) {
  CipInstance **next_instance = NULL;
  CipInstance *first_instance = NULL; /* Initialize to error result */
  EipUint32 instance_number = 1; /* the first instance is number 1 */
  int new_instances = 0;

  OPENER_TRACE_INFO("adding %d instances to class %s\n",
                    number_of_instances,
                    cip_class->class_name);

  next_instance = &cip_class->instances; /* get address of pointer to head of chain */
  while(*next_instance) /* as long as what pp points to is not zero */
  {
    next_instance = &(*next_instance)->next; /* follow the chain until pp points to pointer that contains a zero */
    instance_number++; /* keep track of what the first new instance number will be */
  }

  /* Allocate and initialize all needed instances one by one. */
  for(new_instances = 0; new_instances < number_of_instances; new_instances++) {
    CipInstance *current_instance =
      (CipInstance *) CipCalloc(1, sizeof(CipInstance) );
    OPENER_ASSERT(NULL != current_instance); /* fail if run out of memory */
    if(NULL == current_instance) {
      break;
    }
    if(NULL == first_instance) {
      first_instance = current_instance; /* remember the first allocated instance */
    }

    current_instance->instance_number = instance_number; /* assign the next sequential instance number */
    current_instance->cip_class = cip_class; /* point each instance to its class */

    if(cip_class->number_of_attributes) /* if the class calls for instance attributes */
    { /* then allocate storage for the attribute array */
      current_instance->attributes = (CipAttributeStruct *) CipCalloc(
        cip_class->number_of_attributes,
        sizeof(CipAttributeStruct) );
      OPENER_ASSERT(NULL != current_instance->attributes);/* fail if run out of memory */
      if(NULL == current_instance->attributes) {
        break;
      }
    }

    *next_instance = current_instance; /* link the previous pointer to this new node */
    next_instance = &current_instance->next; /* update pp to point to the next link of the current node */
    cip_class->number_of_instances += 1; /* update the total number of instances recorded by the class */
    instance_number++; /* update to the number of the next node*/
  }

  if(new_instances != number_of_instances) {
    /* TODO: Free again all attributes and instances allocated so far in this call. */
    OPENER_TRACE_ERR(
      "ERROR: Allocated only %d instances of requested %d for class %s\n",
      new_instances,
      number_of_instances,
      cip_class->class_name);
    first_instance = NULL; /* failed to allocate all instances / attributes */
  }
  return first_instance;
}

CipInstance *AddCipInstance(CipClass *RESTRICT const cip_class,
                            const EipUint32 instance_id) {
  CipInstance *instance = GetCipInstance(cip_class, instance_id);

  if(NULL == instance) { /*we have no instance with given id*/
    instance = AddCipInstances(cip_class, 1);
    instance->instance_number = instance_id;
  }
  return instance;
}

CipClass *CreateCipClass(const CipUdint class_code,
                         const int number_of_class_attributes,
                         const EipUint32 highest_class_attribute_number,
                         const int number_of_class_services,
                         const int number_of_instance_attributes,
                         const EipUint32 highest_instance_attribute_number,
                         const int number_of_instance_services,
                         const int number_of_instances,
                         const char *const name,
                         const EipUint16 revision,
                         InitializeCipClass initializer) {

  OPENER_TRACE_INFO("creating class '%s' with code: 0x%" PRIX32 "\n", name,
                    class_code);

  OPENER_ASSERT(NULL == GetCipClass(class_code) ); /* check if an class with the ClassID already exists */
  /* should never try to redefine a class*/

  /* a metaClass is a class that holds the class attributes and services
     CIP can talk to an instance, therefore an instance has a pointer to its class
     CIP can talk to a class, therefore a class struct is a subclass of the instance struct,
     and contains a pointer to a metaclass
     CIP never explicitly addresses a metaclass*/

  CipClass *const cip_class = (CipClass *) CipCalloc(1, sizeof(CipClass) ); /* create the class object*/
  CipClass *const meta_class = (CipClass *) CipCalloc(1, sizeof(CipClass) ); /* create the metaclass object*/

  /* initialize the class-specific fields of the Class struct*/
  cip_class->class_code = class_code; /* the class remembers the class ID */
  cip_class->revision = revision; /* the class remembers the class ID */
  cip_class->number_of_instances = 0; /* the number of instances initially zero (more created below) */
  cip_class->instances = 0;
  cip_class->number_of_attributes = number_of_instance_attributes; /* the class remembers the number of instances of that class */
  cip_class->highest_attribute_number = highest_instance_attribute_number; /* indicate which attributes are included in instance getAttributeAll */
  cip_class->number_of_services = number_of_instance_services; /* the class manages the behavior of the instances */
  cip_class->services = 0;
  cip_class->class_name = name; /* initialize the class-specific fields of the metaClass struct */
  meta_class->class_code = 0xffffffff; /* set metaclass ID (this should never be referenced) */
  meta_class->number_of_instances = 1; /* the class object is the only instance of the metaclass */
  meta_class->instances = (CipInstance *) cip_class;
  meta_class->number_of_attributes = number_of_class_attributes + 7; /* the metaclass remembers how many class attributes exist*/
  meta_class->highest_attribute_number = highest_class_attribute_number; /* indicate which attributes are included in class getAttributeAll*/
  meta_class->number_of_services = number_of_class_services; /* the metaclass manages the behavior of the class itself */
  meta_class->class_name = (char *) CipCalloc(1, strlen(name) + 6); /* fabricate the name "meta<classname>"*/
  snprintf(meta_class->class_name, strlen(name) + 6, "meta-%s", name);

  /* initialize the instance-specific fields of the Class struct*/
  cip_class->class_instance.instance_number = 0; /* the class object is instance zero of the class it describes (weird, but that's the spec)*/
  cip_class->class_instance.attributes = 0; /* this will later point to the class attibutes*/
  cip_class->class_instance.cip_class = meta_class; /* the class's class is the metaclass (like SmallTalk)*/
  cip_class->class_instance.next = 0; /* the next link will always be zero, since there is only one instance of any particular class object */

  meta_class->class_instance.instance_number = 0xffffffff; /*the metaclass object does not really have a valid instance number*/
  meta_class->class_instance.attributes = NULL;/* the metaclass has no attributes*/
  meta_class->class_instance.cip_class = NULL; /* the metaclass has no class*/
  meta_class->class_instance.next = NULL; /* the next link will always be zero, since there is only one instance of any particular metaclass object*/

  /* further initialization of the class object*/

  cip_class->class_instance.attributes = (CipAttributeStruct *) CipCalloc(
    meta_class->number_of_attributes,
    sizeof(CipAttributeStruct) );
  /* TODO -- check that we didn't run out of memory?*/

  meta_class->services = (CipServiceStruct *) CipCalloc(
    meta_class->number_of_services,
    sizeof(CipServiceStruct) );

  cip_class->services = (CipServiceStruct *) CipCalloc(
    cip_class->number_of_services,
    sizeof(CipServiceStruct) );

  if(number_of_instances > 0) {
    AddCipInstances(cip_class, number_of_instances); /*TODO handle return value and clean up if necessary*/
  }

  if(RegisterCipClass(cip_class) == kEipStatusError) {/* no memory to register class in Message Router */
    return 0; /*TODO handle return value and clean up if necessary*/
  }

  AllocateAttributeMasks(meta_class); /* Allocation of bitmasks for Class Attributes */
  AllocateAttributeMasks(cip_class); /* Allocation of bitmasks for Instance Attributes */

  if(NULL == initializer) {
    InsertAttribute( (CipInstance *) cip_class, 1, kCipUint, EncodeCipUint,
                     (void *) &cip_class->revision, kGetableSingleAndAll );                                                    /* revision */
    InsertAttribute( (CipInstance *) cip_class, 2, kCipUint, EncodeCipUint,
                     (void *) &cip_class->number_of_instances,
                     kGetableSingleAndAll );                                                                                              /* #2 Max instance no. */
    InsertAttribute( (CipInstance *) cip_class, 3, kCipUint, EncodeCipUint,
                     (void *) &cip_class->number_of_instances,
                     kGetableSingleAndAll );                                                                                              /* number of instances currently existing*/
    InsertAttribute( (CipInstance *) cip_class, 4, kCipUint, EncodeCipUint,
                     (void *) &kCipUintZero, kGetableAllDummy );                                                    /* optional attribute list - default = 0 */
    InsertAttribute( (CipInstance *) cip_class, 5, kCipUint, EncodeCipUint,
                     (void *) &kCipUintZero, kNotSetOrGetable );                                                    /* optional service list - default = 0 */
    InsertAttribute( (CipInstance *) cip_class, 6, kCipUint, EncodeCipUint,
                     (void *) &meta_class->highest_attribute_number,
                     kGetableSingle );                                                                                                    /* max class attribute number*/
    InsertAttribute( (CipInstance *) cip_class, 7, kCipUint, EncodeCipUint,
                     (void *) &cip_class->highest_attribute_number,
                     kGetableSingle );                                                                                                   /* max instance attribute number*/

    if(number_of_class_services > 0) {
      if(number_of_class_services > 1) { /*only if the mask has values add the get_attribute_all service */
        InsertService(meta_class,
                      kGetAttributeAll,
                      &GetAttributeAll,
                      "GetAttributeAll");                                                 /* bind instance services to the metaclass*/
      }
      InsertService(meta_class,
                    kGetAttributeSingle,
                    &GetAttributeSingle,
                    "GetAttributeSingle");
    }
  } else {
    initializer(cip_class);
  }

  /* create the standard class services*/
  return cip_class;
}

void InsertAttribute(CipInstance *const instance,
                     const EipUint16 attribute_number,
                     const EipUint8 cip_type,
                     CipAttributeEncodeInMessage encode_function,
                     void *const data,
                     const EipByte cip_flags) {

  OPENER_ASSERT(NULL != data); /* Its not allowed to push a NULL pointer, as this marks an unused attribute struct */

  CipAttributeStruct *attribute = instance->attributes;
  CipClass *cip_class = instance->cip_class;

  OPENER_ASSERT(NULL != attribute);
  /* adding a attribute to a class that was not declared to have any attributes is not allowed */
  for(int i = 0; i < instance->cip_class->number_of_attributes; i++) {
    if(attribute->data == NULL) { /* found non set attribute */
      attribute->attribute_number = attribute_number;
      attribute->type = cip_type;
      attribute->encode = encode_function;
      attribute->attribute_flags = cip_flags;
      attribute->data = data;

      OPENER_ASSERT(attribute_number <= cip_class->highest_attribute_number);

      size_t index = CalculateIndex(attribute_number);

      cip_class->get_single_bit_mask[index] |=
        (cip_flags & kGetableSingle) ? 1 << (attribute_number) % 8 : 0;
      cip_class->get_all_bit_mask[index] |=
        (cip_flags & (kGetableAll | kGetableAllDummy) ) ? 1 <<
        (attribute_number) % 8 : 0;
      cip_class->set_bit_mask[index] |= ( (cip_flags & kSetable) ? 1 : 0 ) <<
                                        ( (attribute_number) % 8 );

      return;
    }
    attribute++;
  } OPENER_TRACE_ERR(
    "Tried to insert too many attributes into class: %" PRIu32 " '%s', instance %" PRIu32 "\n",
    cip_class->class_code,
    cip_class->class_name,
    instance->instance_number);
  OPENER_ASSERT(false);
  /* trying to insert too many attributes*/
}

void InsertService(const CipClass *const cip_class,
                   const EipUint8 service_number,
                   const CipServiceFunction service_function,
                   char *const service_name) {

  CipServiceStruct *service = cip_class->services; /* get a pointer to the service array*/
  OPENER_TRACE_INFO("%s, number of services:%d, service number:%d\n",
                    cip_class->class_name, cip_class->number_of_services,
                    service_number);
  OPENER_ASSERT(service != NULL);
  /* adding a service to a class that was not declared to have services is not allowed*/
  for(int i = 0; i < cip_class->number_of_services; i++) /* Iterate over all service slots attached to the class */
  {
    if(service->service_number == service_number ||
       service->service_function == NULL)                                              /* found undefined service slot*/
    {
      service->service_number = service_number; /* fill in service number*/
      service->service_function = service_function; /* fill in function address*/
      service->name = service_name;
      return;
    }
    ++service;
  }
  OPENER_ASSERT(false);
  /* adding more services than were declared is a no-no*/
}

void InsertGetSetCallback(CipClass *const cip_class,
                          CipGetSetCallback callback_function,
                          CIPAttributeFlag callbacks_to_install) {
  if(0 != (kPreGetFunc & callbacks_to_install) ) {
    cip_class->PreGetCallback = callback_function;
  }
  if(0 != (kPostGetFunc & callbacks_to_install) ) {
    cip_class->PostGetCallback = callback_function;
  }
  if(0 != (kPreSetFunc & callbacks_to_install) ) {
    cip_class->PreSetCallback = callback_function;
  }
  /* The PostSetCallback is used for both, the after set action and the storage
   * of non volatile data. Therefore check for both flags set. */
  if(0 != ( (kPostSetFunc | kNvDataFunc) & callbacks_to_install ) ) {
    cip_class->PostSetCallback = callback_function;
  }
}

CipAttributeStruct *GetCipAttribute(const CipInstance *const instance,
                                    const EipUint16 attribute_number) {

  CipAttributeStruct *attribute = instance->attributes; /* init pointer to array of attributes*/
  for(int i = 0; i < instance->cip_class->number_of_attributes; i++) {
    if(attribute_number == attribute->attribute_number) {
      return attribute;
    } else {
      ++attribute;
    }
  }

  OPENER_TRACE_WARN("attribute %d not defined\n", attribute_number);

  return NULL;
}

void GenerateGetAttributeSingleHeader(
  const CipMessageRouterRequest *const message_router_request,
  CipMessageRouterResponse *const message_router_response) {
  InitializeENIPMessage(&message_router_response->message);
  message_router_response->reply_service =
    (0x80 | message_router_request->service);
  message_router_response->general_status = kCipErrorAttributeNotSupported;
  message_router_response->size_of_additional_status = 0;
}

/* TODO this needs to check for buffer overflow*/
EipStatus GetAttributeSingle(CipInstance *RESTRICT const instance,
                             CipMessageRouterRequest *const message_router_request,
                             CipMessageRouterResponse *const message_router_response,
                             const struct sockaddr *originator_address,
                             const int encapsulation_session) {
  /* Mask for filtering get-ability */

  CipAttributeStruct *attribute = GetCipAttribute(instance,
                                                  message_router_request->request_path.attribute_number);

  GenerateGetAttributeSingleHeader(message_router_request,
                                   message_router_response);

  EipUint16 attribute_number =
    message_router_request->request_path.attribute_number;

  if( (NULL != attribute) && (NULL != attribute->data) ) {
    uint8_t get_bit_mask =
      (instance->cip_class->get_single_bit_mask[CalculateIndex(attribute_number)
       ]);
    if(0 != (get_bit_mask & (1 << (attribute_number % 8) ) ) ) {
      OPENER_TRACE_INFO("getAttribute %d\n",
                        message_router_request->request_path.attribute_number); /* create a reply message containing the data*/

      /* Call the PreGetCallback if enabled for this attribute and the class provides one. */
      if( (attribute->attribute_flags & kPreGetFunc) &&
          NULL != instance->cip_class->PreGetCallback ) {
        instance->cip_class->PreGetCallback(instance,
                                            attribute,
                                            message_router_request->service);
      }

      OPENER_ASSERT(NULL != attribute);
      attribute->encode(attribute->data, &message_router_response->message);
      message_router_response->general_status = kCipErrorSuccess;

      /* Call the PostGetCallback if enabled for this attribute and the class provides one. */
      if( (attribute->attribute_flags & kPostGetFunc) &&
          NULL != instance->cip_class->PostGetCallback ) {
        instance->cip_class->PostGetCallback(instance,
                                             attribute,
                                             message_router_request->service);
      }
    }
  }

  return kEipStatusOkSend;
}

void EncodeCipBool(const CipBool *const data,
                   ENIPMessage *const outgoing_message) {
  AddSintToMessage(*(EipUint8 *) (data), outgoing_message);
}

void EncodeCipByte(const CipByte *const data,
                   ENIPMessage *const outgoing_message) {
  AddSintToMessage(*(EipUint8 *) (data), outgoing_message);
}

void EncodeCipWord(const CipWord *const data,
                   ENIPMessage *const outgoing_message) {
  AddIntToMessage(*(EipUint16 *) (data), outgoing_message);
}

void EncodeCipDword(const CipDword *const data,
                    ENIPMessage *const outgoing_message) {
  AddDintToMessage(*(EipUint32 *) (data), outgoing_message);
}

void EncodeCipLword(const CipLword *const data,
                    ENIPMessage *const outgoing_message) {
  AddLintToMessage(*(EipUint64 *) (data), outgoing_message);
}

void EncodeCipUsint(const CipUsint *const data,
                    ENIPMessage *const outgoing_message) {
  AddSintToMessage(*(EipUint8 *) (data), outgoing_message);
}

void EncodeCipUint(const CipUint *const data,
                   ENIPMessage *const outgoing_message) {
  AddIntToMessage(*(EipUint16 *) (data), outgoing_message);
}

void EncodeCipUdint(const CipUdint *const data,
                    ENIPMessage *const outgoing_message) {
  AddDintToMessage(*(EipUint32 *) (data), outgoing_message);
}

void EncodeCipUlint(const CipUlint *const data,
                    ENIPMessage *const outgoing_message) {
  AddLintToMessage(*(EipUint64 *) (data), outgoing_message);
}

void EncodeCipSint(const CipSint *const data,
                   ENIPMessage *const outgoing_message) {
  AddSintToMessage(*(EipUint8 *) (data), outgoing_message);
}

void EncodeCipInt(const CipInt *const data,
                  ENIPMessage *const outgoing_message) {
  AddIntToMessage(*(EipUint16 *) (data), outgoing_message);
}

void EncodeCipDint(const CipDint *const data,
                   ENIPMessage *const outgoing_message) {
  AddDintToMessage(*(EipUint32 *) (data), outgoing_message);
}

void EncodeCipLint(const CipLint *const data,
                   ENIPMessage *const outgoing_message) {
  AddLintToMessage(*(EipUint64 *) (data), outgoing_message);
}

void EncodeCipReal(const CipReal *const data,
                   ENIPMessage *const outgoing_message) {
  AddDintToMessage(*(EipUint32 *) (data), outgoing_message);
}

void EncodeCipLreal(const CipLreal *const data,
                    ENIPMessage *const outgoing_message) {
  AddLintToMessage(*(EipUint64 *) (data), outgoing_message);
}

void EncodeCipShortString(const CipShortString *const data,
                          ENIPMessage *const outgoing_message) {
  CipShortString *const short_string = (CipShortString *) data;

  AddSintToMessage(short_string->length, outgoing_message);

  memcpy(outgoing_message->current_message_position,
         short_string->string,
         short_string->length);
  outgoing_message->current_message_position += short_string->length;
  outgoing_message->used_message_length += short_string->length;
}

void EncodeCipString(const CipString *const data,
                     ENIPMessage *const outgoing_message) {
  CipString *const string = (CipString *) data;

  AddIntToMessage(*(EipUint16 *) &(string->length), outgoing_message);
  if(0 != string->length) {
    memcpy(outgoing_message->current_message_position,
           string->string,
           string->length);
    outgoing_message->current_message_position += string->length;
    outgoing_message->used_message_length += string->length;

    if(outgoing_message->used_message_length & 0x01) {
      /* we have an odd byte count */
      AddSintToMessage(0, outgoing_message);
    }
  }
}

void EncodeCipString2(const CipString2 *const data,
                      ENIPMessage *const outgoing_message) {
  OPENER_ASSERT(false); /* Not implemented yet */
}

void EncodeCipStringN(const CipStringN *const data,
                      ENIPMessage *const outgoing_message) {
  OPENER_ASSERT(false); /* Not implemented yet */
}

static void CipStringIHeaderEncoding(const CipStringIStruct *const string,
                                     ENIPMessage *const outgoing_message) {
  EncodeCipUsint(&(string->language_char_1), outgoing_message);
  EncodeCipUsint(&(string->language_char_2), outgoing_message);
  EncodeCipUsint(&(string->language_char_3), outgoing_message);
  EncodeCipUsint(&(string->char_string_struct), outgoing_message);
  EncodeCipUint(&(string->character_set), outgoing_message);
}

void EncodeCipStringI(const CipStringI *const data,
                      ENIPMessage *const outgoing_message) {
  const CipStringI *const string_i = data;
  EncodeCipUsint(&(string_i->number_of_strings), outgoing_message);
  for(size_t i = 0; i < string_i->number_of_strings; ++i) {
    CipStringIHeaderEncoding( (string_i->array_of_string_i_structs) + i,
                              outgoing_message );
    switch(string_i->array_of_string_i_structs[i].char_string_struct) {
      case kCipString:
        EncodeCipString(string_i->array_of_string_i_structs[i].string,
                        outgoing_message);
        break;
      case kCipString2:
        EncodeCipString2(string_i->array_of_string_i_structs[i].string,
                         outgoing_message);
        break;
      case kCipStringN:
        EncodeCipStringN(string_i->array_of_string_i_structs[i].string,
                         outgoing_message);
        break;
      case kCipShortString:
        EncodeCipShortString(string_i->array_of_string_i_structs[i].string,
                             outgoing_message);
        break;
      default:
        OPENER_ASSERT(false);
        break;
    }
  }
}

void EncodeCipByteArray(const CipByteArray *const data,
                        ENIPMessage *const outgoing_message) {
  OPENER_TRACE_INFO(" -> get attribute byte array\r\n");
  CipByteArray *cip_byte_array = (CipByteArray *) data;
  memcpy(outgoing_message->current_message_position,
         cip_byte_array->data,
         cip_byte_array->length);
  outgoing_message->current_message_position += cip_byte_array->length;
  outgoing_message->used_message_length += cip_byte_array->length;
}

void EncodeCipEPath(const CipEpath *const data,
                    ENIPMessage *const outgoing_message) {
  EncodeEPath( (CipEpath *) data, outgoing_message );
}

void EncodeCipEthernetLinkPhyisicalAddress(const void *const data,
                                           ENIPMessage *const outgoing_message)
{
  EipUint8 *p = (EipUint8 *) data;
  memcpy(outgoing_message->current_message_position, p, 6);
  outgoing_message->current_message_position += 6;
  outgoing_message->used_message_length += 6;
}

int DecodeData(const EipUint8 cip_data_type,
               void *const cip_data,
               const EipUint8 **const cip_message) {
  int number_of_decoded_bytes = -1;

  switch(cip_data_type)
  /* check the data type of attribute */
  {
    case (kCipBool):
    case (kCipSint):
    case (kCipUsint):
    case (kCipByte):
      *(EipUint8 *) (cip_data) = **cip_message;
      ++(*cip_message);
      number_of_decoded_bytes = 1;
      break;

    case (kCipInt):
    case (kCipUint):
    case (kCipWord):
      (*(EipUint16 *) (cip_data) ) = GetUintFromMessage(cip_message);
      number_of_decoded_bytes = 2;
      break;

    case (kCipDint):
    case (kCipUdint):
    case (kCipDword):
    case (kCipReal):
      (*(EipUint32 *) (cip_data) ) = GetUdintFromMessage(cip_message);
      number_of_decoded_bytes = 4;
      break;

    case (kCipLint):
    case (kCipUlint):
    case (kCipLword): {
      (*(EipUint64 *) (cip_data) ) = GetLintFromMessage(cip_message);
      number_of_decoded_bytes = 8;
    }
    break;

    case (kCipString): {
      CipString *string = (CipString *) cip_data;
      string->length = GetUintFromMessage(cip_message);
      memcpy(string->string, *cip_message, string->length);
      *cip_message += string->length;

      number_of_decoded_bytes = string->length + 2; /* we have a two byte length field */
      if(number_of_decoded_bytes & 0x01) {
        /* we have an odd byte count */
        ++(*cip_message);
        number_of_decoded_bytes++;
      }
    }
    break;
    case (kCipShortString): {
      CipShortString *short_string = (CipShortString *) cip_data;

      short_string->length = **cip_message;
      ++(*cip_message);

      memcpy(short_string->string, *cip_message, short_string->length);
      *cip_message += short_string->length;

      number_of_decoded_bytes = short_string->length + 1;
      break;
    }

    default:
      break;
  }

  return number_of_decoded_bytes;
}

CipServiceStruct *GetCipService(const CipInstance *const instance,
                                CipUsint service_number) {
  CipServiceStruct *service = instance->cip_class->services;
  for(size_t i = 0; i < instance->cip_class->number_of_services; i++) /* hunt for the GET_ATTRIBUTE_SINGLE service*/
  {
    if(service->service_number == service_number) {
      return service; /* found the service */
    }
    service++;
  }
  return NULL; /* didn't find the service */
}

EipStatus GetAttributeAll(CipInstance *instance,
                          CipMessageRouterRequest *message_router_request,
                          CipMessageRouterResponse *message_router_response,
                          const struct sockaddr *originator_address,
                          const int encapsulation_session) {

  InitializeENIPMessage(&message_router_response->message);
  CipAttributeStruct *attribute = instance->attributes; /* pointer to list of attributes*/

  //Missing header

  if(0 == instance->cip_class->number_of_attributes) {
    /*there are no attributes to be sent back*/
    message_router_response->reply_service =
      (0x80 | message_router_request->service);
    message_router_response->general_status = kCipErrorServiceNotSupported;
    message_router_response->size_of_additional_status = 0;
  } else {
    GenerateGetAttributeSingleHeader(message_router_request,
                                     message_router_response);
    message_router_response->general_status = kCipErrorSuccess;
    for(size_t j = 0; j < instance->cip_class->number_of_attributes; j++) {
      /* for each instance attribute of this class */
      EipUint16 attribute_number = attribute->attribute_number;
      if( (instance->cip_class->get_all_bit_mask[CalculateIndex(attribute_number)
           ]) & (1 << (attribute_number % 8) ) ) {
        /* only return attributes that are flagged as being part of GetAttributeAll */
        message_router_request->request_path.attribute_number =
          attribute_number;

        attribute->encode(attribute->data, &message_router_response->message);
      }
      attribute++;
    }
  }
  return kEipStatusOkSend;
}

void EncodeEPath(CipEpath *epath,
                 ENIPMessage *message) {
  unsigned int length = epath->path_size;
  size_t start_length = message->used_message_length;
  AddIntToMessage(epath->path_size, message);

  if(epath->class_id < 256) {
    AddSintToMessage(0x20, message); /* 8 Bit Class Id */
    AddSintToMessage( (EipUint8) epath->class_id, message );
    length -= 1;
  } else {
    AddSintToMessage(0x21, message); /*16Bit Class Id */
    AddSintToMessage(0, message); /*pad byte */
    AddIntToMessage(epath->class_id, message);
    length -= 2;
  }

  if(0 < length) {
    if(epath->instance_number < 256) {
      AddSintToMessage(0x24, message); /*8Bit Instance Id */
      AddSintToMessage(epath->instance_number, message);
      length -= 1;
    } else {
      AddSintToMessage(0x25, message); /*16Bit Instance Id */
      AddSintToMessage(0, message); /*pad byte */
      AddIntToMessage(epath->instance_number, message);
      length -= 2;
    }

    if(0 < length) {
      if(epath->attribute_number < 256) {
        AddSintToMessage(0x30, message); /*8Bit Attribute Id */
        AddSintToMessage(epath->attribute_number, message);
        length -= 1;
      } else {
        AddSintToMessage(0x31, message); /*16Bit Attribute Id */
        AddSintToMessage(0, message); /*pad byte */
        AddIntToMessage(epath->attribute_number, message);
        length -= 2;
      }
    }
  }

  OPENER_ASSERT(
    2 + epath->path_size * 2 == message->used_message_length - start_length);             /* path size is in 16 bit chunks according to the specification */
}

int DecodePaddedEPath(CipEpath *epath,
                      const EipUint8 **message) {
  unsigned int number_of_decoded_elements = 0;
  const EipUint8 *message_runner = *message;

  epath->path_size = *message_runner;
  message_runner++;
  /* copy path to structure, in version 0.1 only 8 bit for Class,Instance and Attribute, need to be replaced with function */
  epath->class_id = 0;
  epath->instance_number = 0;
  epath->attribute_number = 0;

  while(number_of_decoded_elements < epath->path_size) {
    if(kSegmentTypeReserved == ( (*message_runner) & kSegmentTypeReserved ) ) {
      /* If invalid/reserved segment type, segment type greater than 0xE0 */
      return kEipStatusError;
    }

    number_of_decoded_elements++; /*At least one element is decoded */
    switch(*message_runner) {
      case SEGMENT_TYPE_LOGICAL_SEGMENT + LOGICAL_SEGMENT_TYPE_CLASS_ID +
        LOGICAL_SEGMENT_FORMAT_EIGHT_BIT:
        epath->class_id = *(EipUint8 *) (message_runner + 1);
        message_runner += 2;
        break;

      case SEGMENT_TYPE_LOGICAL_SEGMENT + LOGICAL_SEGMENT_TYPE_CLASS_ID +
        LOGICAL_SEGMENT_FORMAT_SIXTEEN_BIT:
        message_runner += 2;
        epath->class_id = GetUintFromMessage(&(message_runner) );
        number_of_decoded_elements++;
        break;

      case SEGMENT_TYPE_LOGICAL_SEGMENT + LOGICAL_SEGMENT_TYPE_INSTANCE_ID +
        LOGICAL_SEGMENT_FORMAT_EIGHT_BIT:
        epath->instance_number = *(EipUint8 *) (message_runner + 1);
        message_runner += 2;
        break;

      case SEGMENT_TYPE_LOGICAL_SEGMENT + LOGICAL_SEGMENT_TYPE_INSTANCE_ID +
        LOGICAL_SEGMENT_FORMAT_SIXTEEN_BIT:
        message_runner += 2;
        epath->instance_number = GetUintFromMessage(&(message_runner) );
        number_of_decoded_elements++;
        break;

      case SEGMENT_TYPE_LOGICAL_SEGMENT + LOGICAL_SEGMENT_TYPE_ATTRIBUTE_ID +
        LOGICAL_SEGMENT_FORMAT_EIGHT_BIT:
        epath->attribute_number = *(EipUint8 *) (message_runner + 1);
        message_runner += 2;
        break;

      case SEGMENT_TYPE_LOGICAL_SEGMENT + LOGICAL_SEGMENT_TYPE_ATTRIBUTE_ID +
        LOGICAL_SEGMENT_FORMAT_SIXTEEN_BIT:
        message_runner += 2;
        epath->attribute_number = GetUintFromMessage(&(message_runner) );
        number_of_decoded_elements++;
        break;

      case SEGMENT_TYPE_LOGICAL_SEGMENT + LOGICAL_SEGMENT_TYPE_MEMBER_ID +
        LOGICAL_SEGMENT_FORMAT_EIGHT_BIT:
        message_runner += 2;
        break;
      case SEGMENT_TYPE_LOGICAL_SEGMENT + LOGICAL_SEGMENT_TYPE_MEMBER_ID +
        LOGICAL_SEGMENT_FORMAT_SIXTEEN_BIT:
        message_runner += 2;
        number_of_decoded_elements++;
        break;

      default:
        OPENER_TRACE_ERR("wrong path requested\n");
        return kEipStatusError;
    }
  }

  *message = message_runner;
  return number_of_decoded_elements * 2 + 1; /* number_of_decoded_elements times 2 as every encoding uses 2 bytes */
}

#if 1 /* Renesas EIP Sample : CT18.1 Error "The DUT does not support required Electronic Key Format 4." */
int DecodePaddedEPathKeyFromat(CipEpath *epath,
                      const EipUint8 **message,
                      EipUint8 *general_status,
                      EipUint16 *extended_status) {
  unsigned int number_of_decoded_elements = 0;
  const EipUint8 *message_runner = *message;
  EipUint8 key_format = 0;
  CipByte compatiblity_mode = 0;
  CipUint vendor_id = 0;
  CipUint device_type = 0;
  CipUint product_code = 0;
  CipByte major_revision_compatibility = 0;
  CipUsint minor_revision = 0;
  CipUdint serial_number = 0;

  epath->path_size = *message_runner;
  message_runner++;
  /* copy path to structure, in version 0.1 only 8 bit for Class,Instance and Attribute, need to be replaced with function */
  epath->class_id = 0;
  epath->instance_number = 0;
  epath->attribute_number = 0;

  while(number_of_decoded_elements < epath->path_size) {
    if(kSegmentTypeReserved == ( (*message_runner) & kSegmentTypeReserved ) ) {
      /* If invalid/reserved segment type, segment type greater than 0xE0 */
      return kEipStatusError;
    }

    number_of_decoded_elements++; /*At least one element is decoded */
    switch(*message_runner) {
      case SEGMENT_TYPE_LOGICAL_SEGMENT + LOGICAL_SEGMENT_TYPE_CLASS_ID +
        LOGICAL_SEGMENT_FORMAT_EIGHT_BIT:
        epath->class_id = *(EipUint8 *) (message_runner + 1);
        message_runner += 2;
        break;

      case SEGMENT_TYPE_LOGICAL_SEGMENT + LOGICAL_SEGMENT_TYPE_CLASS_ID +
        LOGICAL_SEGMENT_FORMAT_SIXTEEN_BIT:
        message_runner += 2;
        epath->class_id = GetUintFromMessage(&(message_runner) );
        number_of_decoded_elements++;
        break;

      case SEGMENT_TYPE_LOGICAL_SEGMENT + LOGICAL_SEGMENT_TYPE_INSTANCE_ID +
        LOGICAL_SEGMENT_FORMAT_EIGHT_BIT:
        epath->instance_number = *(EipUint8 *) (message_runner + 1);
        message_runner += 2;
        break;

      case SEGMENT_TYPE_LOGICAL_SEGMENT + LOGICAL_SEGMENT_TYPE_INSTANCE_ID +
        LOGICAL_SEGMENT_FORMAT_SIXTEEN_BIT:
        message_runner += 2;
        epath->instance_number = GetUintFromMessage(&(message_runner) );
        number_of_decoded_elements++;
        break;

      case SEGMENT_TYPE_LOGICAL_SEGMENT + LOGICAL_SEGMENT_TYPE_ATTRIBUTE_ID +
        LOGICAL_SEGMENT_FORMAT_EIGHT_BIT:
        epath->attribute_number = *(EipUint8 *) (message_runner + 1);
        message_runner += 2;
        break;

      case SEGMENT_TYPE_LOGICAL_SEGMENT + LOGICAL_SEGMENT_TYPE_ATTRIBUTE_ID +
        LOGICAL_SEGMENT_FORMAT_SIXTEEN_BIT:
        message_runner += 2;
        epath->attribute_number = GetUintFromMessage(&(message_runner) );
        number_of_decoded_elements++;
        break;

      case SEGMENT_TYPE_LOGICAL_SEGMENT + LOGICAL_SEGMENT_TYPE_MEMBER_ID +
        LOGICAL_SEGMENT_FORMAT_EIGHT_BIT:
        message_runner += 2;
        break;
      case SEGMENT_TYPE_LOGICAL_SEGMENT + LOGICAL_SEGMENT_TYPE_MEMBER_ID +
        LOGICAL_SEGMENT_FORMAT_SIXTEEN_BIT:
        message_runner += 2;
        number_of_decoded_elements++;
        break;

      case SEGMENT_TYPE_LOGICAL_SEGMENT + LOGICAL_SEGMENT_TYPE_SPECIAL:
        key_format = *(EipUint8 *) (message_runner + 1);

        if (key_format == ELECTRONIC_KEY_SEGMENT_KEY_FORMAT_4) {
          memcpy((void *)&vendor_id, (void *)&message_runner[2], sizeof(vendor_id));
          memcpy((void *)&device_type, (void *)&message_runner[4], sizeof(device_type));
          memcpy((void *)&product_code, (void *)&message_runner[6], sizeof(product_code));
          memcpy((void *)&major_revision_compatibility, (void *)&message_runner[8], sizeof(major_revision_compatibility));
          memcpy((void *)&minor_revision, (void *)&message_runner[9], sizeof(minor_revision));
          compatiblity_mode = (major_revision_compatibility & 0x80);
          compatiblity_mode = (compatiblity_mode >> 15);

          /* Check VendorID and ProductCode, must match, or 0 */
          if( ( (vendor_id != g_identity.vendor_id) &&
                (vendor_id != 0) )
              || ( (product_code !=
                    g_identity.product_code) &&
                   (product_code != 0) ) ) {
            *general_status = kCipErrorKeyFailureInPath;
            *extended_status = 0x0114;
            return kEipStatusError;
          } else {
            /* VendorID and ProductCode are correct */

            /* Check DeviceType, must match or 0 */
            if( (device_type !=
                 g_identity.device_type) &&
                (device_type != 0) ) {
              *general_status = kCipErrorKeyFailureInPath;
              *extended_status = 0x0115;
              return kEipStatusError;
            } else {
              /* VendorID, ProductCode and DeviceType are correct */

              if(false == compatiblity_mode) {
                /* Major = 0 is valid */
                if(0 != major_revision_compatibility ) {
                  /* Check Major / Minor Revision, Major must match, Minor match or 0 */
                  if( (major_revision_compatibility !=
                       g_identity.revision.major_revision)
                      || ( (minor_revision !=
                            g_identity.revision.minor_revision) &&
                           (minor_revision != 0) ) ) {
                    *general_status = kCipErrorKeyFailureInPath;
                    *extended_status = 0x0116;
                    return kEipStatusError;
                  }
                }

              } else {
                /* Compatibility mode is set */

                /* Major must match, Minor != 0 and <= MinorRevision */
                if( (major_revision_compatibility !=
                     g_identity.revision.major_revision) ||
                    (minor_revision == 0)
                    || (minor_revision >
                        g_identity.revision.minor_revision) ) {
                  *general_status = kCipErrorKeyFailureInPath;
                  *extended_status = 0x0116;
                  return kEipStatusError;
                }
              } /* end if CompatiblityMode handling */
            }
          }

          message_runner += 10;
          number_of_decoded_elements += 4;

        } else {
          OPENER_TRACE_ERR("wrong path requested\n");
          return kEipStatusError;
        }
        break;

      default:
        OPENER_TRACE_ERR("wrong path requested\n");
        return kEipStatusError;
    }
  }

  *message = message_runner;
  return number_of_decoded_elements * 2 + 1; /* number_of_decoded_elements times 2 as every encoding uses 2 bytes */
}
#endif 

void AllocateAttributeMasks(CipClass *target_class) {
  unsigned size = 1 + CalculateIndex(target_class->highest_attribute_number);
  OPENER_TRACE_INFO(
    ">>> Allocate memory for %s %u bytes times 3 for masks\n",
    target_class->class_name, size);
  target_class->get_single_bit_mask = CipCalloc(size, sizeof(uint8_t) );
  target_class->set_bit_mask = CipCalloc(size, sizeof(uint8_t) );
  target_class->get_all_bit_mask = CipCalloc(size, sizeof(uint8_t) );
}

size_t CalculateIndex(EipUint16 attribute_number) {
  size_t index = attribute_number / 8;
  return index;
}
