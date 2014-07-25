#ifndef STATIS_H
#define STATIS_H

#include "string.h"

#include <iostream>
using namespace std;

class Service;

class Statis
{
public:
	Statis(char* file) :m_blocked_(0), m_succeed_(0), m_blocked_BW_(0)
	{
		strcpy_s(m_file_, file);
	};
	virtual void init() = 0;
	virtual void BlockOccur(Service& service) = 0;
	virtual void Output(char* file, int traffic_load) = 0;
	virtual ~Statis() = 0;
protected:
	int m_blocked_;//阻塞数量	
	int m_succeed_;//成功数量
	int m_blocked_BW_;//阻塞带宽
	char m_file_[20];
};

class CommonStatis : public Statis
{
public:
	CommonStatis(char* file) :Statis(file)
	{
	};
	void init();
	virtual void BlockOccur(Service& service);
	virtual void Output(int traffic_load);
	void Output2Ostream(ostream& os, int traffic_load);
private:

};

#endif