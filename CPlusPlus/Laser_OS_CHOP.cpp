/*
* Copyright 2020 - LaserDock Limited
* Written by Tim Greiser - Prim8 Technology Consulting, LLC
* Contains related libraries from Derivative Inc. & Laser Dock Limited
*/
#include "Laser_OS_CHOP.h"

#include "pch.h"

#include <laserdocklib/LaserdockDevice.h>
#include <laserdocklib/LaserdockDeviceManager.h>
#include <laserdocklib/LaserdockSample.h>

using namespace std;

typedef bool (LaserdockDevice::* ReadMethodPtr)(uint32_t*);

LaserdockSample* samples;
uint16_t _max;

void print_string(string str) {

	std::cout << str << endl;

}

void print_uint32(string name, LaserdockDevice* d, ReadMethodPtr method) {

	uint32_t n = 0;
	bool pass = (d->*method)(&n);
	if (!pass) {
		print_string("Failed to read " + name);
		return;
	}
	std::cout << name << ": " << n << endl;

}

Laser_OS_CHOP::Laser_OS_CHOP(const OP_NodeInfo* info, uint16_t maxSamples) {
}

Laser_OS_CHOP::~Laser_OS_CHOP() {
	print_string("Shutting down");
}

void
Laser_OS_CHOP::connect(int id, double pointRate) {
	std::cout << "connected " << id << endl;
	std::vector<std::unique_ptr<LaserdockDevice> > devices = LaserdockDeviceManager::getInstance().get_laserdock_devices();
	if (devices.size() > id) {
		device = devices[id].release();
	}
	if (!device) {
		std::cout << "Device not found!";
	}
	else {
		std::cout << "Initialized.\n";
	}

	print_uint32("Firmware major version", device, &LaserdockDevice::version_major_number);
	print_uint32("Firmware minor version", device, &LaserdockDevice::version_minor_number);
	print_uint32("Max Dac Rate", device, &LaserdockDevice::max_dac_rate);
	print_uint32("Min Dac Value", device, &LaserdockDevice::min_dac_value);
	print_uint32("Max Dac Value", device, &LaserdockDevice::max_dac_value);
	device->set_dac_rate(pointRate);
	print_uint32("Current Dac Rate", device, &LaserdockDevice::dac_rate);

	bool isClearRingbuffer = device->clear_ringbuffer();
	if (isClearRingbuffer) {
		print_string("Clearing ringbuffer");
	}
	else {
		print_string("Clearing ringbuffer not ok");
	}

	bool enabled = false;

	if (!device->enable_output()) {
		print_string("Failed enabling output state");
	}

	if (!device->get_output(&enabled)) {
		print_string("Failed reading output state");
	}
	else {
		if (!enabled) {
			print_string("Could not initialize device for output.");
		}
	}

}

void
Laser_OS_CHOP::disconnect() {
	if (device) {
		device->disable_output();
		bool isClearRingbuffer = device->clear_ringbuffer();
		device = NULL;
	}
}

void
Laser_OS_CHOP::getGeneralInfo(CHOP_GeneralInfo* ginfo, const OP_Inputs* inputs, void* reserved1) {
	ginfo->cookEveryFrame = true;
	ginfo->timeslice = false;
	ginfo->inputMatchIndex = 0;
}

bool
Laser_OS_CHOP::getOutputInfo(CHOP_OutputInfo* info, const OP_Inputs* inputs, void* reserved1) {
	if (inputs->getNumInputs() > 0) {
		return false;
	}
	return true;
}

void
Laser_OS_CHOP::getChannelName(int32_t index, OP_String* name, const OP_Inputs* inputs, void* reserved1)
{
	// unless getOutputInfo sets up channels, this won't be called.
	name->setString("chan1");
}

int32_t
Laser_OS_CHOP::getNumInfoCHOPChans(void* reserved1) {
	return 0;
}

void
Laser_OS_CHOP::getInfoCHOPChan(int32_t index,
	OP_InfoCHOPChan* chan,
	void* reserved1) {

}
bool
Laser_OS_CHOP::getInfoDATSize(OP_InfoDATSize* infoSize, void* reserved1)
{
	infoSize->rows = 2;
	infoSize->cols = 2;
	infoSize->byColumn = false;
	return true;
}

void
Laser_OS_CHOP::getInfoDATEntries(int32_t index,
	int32_t nEntries,
	OP_InfoDATEntries* entries,
	void* reserved1)
{

	char tempBuffer[4096];

	if (index == 0)
	{
		// Set the value for the first column
		entries->values[0]->setString("executeCount");

		// Set the value for the second column
#ifdef _WIN32
		sprintf_s(tempBuffer, "%d", myExecuteCount);
#else // macOS
		snprintf(tempBuffer, sizeof(tempBuffer), "%d", myExecuteCount);
#endif
		entries->values[1]->setString(tempBuffer);
	}

}

