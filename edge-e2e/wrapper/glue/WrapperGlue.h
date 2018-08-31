// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once
#include <iostream>
#include <string>
#include <map>

class WrapperGlue {
public:
	WrapperGlue();
    ~WrapperGlue();

	void CleanupResources();
	void PrintMessage(std::string message);

};