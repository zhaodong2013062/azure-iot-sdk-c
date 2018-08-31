// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once
#include <iostream>
#include <string>
#include <map>
#include "InternalGlue.h"

using namespace std;

class ModuleGlue : public InternalGlue {
public:
	ModuleGlue();
	~ModuleGlue();

private:
    virtual string getNextClientId();
};