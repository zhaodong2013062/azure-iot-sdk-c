// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

/** @file iothub_module_client_ll.h
*    @brief     APIs that allow a user (usually a module) to communicate
*             with an Azure IoTHub.
*
*    @details IoTHubModuleClient_LL is a module that allows a user (usually a
*             module) to communicate with an Azure IoTHub. It can send events
*             and receive messages. At any given moment in time there can only
*             be at most 1 message callback function.
*
*             This API surface contains a set of APIs that allows the user to
*             interact with the lower layer portion of the IoTHubClient. These APIs
*             contain @c _LL_ in their name, but retain the same functionality like the
*             @c IoTHubModuleClient_... APIs, with one difference. If the @c _LL_ APIs are
*             used then the user is responsible for scheduling when the actual work done
*             by the IoTHubClient happens (when the data is sent/received on/from the wire).
*             This is useful for constrained devices where spinning a separate thread is
*             often not desired.
*/

#ifndef IOTHUB_MODULE_CLIENT_LL_MOCK_H
#define IOTHUB_MODULE_CLIENT_LL_MOCK_H

#include "iothub_module_client_ll.h"

#ifdef __cplusplus
extern "C"
{
#endif

    MOCKABLE_FUNCTION(, IOTHUB_MODULE_CLIENT_LL_HANDLE, IoTHubModuleClient_LL_CreateFromConnectionString, const char*, connectionString, IOTHUB_CLIENT_TRANSPORT_PROVIDER, protocol);
    MOCKABLE_FUNCTION(, void, IoTHubModuleClient_LL_Destroy, IOTHUB_MODULE_CLIENT_LL_HANDLE, iotHubModuleClientHandle);
    MOCKABLE_FUNCTION(, IOTHUB_CLIENT_RESULT, IoTHubModuleClient_LL_SendEventAsync, IOTHUB_MODULE_CLIENT_LL_HANDLE, iotHubModuleClientHandle, IOTHUB_MESSAGE_HANDLE, eventMessageHandle, IOTHUB_CLIENT_EVENT_CONFIRMATION_CALLBACK, eventConfirmationCallback, void*, userContextCallback);
    MOCKABLE_FUNCTION(, IOTHUB_CLIENT_RESULT, IoTHubModuleClient_LL_GetSendStatus, IOTHUB_MODULE_CLIENT_LL_HANDLE, iotHubModuleClientHandle, IOTHUB_CLIENT_STATUS*, iotHubClientStatus);
    MOCKABLE_FUNCTION(, IOTHUB_CLIENT_RESULT, IoTHubModuleClient_LL_SetMessageCallback, IOTHUB_MODULE_CLIENT_LL_HANDLE, iotHubModuleClientHandle, IOTHUB_CLIENT_MESSAGE_CALLBACK_ASYNC, messageCallback, void*, userContextCallback);
    MOCKABLE_FUNCTION(, IOTHUB_CLIENT_RESULT, IoTHubModuleClient_LL_SetConnectionStatusCallback, IOTHUB_MODULE_CLIENT_LL_HANDLE, iotHubModuleClientHandle, IOTHUB_CLIENT_CONNECTION_STATUS_CALLBACK, connectionStatusCallback, void*, userContextCallback);
    MOCKABLE_FUNCTION(, IOTHUB_CLIENT_RESULT, IoTHubModuleClient_LL_SetRetryPolicy, IOTHUB_MODULE_CLIENT_LL_HANDLE, iotHubModuleClientHandle, IOTHUB_CLIENT_RETRY_POLICY, retryPolicy, size_t, retryTimeoutLimitInSeconds);
    MOCKABLE_FUNCTION(, IOTHUB_CLIENT_RESULT, IoTHubModuleClient_LL_GetRetryPolicy, IOTHUB_MODULE_CLIENT_LL_HANDLE, iotHubModuleClientHandle, IOTHUB_CLIENT_RETRY_POLICY*, retryPolicy, size_t*, retryTimeoutLimitInSeconds);
    MOCKABLE_FUNCTION(, IOTHUB_CLIENT_RESULT, IoTHubModuleClient_LL_GetLastMessageReceiveTime, IOTHUB_MODULE_CLIENT_LL_HANDLE, iotHubModuleClientHandle, time_t*, lastMessageReceiveTime);
    MOCKABLE_FUNCTION(, void, IoTHubModuleClient_LL_DoWork, IOTHUB_MODULE_CLIENT_LL_HANDLE, iotHubModuleClientHandle);
    MOCKABLE_FUNCTION(, IOTHUB_CLIENT_RESULT, IoTHubModuleClient_LL_SetOption, IOTHUB_MODULE_CLIENT_LL_HANDLE, iotHubModuleClientHandle, const char*, optionName, const void*, value);
    MOCKABLE_FUNCTION(, IOTHUB_CLIENT_RESULT, IoTHubModuleClient_LL_SetModuleTwinCallback, IOTHUB_MODULE_CLIENT_LL_HANDLE, iotHubModuleClientHandle, IOTHUB_CLIENT_DEVICE_TWIN_CALLBACK, moduleTwinCallback, void*, userContextCallback);
    MOCKABLE_FUNCTION(, IOTHUB_CLIENT_RESULT, IoTHubModuleClient_LL_SendReportedState, IOTHUB_MODULE_CLIENT_LL_HANDLE, iotHubModuleClientHandle, const unsigned char*, reportedState, size_t, size, IOTHUB_CLIENT_REPORTED_STATE_CALLBACK, reportedStateCallback, void*, userContextCallback);
    MOCKABLE_FUNCTION(, IOTHUB_CLIENT_RESULT, IoTHubModuleClient_LL_SetModuleMethodCallback, IOTHUB_MODULE_CLIENT_LL_HANDLE, iotHubModuleClientHandle, IOTHUB_CLIENT_DEVICE_METHOD_CALLBACK_ASYNC, moduleMethodCallback, void*, userContextCallback);
    MOCKABLE_FUNCTION(, IOTHUB_CLIENT_RESULT, IoTHubModuleClient_LL_SendEventToOutputAsync, IOTHUB_MODULE_CLIENT_LL_HANDLE, iotHubModuleClientHandle, IOTHUB_MESSAGE_HANDLE, eventMessageHandle, const char*, outputName, IOTHUB_CLIENT_EVENT_CONFIRMATION_CALLBACK, eventConfirmationCallback, void*, userContextCallback);
    MOCKABLE_FUNCTION(, IOTHUB_CLIENT_RESULT, IoTHubModuleClient_LL_SetInputMessageCallback, IOTHUB_MODULE_CLIENT_LL_HANDLE, iotHubModuleClientHandle, const char*, inputName, IOTHUB_CLIENT_MESSAGE_CALLBACK_ASYNC, eventHandlerCallback, void*, userContextCallback);

#ifdef USE_EDGE_MODULES
    MOCKABLE_FUNCTION(, IOTHUB_MODULE_CLIENT_LL_HANDLE, IoTHubModuleClient_LL_CreateFromEnvironment, IOTHUB_CLIENT_TRANSPORT_PROVIDER, protocol);
    MOCKABLE_FUNCTION(, IOTHUB_CLIENT_RESULT, IoTHubModuleClient_LL_DeviceMethodInvoke, IOTHUB_MODULE_CLIENT_LL_HANDLE, iotHubModuleClientHandle, const char*, deviceId, const char*, methodName, const char*, methodPayload, unsigned int, timeout, int*, responseStatus, unsigned char**, responsePayload, size_t*, responsePayloadSize);
    MOCKABLE_FUNCTION(, IOTHUB_CLIENT_RESULT, IoTHubModuleClient_LL_ModuleMethodInvoke, IOTHUB_MODULE_CLIENT_LL_HANDLE, iotHubModuleClientHandle, const char*, deviceId, const char*, moduleId, const char*, methodName, const char*, methodPayload, unsigned int, timeout, int*, responseStatus, unsigned char**, responsePayload, size_t*, responsePayloadSize);
#endif /*USE_EDGE_MODULES*/

#ifdef __cplusplus
}
#endif

#endif /* IOTHUB_MODULE_CLIENT_LL_MOCK_H */
