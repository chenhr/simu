#ifndef GENERATOR_H
#define GENERATOR_H

#include <map>
#include <set>
#include <vector>
using namespace std;

enum Type
{
	kSetup,
	kTeardown
};

class ServiceTimeIDIndex
{
public:
	ServiceTimeIDIndex(double time, int ID, int type) :m_time_(time),
		m_ID_(ID), m_type_(type)
	{};
	bool operator< (const ServiceTimeIDIndex& servTimeIDIndex) const
	{
		return m_time_ < servTimeIDIndex.m_time_;
	};
	const int get_type() const
	{
		return m_type_;
	};
	const int get_ID() const
	{
		return m_ID_;
	};
private:
	double m_time_;
	int m_ID_;
	int m_type_;

};
class Service;

class Generator
{
public:
	Generator();
	void GenerateServiceQ(const int rou);
	void MarkIgnoredID(Service& service);
	bool IsIgnored(Service& service);
	~Generator();
	set<ServiceTimeIDIndex> m_service_queue_;//service Queue
	map<int, Service*> m_ID_service_;//query ID get service
private: 
	double GenerateTime(double beta);
	void GenerateSrcDst(int& src, int& dst);//output src and dst node;
	int GenerateBW();
	//由于没建路成功，对应的拆路应该跳过，此ID记录需要跳过的拆路;
	vector<char> m_ignor_ID_;
	
	

};

#endif

