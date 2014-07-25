#ifndef SERVICE_H
#define SERVICE_H

#include <vector>
using std::vector;

class Service
{
public:
	/*
	Service(int ID, double arrival_time, double hold_time, int src, int dst, int bandwidth) :
		m_ID_(ID), m_arrival_time_(arrival_time), m_hold_time_(hold_time), m_src_(src),
		m_dst_(dst), m_bandwith_(bandwidth)
	{};
	*/
	Service(int ID, int arrival_time, int deadline_time, int src, int dst, int capacity) :
		m_ID_(ID), m_arrival_time_(arrival_time), m_deadline_time_(deadline_time)
		, m_src_(src), m_dst_(dst), m_capacity_(capacity)
	{};
	void set_arrival_hold_time(const double arrival_time, const double hold_time)
	{
		m_arrival_time_ = arrival_time;
		m_hold_time_ = hold_time;
	};
	int get_arrival_time()
	{
		return m_arrival_time_;
	}
	int get_deadline_time()
	{
		return m_deadline_time_;
	}
	void set_start_end_time(int start, int end)
	{
		m_start_time_ = start;
		m_end_time_ = end;
	}
	void set_src_dst(const int src, const int dst)
	{
		m_src_ = src;
		m_dst_ = dst;
	};
	void get_src_dst(int& src, int& dst)
	{
		src = m_src_;
		dst = m_dst_;
	};

	void set_routing_path(const vector<int>& path_list)
	{
		m_routing_path_ = path_list;
	};
	vector<int> get_routing_path()
	{
		return m_routing_path_;
	}
	void set_start_end_slot(const int start, const int end)
	{
		m_start_slot_index_ = start;
		m_end_slot_index_ = end;
	};
	void get_start_end_slot(int& start, int& end) //output slot and end slot
	{
		start = m_start_slot_index_;
		end = m_end_slot_index_;
	};
	int get_bandwidth()
	{
		return m_bandwith_;
	};
	void set_bandwidth(int slot)
	{
		m_bandwith_ = slot;
	}
	int get_capacity()
	{
		return m_capacity_;
	};
	int get_ID()
	{
		return m_ID_;
	};
	void get_start_end_time(int& start, int& end)
	{
		start = m_start_time_;
		end = m_end_time_;
	}
private:
	int m_ID_;
	int m_arrival_time_;
	int m_hold_time_;
	int m_src_;
	int m_dst_;
	int m_bandwith_;
	int m_capacity_;
	int m_modulation_;
	int m_start_slot_index_;
	int m_end_slot_index_;
	vector<int> m_routing_path_;
	int m_start_time_;
	int m_end_time_;
	int m_deadline_time_;
};

#endif

