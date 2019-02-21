// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

/* Structures for creation of IoTHub device and module clients */

#ifndef IOTHUB_CLIENT_CREATE_H
#define IOTHUB_CLIENT_CREATE_H

#include "azure_c_shared_utility/macro_utils.h"
#include "azure_c_shared_utility/umock_c_prod.h"

#include "iothub_client_core_common.h"

typedef enum IOTHUB_CLIENT_AUTH_TYPE_TAG
{
    IOTHUB_CLIENT_AUTH_TYPE_CONNECTION_STRING,
    IOTHUB_CLIENT_AUTH_TYPE_X509_THUMBPRINT_AUTH,
    IOTHUB_CLIENT_AUTH_TYPE_X509_CA_CERTIFICATE_AUTH,
    IOTHUB_CLIENT_AUTH_TYPE_FOR_MULTIPLEXED,
    IOTHUB_CLIENT_AUTH_TYPE_EDGE_ENVIRONMENT,
    IOTHUB_CLIENT_AUTH_TYPE_FROM_DEVICE_AUTHENTICATION
} IOTHUB_CLIENT_AUTH_TYPE;

#define IOTHUB_CLIENT_AUTH_CONNECTION_STRING_VERSION_1 1
typedef struct IOTHUB_CLIENT_AUTH_CONNECTION_STRING_TAG
{
    int version;
    const char* connectionString;
} IOTHUB_CLIENT_AUTH_CONNECTION_STRING;

#define IOTHUB_CLIENT_AUTH_X509_THUMBPRINT_VERSION_1 1
typedef struct IOTHUB_CLIENT_AUTH_X509_THUMBPRINT_TAG
{
    int version;
    const char* connectionString;
    const char* privateKey;
    const char* publicKey;
} IOTHUB_CLIENT_AUTH_X509_THUMBPRINT;

#define IOTHUB_CLIENT_AUTH_X509_CA_CERTIFICATE_VERSION_1 1
typedef struct 
{
    int version;
    const char* connectionString;
    const char* privateKey;
    const char* publicKey;
} IOTHUB_CLIENT_AUTH_X509_CA_CERTIFICATE;

#define IOTHUB_CLIENT_AUTH_FOR_MULTIPLEXED_CONNECTION_VERSION_1 1
typedef struct IOTHUB_CLIENT_AUTH_FOR_MULTIPLEXED_CONNECTION_TAG
{
    int version;
    const char* deviceId;
    const char* deviceKey;
    const char* deviceSasToken;
    const char* iotHubName;
    const char* iotHubSuffix;
    const char* protocolGatewayHostName;
} IOTHUB_CLIENT_AUTH_FOR_MULTIPLEXED_CONNECTION;

#define IOTHUB_CLIENT_AUTH_EDGE_ENVIRONMENT_VERSION_1 1
typedef struct IOTHUB_CLIENT_AUTH_EDGE_ENVIRONMENT_TAG
{
    int version;
} IOTHUB_CLIENT_AUTH_EDGE_ENVIRONMENT;

#define IOTHUB_CLIENT_AUTH_FROM_PROVISIONING_VERSION_1 1
typedef struct IOTHUB_CLIENT_AUTH_FROM_PROVISIONING_TAG
{
    int version;
    const char* iothub_uri;
    const char* device_id;
} IOTHUB_CLIENT_AUTH_FROM_PROVISIONING;

#define IOTHUB_CLIENT_AUTHENTICATION_TAG_VERSION_1 1
typedef struct IOTHUB_CLIENT_AUTHENTICATION_TAG
{
    int version;
    
    IOTHUB_CLIENT_AUTH_TYPE authenticationType;

    union 
    {
        IOTHUB_CLIENT_AUTH_CONNECTION_STRING connectionString;
        IOTHUB_CLIENT_AUTH_X509_THUMBPRINT x509Thumbprint;
        IOTHUB_CLIENT_AUTH_X509_CA_CERTIFICATE caCertificate;
        IOTHUB_CLIENT_AUTH_FOR_MULTIPLEXED_CONNECTION multiplexedConnection;
        IOTHUB_CLIENT_AUTH_EDGE_ENVIRONMENT edgeEnvironment;
        IOTHUB_CLIENT_AUTH_FROM_PROVISIONING fromProvisioning;
    } auth;
} IOTHUB_CLIENT_AUTHENTICATION;


#endif /* IOTHUB_CLIENT_CREATE_H */

