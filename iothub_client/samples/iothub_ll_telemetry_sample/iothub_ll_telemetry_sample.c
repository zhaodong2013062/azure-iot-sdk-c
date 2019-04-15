// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// CAVEAT: This sample is to demonstrate azure IoT client concepts only and is not a guide design principles or style
// Checking of return codes and error values shall be omitted for brevity.  Please practice sound engineering practices
// when writing production code.

#include <stdio.h>
#include <stdlib.h>

#include "iothub.h"
#include "iothub_client.h"
#include "iothub_device_client.h"
#include "iothub_client_options.h"
#include "iothub_message.h"
#include "azure_c_shared_utility/threadapi.h"
#include "azure_c_shared_utility/crt_abstractions.h"
#include "azure_c_shared_utility/shared_util_options.h"

#include "iothubtransportamqp.h"

#define NUMDEVICES 4

typedef struct DEVICEKEYVALUE_TAG {
		const char* id;
		const char* key;
} DeviceKeyValue;

DeviceKeyValue DeviceList[NUMDEVICES];

static const char* hubName = "";
static const char* hubSuffix = "";


static void connectionStatusCallback(IOTHUB_CLIENT_CONNECTION_STATUS result, IOTHUB_CLIENT_CONNECTION_STATUS_REASON reason, void* user_context)
{
    (void)reason;
    (void)user_context;
    // This sample DOES NOT take into consideration network outages.
    if (result == IOTHUB_CLIENT_CONNECTION_AUTHENTICATED)
    {
        (void)printf("The device client is connected to iothub\r\n");
    }
    else
    {
        (void)printf("The device client has been disconnected\r\n");
    }
}


static IOTHUBMESSAGE_DISPOSITION_RESULT ReceiveMessageCallback(IOTHUB_MESSAGE_HANDLE message, void* userContextCallback)
{
    message;
    userContextCallback;
    (void)printf(" Received message. ");
    return IOTHUBMESSAGE_ACCEPTED;
}

static int DeviceMethodCallback(const char* method_name, const unsigned char* payload, size_t size, METHOD_HANDLE method_id, void* userContextCallback)
{
    (void)printf("Device method name: %s called\r\n", method_name);
    userContextCallback;
    size;
    payload;
    method_name;
	method_id;
    return 0;
}

static IOTHUB_DEVICE_CLIENT_HANDLE createClient(const char * deviceId, const char * deviceKey, IOTHUB_CLIENT_TRANSPORT_PROVIDER protocol, TRANSPORT_HANDLE transport_handle)
{
    bool traceOn = true;
    IOTHUB_DEVICE_CLIENT_HANDLE handle;
    IOTHUB_CLIENT_CONFIG config = { 0 };
    config.deviceId = deviceId;
    config.deviceKey = deviceKey;
    config.iotHubName = hubName;
    config.iotHubSuffix = hubSuffix;
    config.deviceSasToken = NULL;
    config.protocol = protocol;
    if ((handle = IoTHubDeviceClient_CreateWithTransport(transport_handle,&config)) == NULL) 
    {
        (void)printf("ERROR: iotHubClientHandle1 is Null!\r\n");
    } else
    {
        
    (void)IoTHubClient_SetOption(handle, OPTION_LOG_TRACE, &traceOn);
    (void)IoTHubClient_SetConnectionStatusCallback(handle, connectionStatusCallback, NULL);
    (void)	(handle, ReceiveMessageCallback, NULL);
    (void)IoTHubClient_SetDeviceMethodCallback_Ex(handle, DeviceMethodCallback, NULL);
    }

    return handle;
}

static void destroyClient(IOTHUB_DEVICE_CLIENT_HANDLE handle)
{
    (void)IoTHubClient_SetConnectionStatusCallback(handle, NULL, NULL);
	//(void)IoTHubClient_SetMessageCallback(handle, NULL, NULL);
    (void)IoTHubClient_SetDeviceMethodCallback_Ex(handle, NULL, NULL);
    (void)IoTHubClient_Destroy(handle);
}

int main(void)
{

	DeviceKeyValue deviceList[4];
	deviceList[0] = device1;
	deviceList[1] = device2;
	deviceList[2] = device3;
	deviceList[3] = device4;


    IOTHUB_CLIENT_TRANSPORT_PROVIDER protocol;
    protocol = AMQP_Protocol;
    TRANSPORT_HANDLE transport_handle;
	IOTHUB_DEVICE_CLIENT_HANDLE handleList[4];


    (void)IoTHub_Init();

    // CREATE TRANSPORT AND DEVICE HANDLES
    transport_handle = IoTHubTransport_Create(protocol, hubName, hubSuffix);
    if (transport_handle == NULL) 
    { 
        (void)printf("Failed to creating the protocol handle.\r\n");
        return 0;
    } 
	
	int b = 0;
	while (b < 15)
	{
		(void)printf("///////////////////////////////////////////////////////\r\n");
		int a;
		for (a = 0; a < NUMDEVICES; a++)
		{
			(void)printf("Creating IoTHub Device handle%d\r\n", a);
			handleList[a] = createClient(deviceList[a].id, deviceList[a].key, protocol, transport_handle);
			if (handleList[a] == NULL)
			{
				(void)printf("ERROR: iotHubClientHandle%d is Null!\r\n", a);
				return 0;
			}
		}
		// END CREATE TRANSPORT AND DEVICE HANDLES 

		ThreadAPI_Sleep(10);
		(void)printf("Destroying Handles...\r\n");
		for (a = 0; a < NUMDEVICES; a++)
		{
			(void)printf("Destroying IoTHub Device %d", a);
			destroyClient(handleList[a]);
		}
		(void)printf("Sleeping for 2 Seconds...");
		ThreadAPI_Sleep(2000);
		b++;
	}
	
	ThreadAPI_Sleep(1000);

    IoTHubTransport_Destroy(transport_handle);
    IoTHub_Deinit();

    printf("Press any key to continue");
    (void)getchar();

    return 0;
}
