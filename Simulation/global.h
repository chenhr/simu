#ifndef GLOBAL_H
#define GLOBAL_H

#define kSpectrumNum 10
namespace simu_global
{
	extern const int kInfinity;
	extern const int kServiceNum;
	extern const float kMu;
	//Service Bandwidth Array
	extern const int kBWArray[2];
	// Topology file
	extern const char* kFile;
	extern const char* KOutputFile;
	extern const int kAllocSpecStrategy;
	extern const int kStartRou;
	extern const int kEndRou;
	extern const int kStepRou;
	// Number of routing paths in KSP algorithm
	extern const int kKpaths;
	extern const int kTotalTime;
	// Guard Band
	extern const int kGB;
}

#endif