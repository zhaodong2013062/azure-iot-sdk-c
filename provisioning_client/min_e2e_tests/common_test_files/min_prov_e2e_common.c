// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifdef __cplusplus
#include <cstdlib>
#include <cstddef>
#include <cstring>
#else
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "umock_c/umock_c.h"
#endif

#include "testrunnerswitcher.h"

#include "azure_c_shared_utility/threadapi.h"
#include "azure_c_shared_utility/crt_abstractions.h"
#include "azure_macro_utils/macro_utils.h"
#include "azure_c_shared_utility/xlogging.h"
#include "azure_c_shared_utility/strings.h"
#include "azure_c_shared_utility/uniqueid.h"
#include "azure_c_shared_utility/azure_base64.h"

#include "azure_prov_client/prov_device_ll_client.h"
#include "azure_prov_client/prov_security_factory.h"
#include "azure_prov_client/internal/prov_auth_client.h"

#include "prov_service_client/provisioning_service_client.h"
#include "prov_service_client/provisioning_sc_enrollment.h"

#include "../../../certs/certs.h"

#include "min_prov_e2e_common.h"

#define MAX_CLOUD_TRAVEL_TIME       60.0
#define DEVICE_GUID_SIZE            37

static bool g_tracing = true;
static const char* TEST_JSON_PAYLOAD = "{\"a\":\"b\"}";

TEST_DEFINE_ENUM_TYPE(PROV_DEVICE_RESULT, PROV_DEVICE_RESULT_VALUE);

static void iothub_prov_register_device(PROV_DEVICE_RESULT register_result, const char* iothub_uri, const char* device_id, void* user_context)
{
    if (user_context == NULL)
    {
        ASSERT_FAIL("User Context is NULL iothub_prov_register_device");
    }
    else
    {
        PROV_CLIENT_E2E_INFO* prov_info = (PROV_CLIENT_E2E_INFO*)user_context;
        if (register_result == PROV_DEVICE_RESULT_OK)
        {
            (void)mallocAndStrcpy_s(&prov_info->iothub_uri, iothub_uri);
            (void)mallocAndStrcpy_s(&prov_info->device_id, device_id);
            prov_info->reg_result = REG_RESULT_COMPLETE;
        }
        else
        {
            prov_info->reg_result = REG_RESULT_FAILED;
        }
    }
}

static void dps_registation_status(PROV_DEVICE_REG_STATUS reg_status, void* user_context)
{
    (void)reg_status;
    ASSERT_IS_NOT_NULL(user_context, "user_context is NULL");
    ThreadAPI_Sleep(250);
}

static void wait_for_dps_result(PROV_DEVICE_LL_HANDLE handle, PROV_CLIENT_E2E_INFO* prov_info)
{
    time_t begin_operation;
    time_t now_time;

    begin_operation = time(NULL);
    do
    {
        Prov_Device_LL_DoWork(handle);
        ThreadAPI_Sleep(1);
    } while ((prov_info->reg_result == REG_RESULT_BEGIN) &&
        ((now_time = time(NULL)),
        (difftime(now_time, begin_operation) < MAX_CLOUD_TRAVEL_TIME)));
}

void send_dps_test_registration(const char* global_uri, const char* scope_id, PROV_DEVICE_TRANSPORT_PROVIDER_FUNCTION protocol)
{
    PROV_CLIENT_E2E_INFO prov_info;
    memset(&prov_info, 0, sizeof(PROV_CLIENT_E2E_INFO));

    // arrange
    PROV_DEVICE_LL_HANDLE handle;
    handle = Prov_Device_LL_Create(global_uri, scope_id, protocol);
    ASSERT_IS_NOT_NULL(handle, "Failure create a DPS HANDLE");

    Prov_Device_LL_SetOption(handle, PROV_OPTION_LOG_TRACE, &g_tracing);

#ifdef SET_TRUSTED_CERT_IN_SAMPLES
    ASSERT_ARE_EQUAL(PROV_DEVICE_RESULT, PROV_DEVICE_RESULT_OK, Prov_Device_LL_SetOption(handle, OPTION_TRUSTED_CERT, certificates), "Failure setting Trusted Cert option");
#endif // SET_TRUSTED_CERT_IN_SAMPLES

    PROV_DEVICE_RESULT prov_result = Prov_Device_LL_Set_Provisioning_Payload(handle, TEST_JSON_PAYLOAD);
    ASSERT_ARE_EQUAL(PROV_DEVICE_RESULT, PROV_DEVICE_RESULT_OK, prov_result, "Failure calling Prov_Device_LL_Set_Provisioning_Payload");

    // act
    prov_result = Prov_Device_LL_Register_Device(handle, iothub_prov_register_device, &prov_info, dps_registation_status, &prov_info);
    ASSERT_ARE_EQUAL(PROV_DEVICE_RESULT, PROV_DEVICE_RESULT_OK, prov_result, "Failure calling Prov_Device_LL_Register_Device");

    wait_for_dps_result(handle, &prov_info);

    // Assert
    ASSERT_ARE_EQUAL(int, REG_RESULT_COMPLETE, prov_info.reg_result, "Failure calling registering device x509 http");

    free(prov_info.iothub_uri);
    free(prov_info.device_id);

    Prov_Device_LL_Destroy(handle);
}