void
Laser_OS_CHOP::setupParameters(OP_ParameterManager* manager, void* reserved1)
{
	// enable
	{
		OP_NumericParameter	np;

		np.name = "Enable";
		np.label = "Enable Laser Output";

		OP_ParAppendResult res = manager->appendToggle(np);
		assert(res == OP_ParAppendResult::Success);
	}
	// device
	{
		print_string("get_devices");
		std::vector<std::unique_ptr<LaserdockDevice> > devices = LaserdockDeviceManager::getInstance().get_laserdock_devices();
		OP_StringParameter	sp;

		sp.name = "Device";
		sp.label = "Device";

		size_t numDevices = devices.size();
		std::vector<const char*> names;
		std::vector<const char*> labels;

		std::vector<std::string> names_strs;
		std::vector<std::string> labels_strs;

		names_strs.push_back("None");
		labels_strs.push_back("");
		
		int iX = 1;
		for (const std::unique_ptr<LaserdockDevice>& device : devices) {
			int dsize = 64;
			char* d = (char*) malloc(sizeof(char) * dsize);
			int dc = device->sdescription(d, dsize);
			printf("%s\n", d);
			int nsize = 16;
			char* name = (char*) malloc(sizeof(char) * nsize);
			sprintf_s(name, nsize, "Wicked%d", iX);
			names_strs.push_back(name);
			labels_strs.push_back(d);
			free(d);
		}

		// Convert to a vector of c-style strings
		for (const auto& string : names_strs) names.push_back(string.c_str());
		for (const auto& string : labels_strs) labels.push_back(string.c_str());

		sp.defaultValue = names[0];
		OP_ParAppendResult res = manager->appendMenu(sp, names.size(), names.data(), labels.data());
		assert(res == OP_ParAppendResult::Success);
	}
	// scaleX
	{
		OP_NumericParameter	np;

		np.name = "Scalex";
		np.label = "Scale X";
		np.defaultValues[0] = 0.5;
		np.minSliders[0] = -0.0;
		np.maxSliders[0] =  2.0;
		
		OP_ParAppendResult res = manager->appendFloat(np);
		assert(res == OP_ParAppendResult::Success);
	}
	// scaleY
	{
		OP_NumericParameter	np;

		np.name = "Scaley";
		np.label = "Scale Y";
		np.defaultValues[0] = 0.5;
		np.minSliders[0] = -0.0;
		np.maxSliders[0] = 2.0;

		OP_ParAppendResult res = manager->appendFloat(np);
		assert(res == OP_ParAppendResult::Success);
	}
	// red
	{
		OP_NumericParameter	np;

		np.name = "Red";
		np.label = "Red Level";
		np.defaultValues[0] = 1.0;
		np.minSliders[0] = 0.0;
		np.maxSliders[0] = 1.0;

		OP_ParAppendResult res = manager->appendFloat(np);
		assert(res == OP_ParAppendResult::Success);
	}
	// green
	{
		OP_NumericParameter	np;

		np.name = "Green";
		np.label = "Green Level";
		np.defaultValues[0] = 1.0;
		np.minSliders[0] = 0.0;
		np.maxSliders[0] = 1.0;

		OP_ParAppendResult res = manager->appendFloat(np);
		assert(res == OP_ParAppendResult::Success);
	}
	// blue
	{
		OP_NumericParameter	np;

		np.name = "Blue";
		np.label = "Blue Level";
		np.defaultValues[0] = 1.0;
		np.minSliders[0] = 0.0;
		np.maxSliders[0] = 1.0;

		OP_ParAppendResult res = manager->appendFloat(np);
		assert(res == OP_ParAppendResult::Success);
	}
	// intensity
	{
		OP_NumericParameter	np;

		np.name = "Intensity";
		np.label = "Intensity";
		np.defaultValues[0] = 1.0;
		np.minSliders[0] = 0.0;
		np.maxSliders[0] = 1.0;

		OP_ParAppendResult res = manager->appendFloat(np);
		assert(res == OP_ParAppendResult::Success);
	}

	print_string("setupParameters");
}

int connectedDev = 0;
void Laser_OS_CHOP::execute(CHOP_Output* output, const OP_Inputs* inputs, void* reserved) {
	bool enabled = inputs->getParInt("Enable");
	int dev = inputs->getParInt("Device");
	if (dev != connectedDev) {
		device = NULL;
	}
	if (dev > 0 && inputs->getNumInputs() > 0 && enabled) {
		
		const OP_CHOPInput* in = inputs->getInputCHOP(0);
		if (device == NULL) {
			print_string("Initializing");
			
			samples = (LaserdockSample*)calloc(sizeof(LaserdockSample), in->numSamples);
			_max = in->numSamples;
			std::cout << "Initializing - max samples " << _max << endl;

			print_string("enabled");
			connect(dev - 1, in->sampleRate);
			connectedDev = dev;
		}
		//print_uint32("Ring count", device, &LaserdockDevice::ringbuffer_sample_count);
		//print_uint32("Ring empty count", device, &LaserdockDevice::ringbuffer_empty_sample_count);
		double			scalex = inputs->getParDouble("Scalex");
		double			scaley = inputs->getParDouble("Scaley");
		double		 intensity = inputs->getParDouble("Intensity");

		CHOP_Buffer* cbuffer = new CHOP_Buffer(_max);
		cbuffer->fillSamples(in, output, samples, scalex, scaley, inputs->getParDouble("Red") * intensity, inputs->getParDouble("Green") * intensity,
			inputs->getParDouble("Blue") * intensity);
		device->send_samples(samples, _max);
	} else if (!enabled) {
		disconnect();
	}
}

