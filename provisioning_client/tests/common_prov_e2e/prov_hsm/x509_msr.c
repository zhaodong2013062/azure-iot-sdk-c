// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "azure_c_shared_utility/xlogging.h"
#include "azure_c_shared_utility/azure_base64.h"
#include "azure_c_shared_utility/envvariable.h"
#include "azure_c_shared_utility/crt_abstractions.h"

#include "x509_info.h"

#include "RIoT.h"
#include "RiotCrypt.h"
#include "RiotDerEnc.h"
#include "RiotX509Bldr.h"
#include "DiceSha256.h"

static const char* BASE64_x590_ECC_CERT = "BASE64_x590_ECC_CERT";
static const char* BASE64_x509_ECC_KEY = "BASE64_x509_ECC_KEY";
static const char* CERT_COMMON_NAME = "CERT_COMMON_NAME";

typedef struct X509_CERT_INFO_TAG
{
    const char* common_name;
    char* x509_cert;
    char* x509_key;
} X509_CERT_INFO;

static char* retrieve_base64_value(const char* env_var_value)
{
    char* result;
    BUFFER_HANDLE buff_handle;
    const char* base64_value = environment_get_variable(env_var_value);
    if (base64_value == NULL)
    {
        LogError("Failure retrieving environment variable %s", env_var_value);
        result = NULL;
    }
    else if ((buff_handle = Azure_Base64_Decode(base64_value)) == NULL)
    {
        LogError("Failure decoding environment variable %s", env_var_value);
        result = NULL;
    }
    else
    {
        if ((result = malloc(BUFFER_length(buff_handle)+1)) == NULL)
        {
            LogError("Failure allocating mem for environment variable %s", env_var_value);
        }
        else
        {
            memset(result, 0, BUFFER_length(buff_handle)+1);
            memcpy(result, BUFFER_u_char(buff_handle), BUFFER_length(buff_handle));
        }
        BUFFER_delete(buff_handle);
    }
    return result;
}

X509_INFO_HANDLE x509_info_create()
{
    X509_CERT_INFO* result;

    result = malloc(sizeof(X509_CERT_INFO));
    if (result == NULL)
    {
        /* Codes_SRS_HSM_CLIENT_TPM_07_001: [ If any failure is encountered hsm_client_tpm_create shall return NULL ] */
        printf("Failure: malloc X509_INFO.");
    }
    else
    {
        memset(result, 0, sizeof(X509_CERT_INFO));
        if ((result->common_name = environment_get_variable(CERT_COMMON_NAME)) == NULL)
        {
            LogError("Failure retrieving x509 common name");
            free(result);
            result = NULL;
        }
        else if ((result->x509_cert = retrieve_base64_value(BASE64_x590_ECC_CERT)) == NULL)
        {
            LogError("Failure retrieving x509 ecc certificate");
            free(result);
            result = NULL;
        }
        else if ((result->x509_key = retrieve_base64_value(BASE64_x590_ECC_CERT)) == NULL)
        {
            LogError("Failure retrieving x509 ecc key");
            free(result->x509_cert);
            free(result);
            result = NULL;
        }
    }
    return result;
}

void x509_info_destroy(X509_INFO_HANDLE handle)
{
    if (handle != NULL)
    {
        free(handle->x509_cert);
        free(handle->x509_key);
        free(handle);
    }
}

const char* x509_info_get_cert(X509_INFO_HANDLE handle)
{
    const char* result;
    if (handle == NULL)
    {
        LogError("Invalid handle value specified");
        result = NULL;
    }
    else
    {
        result = handle->x509_cert;
    }
    return result;
}

const char* x509_info_get_key(X509_INFO_HANDLE handle)
{
    const char* result;
    if (handle == NULL)
    {
        LogError("Invalid handle value specified");
        result = NULL;
    }
    else
    {
        result = handle->x509_key;
    }
    return result;
}

const char* x509_info_get_cn(X509_INFO_HANDLE handle)
{
    const char* result;
    if (handle == NULL)
    {
        LogError("Invalid handle value specified");
        result = NULL;
    }
    else
    {
        result = handle->common_name;
    }
    return result;
}
