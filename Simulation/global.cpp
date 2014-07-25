#include "global.h"
#include "resource.h"
using namespace simu_global;

namespace simu_global
{
	const int kInfinity = 999;
	const int kServiceNum = 40;
	const float kMu = 0.04;
	const int kBWArray[2] = { 5, 10 };
	//const char* kFile = "map_NSFNet.data";
	const char* kFile = "map_FiveNode.data";
	const char* KOutputFile = "Result.data";
	const int kAllocSpecStrategy = 
		Resource::AllocateStragety::kFirstFit;
	const int kStartRou = 100;
	const int kEndRou = 200;
	const int kStepRou = 1000;
	const int kKpaths = 3;
	const int kTotalTime = 20;
	const int kGB = 0;
};
