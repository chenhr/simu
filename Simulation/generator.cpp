#include <cstdlib>
#include <iostream>
using namespace std;

#include "generator.h"
#include "global.h"
#include "service.h"
#include "topology.h"

Generator::Generator()
{
	m_ignor_ID_.resize(simu_global::kServiceNum, 0);
}

void Generator::GenerateServiceQ(const int rou) // input is the traffic load
{
	double arrival_time = 0.0;
	for (int i = 0; i < simu_global::kServiceNum; ++i)
	{
		int src, dst, bw;
		GenerateSrcDst(src, dst);
		bw = GenerateBW();
		//float hold_time = GenerateTime(simu_global::kMu);
		//生成 deadline_time 5-10
		float deadline_time = arrival_time + (rand() % 6 +5);
		Service *service = new Service(i, arrival_time, deadline_time, src,
			dst, bw);
		m_ID_service_.insert(make_pair(i,service));
		ServiceTimeIDIndex serv_time_id_setup(arrival_time, i, Type::kSetup);
		//不用拆路
		//ServiceTimeIDIndex serv_time_id_teardown(arrival_time+hold_time,
		//	i, Type::kTeardown);

		
		if (!m_service_queue_.insert(serv_time_id_setup).second)
		{
			cerr << i<<" SetUP insert error" << endl;
		}

		/*
		if (!m_service_queue_.insert(serv_time_id_teardown).second)
		{
			cerr << i<<"Teardown insert error" << endl;
		}
		*/
		
		arrival_time += GenerateTime(simu_global::kMu*rou);
	}
}

double Generator::GenerateTime(double beta)
{
	double u, x;
	do
	{
		u = (double)rand() / RAND_MAX;
	} while (u == 0 || u == 1);

	x = -(1 / beta) * log(u);
	return x;
}

void Generator::GenerateSrcDst(int& src, int& dst)
{
	int src_node = rand() % Topology::get_node_num();
	int dst_node = 0;
	do
	{
		dst_node = rand() % Topology::get_node_num();
	} while (src_node == dst_node);
	src = src_node;
	dst = dst_node;
}

int Generator::GenerateBW()
{
	int size = sizeof(simu_global::kBWArray)/sizeof(int);
	return simu_global::kBWArray[rand() % size];
}

Generator::~Generator()
{
	map<int, Service*>::iterator iter = m_ID_service_.begin();
	for (; iter != m_ID_service_.end(); ++iter)
	{
		delete(iter->second);
	}
}

void Generator::MarkIgnoredID(Service& service)
{
	m_ignor_ID_[service.get_ID()] = 1;
}

bool Generator::IsIgnored(Service& service)
{
	if (m_ignor_ID_[service.get_ID()] == 1)
	{
		return true;
	}
	return false;
}