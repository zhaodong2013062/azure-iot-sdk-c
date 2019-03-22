// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.


#ifndef IOTHUB_MESSAGE_INTERNAL_H
#define IOTHUB_MESSAGE_INTERNAL_H


#include "azure_c_shared_utility/macro_utils.h"
#include "azure_c_shared_utility/umock_c_prod.h"

#include "iothub_message.h"

// The internal properties functions are used to manipulate properties of a message that the SDK uses internally only.

MOCKABLE_FUNCTION(, MAP_HANDLE, IoTHubMessage_InternalProperties, IOTHUB_MESSAGE_HANDLE, iotHubMessageHandle);
MOCKABLE_FUNCTION(, IOTHUB_MESSAGE_RESULT, IoTHubMessage_SetInternalProperty, IOTHUB_MESSAGE_HANDLE, iotHubMessageHandle, const char*, key, const char*, value);
MOCKABLE_FUNCTION(, const char*, IoTHubMessage_GetInternalProperty, IOTHUB_MESSAGE_HANDLE, iotHubMessageHandle, const char*, key);

#endif /* IOTHUB_MESSAGE_INTERNAL_H */
