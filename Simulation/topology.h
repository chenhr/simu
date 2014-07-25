#ifndef TOPOLOGY_H
#define TOPOLOGY_H

#include <vector>
using std::vector;

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(const TypeName&);             \
	void operator=(const TypeName&)

class Topology
{
public:
	Topology();
	bool ReadFile(const char* file_name);
	static int get_node_num();
	vector< vector<int> > m_adj_matrix_;

private:
	DISALLOW_COPY_AND_ASSIGN(Topology);
	void Print();
	void VerifyMatrix();
	static int m_node_num_;
	
};

#endif

