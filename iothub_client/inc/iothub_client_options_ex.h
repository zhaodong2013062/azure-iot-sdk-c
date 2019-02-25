// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

/* Structures for configuration of IoTHub device and module clients */


#ifndef IOTHUB_CLIENT_OPTIONS_EX_H
#define IOTHUB_CLIENT_OPTIONS_EX_H

#include "azure_c_shared_utility/macro_utils.h"
#include "azure_c_shared_utility/umock_c_prod.h"

#include "iothub_client_core_common.h"


typedef enum IOTHUB_CLIENT_OPTIONS_TYPE_TAG
{
    IOTHUB_CLIENT_OPTIONS_TYPE_LOGGING_ENABLED,
    IOTHUB_CLIENT_OPTIONS_TYPE_PROXY,
    IOTHUB_CLIENT_OPTIONS_TYPE_PRODUCT_INFO,
    IOTHUB_CLIENT_OPTIONS_TYPE_CUSTOM,
    IOTHUB_CLIENT_OPTIONS_TYPE_TRUSTED_CERTIFICATES,
    IOTHUB_CLIENT_OPTIONS_TYPE_SAS_TOKEN_LIFETIME,
    IOTHUB_CLIENT_OPTIONS_TYPE_URL_AUTO_ENCODE_DECODE
} IOTHUB_CLIENT_OPTIONS_TYPE;

#define IOTHUB_CLIENT_OPTION_PROXY_VERSION_1 1
// Client configures HTTP proxy
typedef struct IOTHUB_CLIENT_OPTION_HTTP_PROXY_TAG
{
    int version;
    const char* address;
    const char* username;
    const char* password;
    int port;
} IOTHUB_CLIENT_OPTION_HTTP_PROXY;

#define IOTHUB_CLIENT_OPTION_CUSTOM_VERSION_1 1
// Allows arbitrary option to be passed to the IoTHub client (or its PAL).
typedef struct IOTHUB_CLIENT_OPTION_CUSTOM_TAG
{
    int version;
    const char* optionName;
    const void* optionValue;
} IOTHUB_CLIENT_OPTION_CUSTOM;

// Result of setting a given IOTHUB_CLIENT_OPTIONS structure.
typedef enum IOTHUB_SET_OPTION_RESULT_TAG
{
    // option succeeded
    IOTHUB_SET_OPTION_RESULT_OK,
    // option was not attempted because an option executed sooner in chain has failed
    IOTHUB_SET_OPTION_RESULT_NOT_ATTEMPTED,
    // option is an initialization only time parameter, but was attempted to be set afterward
    IOTHUB_SET_OPTION_RESULT_INITIALIZATION_ONLY,
    // option failed
    IOTHUB_SET_OPTION_RESULT_ERROR
} IOTHUB_SET_OPTION_RESULT;

typedef struct IOTHUB_CLIENT_OPTIONS_TAG
{
    IOTHUB_CLIENT_OPTIONS_TYPE optionType;
    bool ignoreErrors;
    IOTHUB_SET_OPTION_RESULT result;

    union
    {
        bool loggingEnabled;
        IOTHUB_CLIENT_OPTION_HTTP_PROXY httpProxy;
        const char* productInfo;
        IOTHUB_CLIENT_OPTION_CUSTOM custom;
        const char* trustedCertificates;
        int sasTokenLifetime;
        bool autoUrlEncodeDecode;
    } option;
} IOTHUB_CLIENT_OPTIONS;

#endif /* IOTHUB_CLIENT_OPTIONS_EX_H */

