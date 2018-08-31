// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

/** @file   iothub_message.h
*	@brief  The @c IoTHub_Message component encapsulates one message that
*           can be transferred by an IoT hub client.
*/

#ifndef IOTHUB_MESSAGE_MOCK_H
#define IOTHUB_MESSAGE_MOCK_H

#include "iothub_message.h"


MOCKABLE_FUNCTION(, IOTHUB_MESSAGE_HANDLE, IoTHubMessage_CreateFromByteArray, const unsigned char*, byteArray, size_t, size);
MOCKABLE_FUNCTION(, IOTHUB_MESSAGE_HANDLE, IoTHubMessage_CreateFromString, const char*, source);
MOCKABLE_FUNCTION(, IOTHUB_MESSAGE_HANDLE, IoTHubMessage_Clone, IOTHUB_MESSAGE_HANDLE, iotHubMessageHandle);
MOCKABLE_FUNCTION(, IOTHUB_MESSAGE_RESULT, IoTHubMessage_GetByteArray, IOTHUB_MESSAGE_HANDLE, iotHubMessageHandle, const unsigned char**, buffer, size_t*, size);
MOCKABLE_FUNCTION(, const char*, IoTHubMessage_GetString, IOTHUB_MESSAGE_HANDLE, iotHubMessageHandle);
MOCKABLE_FUNCTION(, IOTHUBMESSAGE_CONTENT_TYPE, IoTHubMessage_GetContentType, IOTHUB_MESSAGE_HANDLE, iotHubMessageHandle);
MOCKABLE_FUNCTION(, IOTHUB_MESSAGE_RESULT, IoTHubMessage_SetContentTypeSystemProperty, IOTHUB_MESSAGE_HANDLE, iotHubMessageHandle, const char*, contentType);
MOCKABLE_FUNCTION(, const char*, IoTHubMessage_GetContentTypeSystemProperty, IOTHUB_MESSAGE_HANDLE, iotHubMessageHandle);
MOCKABLE_FUNCTION(, IOTHUB_MESSAGE_RESULT, IoTHubMessage_SetContentEncodingSystemProperty, IOTHUB_MESSAGE_HANDLE, iotHubMessageHandle, const char*, contentEncoding);
MOCKABLE_FUNCTION(, const char*, IoTHubMessage_GetContentEncodingSystemProperty, IOTHUB_MESSAGE_HANDLE, iotHubMessageHandle);
MOCKABLE_FUNCTION(, MAP_HANDLE, IoTHubMessage_Properties, IOTHUB_MESSAGE_HANDLE, iotHubMessageHandle);
MOCKABLE_FUNCTION(, IOTHUB_MESSAGE_RESULT, IoTHubMessage_SetProperty, IOTHUB_MESSAGE_HANDLE, iotHubMessageHandle, const char*, key, const char*, value);
MOCKABLE_FUNCTION(, const char*, IoTHubMessage_GetProperty, IOTHUB_MESSAGE_HANDLE, iotHubMessageHandle, const char*, key);
MOCKABLE_FUNCTION(, const char*, IoTHubMessage_GetMessageId, IOTHUB_MESSAGE_HANDLE, iotHubMessageHandle);
MOCKABLE_FUNCTION(, IOTHUB_MESSAGE_RESULT, IoTHubMessage_SetMessageId, IOTHUB_MESSAGE_HANDLE, iotHubMessageHandle, const char*, messageId);
MOCKABLE_FUNCTION(, const char*, IoTHubMessage_GetCorrelationId, IOTHUB_MESSAGE_HANDLE, iotHubMessageHandle);
MOCKABLE_FUNCTION(, IOTHUB_MESSAGE_RESULT, IoTHubMessage_SetCorrelationId, IOTHUB_MESSAGE_HANDLE, iotHubMessageHandle, const char*, correlationId);
MOCKABLE_FUNCTION(, const IOTHUB_MESSAGE_DIAGNOSTIC_PROPERTY_DATA*, IoTHubMessage_GetDiagnosticPropertyData, IOTHUB_MESSAGE_HANDLE, iotHubMessageHandle);
MOCKABLE_FUNCTION(, IOTHUB_MESSAGE_RESULT, IoTHubMessage_SetDiagnosticPropertyData, IOTHUB_MESSAGE_HANDLE, iotHubMessageHandle, const IOTHUB_MESSAGE_DIAGNOSTIC_PROPERTY_DATA*, diagnosticData);
MOCKABLE_FUNCTION(, const char*, IoTHubMessage_GetOutputName, IOTHUB_MESSAGE_HANDLE, iotHubMessageHandle);
MOCKABLE_FUNCTION(, IOTHUB_MESSAGE_RESULT, IoTHubMessage_SetOutputName, IOTHUB_MESSAGE_HANDLE, iotHubMessageHandle, const char*, outputName);
MOCKABLE_FUNCTION(, const char*, IoTHubMessage_GetInputName, IOTHUB_MESSAGE_HANDLE, iotHubMessageHandle);
MOCKABLE_FUNCTION(, IOTHUB_MESSAGE_RESULT, IoTHubMessage_SetInputName, IOTHUB_MESSAGE_HANDLE, iotHubMessageHandle, const char*, inputName);
MOCKABLE_FUNCTION(, const char*, IoTHubMessage_GetConnectionModuleId, IOTHUB_MESSAGE_HANDLE, iotHubMessageHandle);
MOCKABLE_FUNCTION(, IOTHUB_MESSAGE_RESULT, IoTHubMessage_SetConnectionModuleId, IOTHUB_MESSAGE_HANDLE, iotHubMessageHandle, const char*, connectionModuleId);
MOCKABLE_FUNCTION(, const char*, IoTHubMessage_GetConnectionDeviceId, IOTHUB_MESSAGE_HANDLE, iotHubMessageHandle);
MOCKABLE_FUNCTION(, IOTHUB_MESSAGE_RESULT, IoTHubMessage_SetConnectionDeviceId, IOTHUB_MESSAGE_HANDLE, iotHubMessageHandle, const char*, connectionDeviceId);
MOCKABLE_FUNCTION(, void, IoTHubMessage_Destroy, IOTHUB_MESSAGE_HANDLE, iotHubMessageHandle);

#ifdef __cplusplus
}
#endif

#endif /* IOTHUB_MESSAGE_MOCK_H */
