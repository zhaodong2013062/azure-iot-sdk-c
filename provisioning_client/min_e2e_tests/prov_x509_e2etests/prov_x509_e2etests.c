// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdlib.h>
#include <stdbool.h>
#include "testrunnerswitcher.h"

#include "azure_c_shared_utility/platform.h"
//#include "azure_c_shared_utility/threadapi.h"
#include "azure_c_shared_utility/crt_abstractions.h"
#include "azure_macro_utils/macro_utils.h"
#include "azure_c_shared_utility/xlogging.h"
#include "azure_c_shared_utility/envvariable.h"

#include "azure_prov_client/prov_security_factory.h"
#include "azure_prov_client/internal/prov_auth_client.h"

#include "azure_prov_client/prov_transport_http_client.h"
#include "azure_prov_client/prov_transport_amqp_client.h"
#include "azure_prov_client/prov_transport_amqp_ws_client.h"
#include "azure_prov_client/prov_transport_mqtt_client.h"
#include "azure_prov_client/prov_transport_mqtt_ws_client.h"

#include "min_prov_e2e_common.h"

static const char* g_dps_scope_id = NULL;
static const char* g_dps_uri = NULL;
static const char* g_desired_iothub = NULL;
static const char* g_desired_dev_id = NULL;
static bool g_enable_tracing = true;

BEGIN_TEST_SUITE(prov_x509_e2etests)

    TEST_SUITE_INITIALIZE(TestClassInitialize)
    {
        platform_init();
        prov_dev_security_init(SECURE_DEVICE_TYPE_X509);

        g_dps_uri = environment_get_variable(DPS_GLOBAL_ENDPOINT);
        ASSERT_IS_NOT_NULL(g_dps_uri, "DPS_GLOBAL_ENDPOINT is NULL");

        g_dps_scope_id = environment_get_variable(DPS_ID_SCOPE);
        ASSERT_IS_NOT_NULL(g_dps_scope_id, "DPS_ID_SCOPE is NULL");

        g_desired_dev_id = environment_get_variable(DPS_EXPECTED_DEV_ID);
        g_desired_iothub = environment_get_variable(DPS_EXPECTED_HUB);
    }

    TEST_SUITE_CLEANUP(TestClassCleanup)
    {
        prov_dev_security_deinit();
        platform_deinit();
    }

    TEST_FUNCTION_INITIALIZE(method_init)
    {
    }

    TEST_FUNCTION_CLEANUP(method_cleanup)
    {
    }

    TEST_FUNCTION(dps_register_x509_device_mqtt_success)
    {
        send_dps_test_registration(g_dps_uri, g_dps_scope_id, Prov_Device_MQTT_Protocol, g_desired_dev_id, g_desired_iothub);
    }

END_TEST_SUITE(prov_x509_e2etests)
