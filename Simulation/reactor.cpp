#include "string.h" //for memset()

#include <cassert>
#include <iostream>
using namespace std;

#include "reactor.h"
#include "topology.h"
#include "global.h"
#include "generator.h"
#include "reactor.h"
#include "Routing.h"
#include "resource.h"
#include "statis.h"
#include "service.h"
#include "strategy.h"

Reactor::Reactor()
{
}

void Reactor::run(int start_rou, int end_rou, int step)
{
	//构建拓扑
	Topology topo;
	if (topo.ReadFile(simu_global::kFile))
	{
		//路由初始化
		Routing *routing = new KRouting(&topo);
		routing->init();
		Statis *statis = new CommonStatis(const_cast<char*>(simu_global::KOutputFile));
		for (int rou = start_rou; rou <= end_rou; rou += step)
		{
			//完成资源初始化
			Resource *resource = Resource::getInstance();
			resource->init(topo);
			Strategy *strategy = new LeasTime(resource);
			statis->init();
			Generator generator;
			generator.GenerateServiceQ(rou);
			assert(!generator.m_service_queue_.empty());
			//assert(generator.m_service_queue_.size() ==
			//	2 * generator.m_ID_service_.size());
			set<ServiceTimeIDIndex>::iterator iter = 
				generator.m_service_queue_.begin();
			for (; iter != generator.m_service_queue_.end(); ++iter)
			{
				Service *service = generator.m_ID_service_[iter->get_ID()];
				//对每个service处理，路由，资源分配
				if (iter->get_type() == Type::kSetup)
				{
					vector< vector<int> > routing_pathlist;
					routing->CalculatePath(*service, routing_pathlist);
					MinMax time_slot;
					if (strategy->SelectPathResource(routing_pathlist, 
						*service, time_slot))
					{
						resource->DirectAllocate(*service, time_slot);
					}
					else
					{
						//分配失败，处理TODO:haoranc
						generator.MarkIgnoredID(*service);
						statis->BlockOccur(*service);
						cout << "Block " << service->get_ID() << endl;
					}
				}
				else if (iter->get_type() == Type::kTeardown)
				{
					if (!generator.IsIgnored(*service))
					{
						resource->TearDownSpectrum(*service);
						//cout << "Teardown" << service->get_ID() << endl;
					}
				}
			}
			//每个traffic load做一次统计
			//CommonStatis *comon_statis = (CommonStatis *)statis;
			statis->Output(rou);
			delete(strategy);
		}
	}
}
