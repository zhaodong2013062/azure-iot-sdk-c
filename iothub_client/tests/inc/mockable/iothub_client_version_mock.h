// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

/** @file iothub_client_version.h
*	@brief Functions for managing the client SDK version.
*/

#ifndef IOTHUB_CLIENT_VERSION_MOCK_H
#define IOTHUB_CLIENT_VERSION_MOCK_H

#include "iothub_client_version.h"
#include "azure_c_shared_utility/umock_c_prod.h"

#ifdef __cplusplus
extern "C"
{
#endif

    MOCKABLE_FUNCTION(, const char*, IoTHubClient_GetVersionString);

#ifdef __cplusplus
}
#endif

#endif // IOTHUB_CLIENT_VERSION_MOCK_H