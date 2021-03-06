#include "stdio.h"

#include <fstream>
#include <iostream>
using namespace std;

#include "statis.h"
#include "service.h"
#include "global.h"

Statis::Statis(const char* file) :m_blocked_(0), m_succeed_(0), m_blocked_BW_(0)
{
	strcpy_s(m_file_, file);
	of.open(m_file_);
	if (!of.is_open())
	{
		perror("Error in open Statis File\n");
	}
}

void Statis::init()
{
	m_blocked_ = 0;
	m_blocked_BW_ = 0;
	m_succeed_ = 0;
}

Statis::~Statis()
{
	of.close();
}

void CommonStatis::BlockOccur(Service& service)
{
	++m_blocked_;
	m_blocked_BW_ += service.get_bandwidth();
}

void CommonStatis::Output(int traffic_load)
{
	Output2Ostream(cout, traffic_load);
	Output2Ostream(of, traffic_load);
}

void CommonStatis::Output2Ostream(ostream& os, int traffic_load)
{
	double block_prob = (double)m_blocked_ / (double)simu_global::kServiceNum;
	/*
	char title[100];
	memset(title, 0, sizeof(title));
	sprintf_s(title, "%s\t%s\n", "TrafficLoad", "BlockProbility");
	os << title;
	*/
	char data[100];
	memset(data, 0, sizeof(data));
	sprintf_s(data, "%-3d\t%.4f\n", traffic_load, block_prob);
	os << data;

}

void CommonStatis::init()
{
	Statis::init();
}