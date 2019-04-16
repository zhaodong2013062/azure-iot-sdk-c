// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef MIN_E2E_TEST_COMMON_H
#define MIN_E2E_TEST_COMMON_H

#include "iothub_device_client.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum TEST_PROTOCOL_TYPE_TAG
{
    TEST_MQTT,
    TEST_MQTT_WEBSOCKETS,
    TEST_AMQP,
    TEST_AMQP_WEBSOCKETS,
    TEST_HTTP
} TEST_PROTOCOL_TYPE;

typedef enum DEVICE_CREATION_TYPE_TAG
{
    DEVICE_CREATION_CONN_STRING,
    DEVICE_CREATION_CREATE,
    DEVICE_CREATION_WITH_TRANSPORT
} DEVICE_CREATION_TYPE;

typedef enum MESSAGE_CREATION_MECHANISM_TAG
{
    TEST_MESSAGE_CREATE_BYTE_ARRAY,
    TEST_MESSAGE_CREATE_STRING,
    TEST_METHODS_CREATE
} MESSAGE_CREATION_MECHANISM;

typedef enum MIN_E2E_TEST_TYPE_TAG
{
    TEST_TYPE_C2D_D2C,
    TEST_TYPE_METHODS,
    TEST_TYPE_TWIN
} MIN_E2E_TEST_TYPE;

typedef enum CONTROL_PACKET_CMDS_TAG
{
    CTRL_COMMAND_HELLO = 100,
    CTRL_COMMAND_GOODBYE = 110,
    CTRL_COMMAND_BEGIN = 120,
    CTRL_COMMAND_END_SUCCESS = 130,
    CTRL_COMMAND_END_FAILURE = 140
} CONTROL_PACKET_CMDS;

typedef struct MIN_E2E_TEST_INFO_TAG* MIN_E2E_TEST_HANDLE;

extern MIN_E2E_TEST_HANDLE min_e2e_create(TEST_PROTOCOL_TYPE protocol_type);
extern void min_e2e_destroy(MIN_E2E_TEST_HANDLE handle);

extern int min_e2e_open_ctrl_channel(MIN_E2E_TEST_HANDLE handle, DEVICE_CREATION_TYPE type, MIN_E2E_TEST_TYPE test_type, IOTHUB_CLIENT_TRANSPORT_PROVIDER protocol);
extern void min_e2e_close_ctrl_channel(MIN_E2E_TEST_HANDLE handle);
extern int min_e2e_execute_telemetry_tests(MIN_E2E_TEST_HANDLE handle, size_t message_count, MESSAGE_CREATION_MECHANISM msg_create_type);
extern int min_e2e_execute_method_tests(MIN_E2E_TEST_HANDLE handle);
extern int min_e2e_execute_twin_tests(MIN_E2E_TEST_HANDLE handle);

#ifdef __cplusplus
}
#endif

#endif // MIN_E2E_TEST_COMMON_H
