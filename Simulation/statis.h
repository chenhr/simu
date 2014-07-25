#ifndef STATIS_H
#define STATIS_H

#include <fstream>
using namespace std;

#include "string.h"

class Service;

class Statis
{
public:
	Statis(const char* file_name);
	virtual void init();
	virtual void BlockOccur(Service& service)=0;
	virtual void Output(int traffic_load) = 0;
	virtual ~Statis() = 0;
protected:
	int m_blocked_;//��������	
	int m_succeed_;//�ɹ�����
	int m_blocked_BW_;//��������
	char m_file_[40];
	ofstream of;
};

class CommonStatis : public Statis
{
public:
	CommonStatis(const char* file) :Statis(file)
	{

	};
	virtual void init();
	virtual void BlockOccur(Service& service);
	virtual void Output(int traffic_load);

private:
	void Output2Ostream(ostream& os, int traffic_load);

};

#endif