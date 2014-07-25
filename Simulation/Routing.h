#ifndef ROUTING_H
#define ROUTING_H

#include <vector>
#include <map>
#include <list>
using namespace std;

class Service;
class Topology;

typedef vector< vector<int> > PathList;

class Routing
{
public:
	Routing(Topology* topo)
	{
		m_topo_ = topo;
	};
	virtual void init(){};
	// return the number of path
	virtual int CalculatePath(Service& service, PathList& path_list){ return 0; };
	virtual ~Routing(){};
protected:
	Topology* m_topo_;
};

class DRouting : public Routing
{
public:
	DRouting(Topology* topo) :Routing(topo)
	{};
	virtual int CalculatePath(Service& service, PathList& path_list);
	virtual void init();
private:
	void CalculateAllPaths();
	void SearchPath(vector<int>& prev, int v, int u);//v:src u:dst
	map< pair<int, int>, vector<int> > m_nodepair_path_;
};

/*
class KRouting : public Routing
{
public:
	KRouting(Topology* topo) : Routing(topo)
	{};
	virtual int CalculatePath(Service& service, PathList& path_list){};
	virtual void init(){};
};
*/

class Vertex;
class Edge;
typedef double PathWeight;
typedef list<int> SinglePath;
typedef vector<SinglePath> MultiplePath;

#ifdef WIN32
#include <memory>
#else
#include "boost/shared_ptr.hpp"
using namespace boost;
#endif

//KSP
class KRouting : public Routing
{
public:
	typedef vector<Vertex> Adjacency_list; // the two dimension array storing a DAG

	KRouting(Topology* topo);
	~KRouting()
	{
		//delete_adj_list(m_adj_map);
		//delete_adj_list(m_in_map);
		//delete_adj_list(m_init_adj_map);
		//delete_adj_list(m_init_in_map);
		m_adj_map.clear();
		m_in_map.clear();
		m_init_adj_map.clear();
		m_init_in_map.clear();
	};
	virtual int CalculatePath(Service& service, PathList& path_list);
	virtual void init();

private:
	int path_calculate(MultiplePath& path, int num, int source, int dest);
	void clear();

	/// Find the k shortest paths (KSP) from node 0 to N-1.									
	/// If fails, return 0, or return the real number of all the shortest paths.								
	int ksp_algorithm(MultiplePath& kpaths, unsigned k, int src, int dest);
	void CreateLinkTab();
	void dijkstra(int srcId, PathWeight* dist, int* path);
	PathWeight dijkstra(int start, int SerEnd, SinglePath& SinglePath);
	/// Default is to compute the shortest distance from node 0 to node N-1.
	/// Add a node to graph, Return the number of new node.
	unsigned AddNode(unsigned ni, int preni);
	void delete_adj_list(Adjacency_list&);
	void CalculateAllPaths();
	map< pair<int, int>, MultiplePath > m_nodepair_path_;
	size_t m_initSize;	// original size of "m_adj_map", it's fixed.
	size_t m_size;		// size of "m_adj_map", because the "m_adj_map" maybe be reallocated. 
	Adjacency_list m_adj_map;
	Adjacency_list m_init_adj_map;
	Adjacency_list m_in_map;
	Adjacency_list m_init_in_map;
};

class Vertex
{
public:
	Vertex& operator = (const Vertex& _graph);

	Vertex(const Vertex& ver)
	{
		*(this) = ver;
	}
	~Vertex()
	{
	}
private:
	friend class Edge;
	friend class KRouting;
	Vertex()
	{
		m_next.reset();
	}
	Vertex(int id, PathWeight w)
	{
		m_next.reset();
		m_id = id;
		m_weightValue = w;
	}

	int m_id;
	shared_ptr<Vertex> m_next;
	PathWeight m_weightValue;
};

class Edge
{
public:
	Edge(){}
	Edge(int id, PathWeight w)
	{
		m_id = id;
		m_metric = w;
	}
	bool operator < (const Edge& edge) const
	{
		return this->m_metric > edge.m_metric;
	}
	int m_id;
	PathWeight m_metric;
};

#endif
