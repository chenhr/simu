/************************************************************************/
/*Author: Chen Haoran                                                   */
/************************************************************************/

#ifndef RESOURCE_H
#define RESOURCE_H

#include <bitset>
#include <map>
#include <vector>
using namespace std;

#include "global.h"
#include "strategy.h"
//#define NULL 0


class Topology;
class Service;

class Resource
{
public:
	enum AllocateStragety
	{
		kFirstFit,
		kRandomFit,
		kMiddleFit
	};
	void init(const Topology& topo);
	static Resource* getInstance()
	{
		if (m_resource_ == NULL)
		{
			m_resource_ = new Resource();
		}
		return m_resource_;
	};
	//return true if there's enough spectrum. Otherwise false.
	bool AllocateSpectrum(Service& service, int allocate_stratagy);
	void DirectAllocate(Service& service, MinMax& min_max);
	void TearDownSpectrum(Service& service);
	~Resource();
	//bitset: bit 1 means free, bit 0 means occupied .vector means time
	map< pair<int, int>, vector< bitset<kSpectrumNum> > > m_resource_matrix_;
private:
	static Resource* m_resource_;
	Resource();
	
	
};

#endif // RESOURCE_H

