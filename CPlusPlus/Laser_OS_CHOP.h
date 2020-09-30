#pragma once
#include "pch.h"

#include <laserdocklib/LaserdockDevice.h>

class Laser_OS_CHOP : public CHOP_CPlusPlusBase
{
public:
	Laser_OS_CHOP(const OP_NodeInfo* info, uint16_t maxSamples);
	virtual ~Laser_OS_CHOP();

	void				connect(int id, double pointRate);
	void				disconnect();

	virtual void		getGeneralInfo(CHOP_GeneralInfo*, const OP_Inputs*, void*) override;
	virtual bool		getOutputInfo(CHOP_OutputInfo*, const OP_Inputs*, void*) override;
	virtual void		getChannelName(int32_t index, OP_String* name, const OP_Inputs*, void* reserved) override;

	virtual void		execute(CHOP_Output*,
		const OP_Inputs*,
		void* reserved) override;


	virtual int32_t		getNumInfoCHOPChans(void* reserved1) override;
	virtual void		getInfoCHOPChan(int index,
		OP_InfoCHOPChan* chan,
		void* reserved1) override;

	virtual bool		getInfoDATSize(OP_InfoDATSize* infoSize, void* resereved1) override;
	virtual void		getInfoDATEntries(int32_t index,
		int32_t nEntries,
		OP_InfoDATEntries* entries,
		void* reserved1) override;

	virtual void		setupParameters(OP_ParameterManager* manager, void* reserved1) override;

private:

	const OP_NodeInfo* myNodeInfo;

	int myExecuteCount;

	// This is the DAC we are connecting to
	LaserdockDevice* device;

};