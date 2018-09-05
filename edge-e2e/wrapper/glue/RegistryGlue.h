// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once
#include <iostream>
#include <string>
#include <map>

using namespace std;

class RegistryGlue {
public:
    RegistryGlue();
    ~RegistryGlue();

    string Connect(string connectionString);
    void Disconnect(string connectionId);
    string GetModuleTwin(string connectionId, string deviceId, string moduleId);
    void PatchModuleTwin(string connectionId, string deviceId, string moduleId, string patch);

    void CleanupResources();

private:
    std::map<string, void*> clientMap;
    std::map<string, void*> twinMap;

};