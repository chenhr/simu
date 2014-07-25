#include <bitset>
#include <cmath>
using namespace std;

#include "strategy.h"
#include "service.h"
#include "global.h"
#include "resource.h"
using namespace simu_global;

#define LARGENUMBER 1000000

bool LeasTime::SelectPathResource(PathList& pathlist,
	Service& service, MinMax& result)
{
	int arrival_time = service.get_arrival_time();
	int deadline_time = service.get_deadline_time();

	MinMax minmax_result;
	int flag_time = LARGENUMBER; //一个大的值
	for (int path_index = 0; path_index < pathlist.size(); ++path_index)
	{
		//对于每一条路径做策略，比较最短时间。
		vector<int>& path = pathlist[path_index];
		bitset<kSpectrumNum> init_spectrum;
		init_spectrum.set();
		vector< bitset<kSpectrumNum> > time_spectrum(
			simu_global::kTotalTime, init_spectrum);
		//初始化time_spectrum
		for (int i = 0; i < path.size() - 1; ++i)
		{
			for (int ii = arrival_time; ii < simu_global::kTotalTime; ++ii)
			{
				time_spectrum[ii] &= m_resource_->
					m_resource_matrix_[make_pair(path[i],path[i + 1])][ii];
			}
		}
		bitset<kSpectrumNum> spectrum;
		spectrum.set();
		// int - endtime, pair<a,b>,number among [a,b] could be the start of spectrum
		// so a = start+1;b= a+count-bandwidth;
		map< int, MinMax > resource_pool;
		bool flag = true;
		for (int ii = arrival_time; ii < deadline_time; ++ii)
		{
			for (int j = ii + 1; j < deadline_time; ++j)
			{
				spectrum.set();
				int slot = ceil(service.get_capacity() / (j - ii));
				if (slot > kSpectrumNum) //超过最大频谱
				{
					continue;
				}
				for (int start = ii; start < j; ++start)
				{
					spectrum &= time_spectrum[start];
				}
				if (slot > spectrum.count()) //count计算1的个数
				{
					continue;
				}
				else
				{
					//找到资源分配的策略。
					// find if there's enough spectrum for the service and get it
					int start(0), count(0);
					
					for (int i = 0; i < kSpectrumNum; ++i)
					{
						if (spectrum.test(i)) // bit i is 1 (free)
						{
							++count;
							if (i != kSpectrumNum - 1)
							{
								continue;
							}
						}
						if (count >= slot)
						{
							int end_time = j;
							if (resource_pool.count(end_time) <= 0) //没有这个endtime作key
							{
								resource_pool[end_time] = make_pair(make_pair(ii,end_time-1),
									make_pair(start, start + slot - 1));
								flag = false;
								break;
							}
						}
						start = i + 1;
						count = 0;
					}
					if (flag != true)
					{
						flag = true;
						break;
					}
				}
			}
		}
		if (!resource_pool.empty())
		{
			map< int, MinMax >::iterator iter = resource_pool.begin();
			if (flag_time > iter->first)
			{
				flag_time = iter->first;
				minmax_result = iter->second;
				service.set_routing_path(path);
			}
		}
		
	}
	if (flag_time == LARGENUMBER)
	{
		return false;
	}
	result = minmax_result;
	return true;
}


bool MaxEffiency::SelectPathResource(PathList& pathlist,
	Service& service, MinMax& result)
{
	int arrival_time = service.get_arrival_time();
	int deadline_time = service.get_deadline_time();

	MinMax minmax_result;
	int time_by_slot = LARGENUMBER; //一个大的值
	for (int path_index = 0; path_index < pathlist.size(); ++path_index)
	{
		//对于每一条路径做策略，比较最短时间。
		vector<int>& path = pathlist[path_index];
		bitset<kSpectrumNum> init_spectrum;
		init_spectrum.set();
		vector< bitset<kSpectrumNum> > time_spectrum(
			simu_global::kTotalTime, init_spectrum);
		//初始化time_spectrum
		for (int i = 0; i < path.size() - 1; ++i)
		{
			for (int ii = arrival_time; ii < simu_global::kTotalTime; ++ii)
			{
				time_spectrum[ii] &= m_resource_->
					m_resource_matrix_[make_pair(path[i], path[i + 1])][ii];
			}
		}
		bitset<kSpectrumNum> spectrum;
		spectrum.set();
		// int - endtime, pair<a,b>,number among [a,b] could be the start of spectrum
		// so a = start+1;b= a+count-bandwidth;
		map< int, MinMax > resource_pool;
		bool flag = true;
		for (int ii = arrival_time; ii < deadline_time; ++ii)
		{
			for (int j = ii + 1; j < deadline_time; ++j)
			{
				spectrum.set();
				int slot = ceil(service.get_capacity() / (j - ii));
				if (slot > kSpectrumNum) //超过最大频谱
				{
					continue;
				}
				for (int start = ii; start < j; ++start)
				{
					spectrum &= time_spectrum[start];
				}
				if (slot > spectrum.count()) //count计算1的个数
				{
					continue;
				}
				else
				{
					//找到资源分配的策略。
					// find if there's enough spectrum for the service and get it
					int start(0), count(0);

					for (int i = 0; i < kSpectrumNum; ++i)
					{
						if (spectrum.test(i)) // bit i is 1 (free)
						{
							++count;
							if (i != kSpectrumNum - 1)
							{
								continue;
							}
						}
						if (count >= slot)
						{
							int end_time = j;
							if (resource_pool.count(end_time) <= 0) //没有这个endtime作key
							{
								resource_pool[end_time] = make_pair(make_pair(ii, end_time - 1),
									make_pair(start, start + slot - 1));
								flag = false;
								break;
							}
						}
						start = i + 1;
						count = 0;
					}
					if (flag != true)
					{
						flag = true;
						break;
					}
				}
			}
		}
		if (!resource_pool.empty())
		{
			map< int, MinMax >::iterator iter = resource_pool.begin();
			int start_time = iter->second.first.first;
			int end_time = iter->second.first.second;
			int start_slot = iter->second.second.first;
			int end_slot = iter->second.second.second;
			int this_time_by_slot = (end_time - start_time + 1)*
				(end_slot - start_slot + 1+simu_global::kGB);
			if (time_by_slot > this_time_by_slot)
			{
				time_by_slot = this_time_by_slot;
				minmax_result = iter->second;
				service.set_routing_path(path);
			}
		}

	}
	if (time_by_slot == LARGENUMBER)
	{
		return false;
	}
	result = minmax_result;
	return true;
}
