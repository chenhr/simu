#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

#include "topology.h"

int Topology::m_node_num_ = 0;

Topology::Topology()
{
}

bool Topology::ReadFile(const char* file_name)
{
	ifstream file(file_name);
	if (!file.is_open())
	{
		cerr << "file:" << file_name << " not found or read error.\n";
		return false;
	}
	string line;
	int val = 0;
	while (!file.eof())
	{
		line.clear();
		getline(file, line);
		istringstream istr(line);
		vector<int> line_vec;
		while (!istr.eof())
		{
			istr >> val;
			line_vec.push_back(val);
		}
		m_adj_matrix_.push_back(line_vec);
	}
	VerifyMatrix();
	m_node_num_ = m_adj_matrix_.size();
	//Print();
	return true;
}

void Topology::Print()
{
	for (int i = 0; i < m_adj_matrix_.size(); ++i)
	{
		for (int j = 0; j < m_adj_matrix_[i].size(); ++j)
		{
			cout << m_adj_matrix_[i][j] << ' ';
		}
		cout << '\n';
	}
}

void Topology::VerifyMatrix()
{
	for (int i = 0; i < m_adj_matrix_.size(); ++i)
	{
		assert(m_adj_matrix_.size() == m_adj_matrix_[i].size());
	}
}

int Topology::get_node_num()
{
	return m_node_num_;
}

