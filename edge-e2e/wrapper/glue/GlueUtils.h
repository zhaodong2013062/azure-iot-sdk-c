// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <string>
#include <parson.h>

using namespace std;

string getJsonString(JSON_Object* root_object, const char* dotname);
string getJsonObjectAsString(JSON_Object* root_object, const char* dotname);
void parseMethodInvokeParameters(string methodInvokeParameters, string *methodName, string *payload, unsigned int *timeout);
string makeInvokeResponse(int statusCode, string payload);
