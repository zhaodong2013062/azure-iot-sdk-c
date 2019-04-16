// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdio.h>
#include <stdlib.h>

#include "testrunnerswitcher.h"

#include "iothubtransportmqtt.h"
#include "min_e2e_test_common.h"
#include "iothub.h"

//static BACK_COMPAT_HANDLE g_back_compat_handle;
static MIN_E2E_TEST_HANDLE g_min_e2e_handle;

BEGIN_TEST_SUITE(iothub_mqtt_e2etests)

    TEST_SUITE_INITIALIZE(suite_init)
    {
        (void)IoTHub_Init();
        g_min_e2e_handle = min_e2e_create(TEST_MQTT);
        ASSERT_IS_NOT_NULL(g_min_e2e_handle, "Failure creating min e2e handle");
    }

    TEST_SUITE_CLEANUP(suite_cleanup)
    {
        min_e2e_destroy(g_min_e2e_handle);
        IoTHub_Deinit();
    }

    TEST_FUNCTION_INITIALIZE(method_init)
    {
    }

    TEST_FUNCTION_CLEANUP(method_cleanup)
    {
    }

    TEST_FUNCTION(MQTT_test_telemetry_string_with_connection_string)
    {
        int result = min_e2e_open_ctrl_channel(g_min_e2e_handle, DEVICE_CREATION_CONN_STRING, TEST_TYPE_C2D_D2C, MQTT_Protocol);
        ASSERT_ARE_EQUAL(int, 0, result, "Failure opening control channel");

        result = min_e2e_execute_telemetry_tests(g_min_e2e_handle, 1, TEST_MESSAGE_CREATE_STRING);
        ASSERT_ARE_EQUAL(int, 0, result, "Failure executing telemetry tests");

        min_e2e_close_ctrl_channel(g_min_e2e_handle);
    }

    // TEST_FUNCTION(MQTT_test_telemetry_byte_array_with_create)
    // {
    //     int result = send_telemetry_with_device_client(g_back_compat_handle, DEVICE_CREATION_CREATE, TEST_MESSAGE_CREATE_BYTE_ARRAY, MQTT_Protocol);
    //     ASSERT_ARE_EQUAL(int, 0, result);
    // }

    // TEST_FUNCTION(MQTT_test_c2d_with_create)
    // {
    //     int result = test_c2d_with_device_client(g_back_compat_handle, DEVICE_CREATION_CREATE, MQTT_Protocol);
    //     ASSERT_ARE_EQUAL(int, 0, result);
    // }

    // TEST_FUNCTION(MQTT_test_methods_with_create)
    // {
    //     int result = test_method_with_device_client(g_back_compat_handle, DEVICE_CREATION_CREATE, MQTT_Protocol);
    //     ASSERT_ARE_EQUAL(int, 0, result);
    // }

    // TEST_FUNCTION(MQTT_test_methods_with_connection_string)
    // {
    //     int result = test_twin_with_device_client(g_back_compat_handle, DEVICE_CREATION_CONN_STRING, MQTT_Protocol);
    //     ASSERT_ARE_EQUAL(int, 0, result);
    // }

END_TEST_SUITE(iothub_mqtt_e2etests)
