#include <cassert>
#include <vector>
using namespace std;

#include "resource.h"
#include "topology.h"
#include "service.h"

Resource* Resource::m_resource_ = NULL;

void Resource::init(const Topology& topo)
{
	for (int i = 0; i < topo.m_adj_matrix_.size(); ++i)
	{
		for (int j = 0; j < topo.m_adj_matrix_[i].size(); ++j)
		{
			if (topo.m_adj_matrix_[i][j] >0 && 
				topo.m_adj_matrix_[i][j] < simu_global::kInfinity)
			{
				vector< bitset<kSpectrumNum> > time_spectrum;
				for (int k = 0; k < simu_global::kTotalTime; ++k)
				{
					bitset<kSpectrumNum> spectrum;
					spectrum.set();
					time_spectrum.push_back(spectrum);
				}
				m_resource_matrix_[make_pair(i, j)] = time_spectrum;
			}
		}
	}
}

/*
两个部分：1）将Resource资源池里的资源分配。2）将service里占用的资源设置。
*/
bool Resource::AllocateSpectrum(Service& service, int allocate_stratagy)
{
	return true;
}

void Resource::TearDownSpectrum(Service& service)
{
	vector<int>& routing_path = service.get_routing_path();
	assert(!routing_path.empty());
	int start_slot(0), end_slot(0);
	service.get_start_end_slot(start_slot, end_slot);
	int start_time(0), end_time(0);
	service.get_start_end_time(start_time, end_time);
	for (int i = 0; i < routing_path.size() - 1; ++i)
	{
			for (int k = start_time; k <= end_time; ++k)
			{
				for (int j = start_slot; j <= end_slot; ++j)
				{
					m_resource_matrix_[make_pair(routing_path[i],
						routing_path[i + 1])][k].set(j);
				}
			}
	}
}

void Resource::DirectAllocate(Service& service, MinMax& min_max)
{
	vector<int>& routing_path = service.get_routing_path();
	int start_time = min_max.first.first;
	int end_time = min_max.first.second;
	int start_slot = min_max.second.first;
	int end_slot = min_max.second.second;
	for (int j = 0; j < routing_path.size() - 1; ++j)
	{
		for (int k = start_time; k <= end_time ; ++k)
		{
			for (int i = start_slot; i <= end_slot+simu_global::kGB; i++)
			{
				m_resource_matrix_[make_pair(routing_path[j],
					routing_path[j + 1])][k].reset(i);
			}
		}
	}
	service.set_start_end_slot(start_slot, end_slot+simu_global::kGB);
	service.set_start_end_time(start_time, end_time);
	service.set_bandwidth(end_slot - start_slot + 1);
}

Resource::Resource()
{
}


Resource::~Resource()
{
}
