// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef MIN_PROV_E2E_COMMON_H
#define MIN_PROV_E2E_COMMON_H

#ifdef __cplusplus
extern "C" {
#include <cstdlib>
#else
#include <stdlib.h>
#endif

#include "azure_prov_client/prov_device_ll_client.h"

typedef enum REGISTRATION_RESULT_TAG
{
    REG_RESULT_BEGIN,
    REG_RESULT_COMPLETE,
    REG_RESULT_FAILED
} REGISTRATION_RESULT;

typedef struct PROV_CLIENT_E2E_INFO_TAG
{
    char* iothub_uri;
    char* device_id;
    REGISTRATION_RESULT reg_result;
} PROV_CLIENT_E2E_INFO;

static const char* const DPS_CONNECTION_STRING = "IOT_DPS_CONNECTION_STRING";
static const char* const DPS_GLOBAL_ENDPOINT = "IOT_DPS_GLOBAL_ENDPOINT";
static const char* const DPS_ID_SCOPE = "IOT_DPS_ID_SCOPE";
static const char* const DPS_TPM_SIMULATOR_IP_ADDRESS = "IOT_DPS_TPM_SIMULATOR_IP_ADDRESS";
static const char* const DPS_EXPECTED_HUB = "IOT_DPS_EXPECTED_HUB";
static const char* const DPS_EXPECTED_DEV_ID = "IOT_DPS_EXPECTED_DEV_ID";

extern void send_dps_test_registration(const char* global_uri, const char* scope_id, PROV_DEVICE_TRANSPORT_PROVIDER_FUNCTION protocol, const char* expected_dev_id, const char* expected_uri);

#ifdef __cplusplus
}
#endif

#endif /* MIN_PROV_E2E_COMMON_H */
