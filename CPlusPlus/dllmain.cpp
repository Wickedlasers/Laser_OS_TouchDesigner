/*
* Copyright 2020 - LaserDock Limited
* Written by Tim Greiser - Prim8 Technology Consulting, LLC
* Contains related libraries from Derivative Inc. & Laser Dock Limited
*/
#include "pch.h"

using namespace std;

typedef bool (LaserdockDevice::* ReadMethodPtr)(uint32_t*);

extern "C" {

	DLLEXPORT 
	void FillCHOPPluginInfo(CHOP_PluginInfo* info) {
		info->apiVersion = CHOPCPlusPlusAPIVersion;
		info->customOPInfo.opType->setString("Laseros");
		info->customOPInfo.opLabel->setString("Laser OS");
		info->customOPInfo.authorName->setString("Tim Greiser");
		info->customOPInfo.authorEmail->setString("tim@prim8.net");
		info->customOPInfo.minInputs = 0;
		info->customOPInfo.maxInputs = 1;
	}

	DLLEXPORT
	// Initialize the CHOP.
	CHOP_CPlusPlusBase* CreateCHOPInstance(const OP_NodeInfo* info) {
		// calculate number of samples
		uint16_t maxSamples = 500;
		return new Laser_OS_CHOP(info, maxSamples);
	}

	DLLEXPORT
	void DestroyCHOPInstance(CHOP_CPlusPlusBase* instance) {
		delete (Laser_OS_CHOP*)instance;
	}

}