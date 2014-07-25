#ifndef STRATEGY_H
#define STRATEGY_H

#include "Routing.h"

typedef pair<int, int> MinMaxSlot;
typedef pair<int, int> MinMaxTime;
typedef pair<MinMaxTime, MinMaxSlot> MinMax;

class Service;
class Resource;

class Strategy
{
public:
	Strategy(Resource *resource) :m_resource_(resource){};
	// result is the output. this function will write the service's routing_path.
	virtual bool SelectPathResource(PathList& pathlist, Service& service, MinMax& result) = 0;
	virtual ~Strategy(){};
protected:
	Resource *m_resource_;
};

class LeasTime :public Strategy
{
public:
	LeasTime(Resource *resource) :Strategy(resource){};
	virtual bool SelectPathResource(PathList& pathlist, Service& service, MinMax& result);
};

class MinSlot :public Strategy
{
public:
	MinSlot(Resource *resource) :Strategy(resource){};
	virtual bool SelectPathResource(PathList& pathlist, Service& service, MinMax& result);
};

class MaxEffiency :public Strategy
{
public:
	MaxEffiency(Resource *resource) :Strategy(resource){};
	virtual bool SelectPathResource(PathList& pathlist, Service& service, MinMax& result);
};

#endif

