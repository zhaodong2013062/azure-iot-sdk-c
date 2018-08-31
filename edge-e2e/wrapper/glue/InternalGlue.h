// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once
#include <iostream>
#include <string>
#include <map>

using namespace std;

class InternalGlue {
public:
	InternalGlue();
	~InternalGlue();

	string Connect(string transportType, string connectionString, string caCertificate);
	string ConnectFromEnvironment(string transportType);
	void Disconnect(string connectionId);
	void EnableInputMessages(string connectionId);
	void EnableMethods(string connectionId);
	void EnableTwin(string connectionId);
	void SendEvent(string connectionId, string eventBody);
	void SendOutputEvent(string connectionId, string outputName, string eventBody);
	string WaitForInputMessage(string connectionId, string inputName);
	void RoundTripMethodCall(string connectionId, string methodName, string requestAndResponse);
	string InvokeModuleMethod(string connectionId, string deviceId, string moduleId, string methodInvokeParameters);
	string InvokeDeviceMethod(string connectionId, string deviceId, string methodInvokeParameters);
	string WaitForDesiredPropertyPatch(string connectionId);
	string GetTwin(string connectionId);
	void SendTwinPatch(string connectionId, string props);
	void CleanupResources();

private:
    virtual string getNextClientId() = 0;
	std::map<string, void*> clientMap;
	std::map<string, void*> twinMap;
};