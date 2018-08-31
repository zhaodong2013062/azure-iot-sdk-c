// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once
#include <iostream>
#include <string>
#include <map>

using namespace std;

class ServiceGlue {
public:
	ServiceGlue();
    ~ServiceGlue();

	string Connect(string connectionString);
	void Disconnect(string connectionId);
	string InvokeDeviceMethod(string connectionId, string deviceId, string methodInvokeParameters);
	string InvokeModuleMethod(string connectionId, string deviceId, string moduleId, string methodInvokeParameters);
	
	void CleanupResources();

private: 
    string _invokeMethodCommon(string connectionId, string deviceId, string moduleId, string methodInvokeParameters);
    std::map<string, void*> clientMap;
};