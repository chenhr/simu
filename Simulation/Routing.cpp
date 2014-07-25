#include <cassert>
#include <queue>
using namespace std;

#include "Routing.h"
#include "topology.h"
#include "global.h"
#include "service.h"

int DRouting::CalculatePath(Service& service, PathList& path_list)
{
	assert(!m_nodepair_path_.empty());
	int src, dst;
	service.get_src_dst(src, dst);
	path_list.push_back(m_nodepair_path_[make_pair(src,dst)]);
	service.set_routing_path(m_nodepair_path_[make_pair(src, dst)]);
	return 1;
}

void DRouting::init()
{
	CalculateAllPaths();
}
void DRouting::CalculateAllPaths()
{
	int size = m_topo_->m_adj_matrix_.size();
	for (int node = 0; node < size; ++node)
	{
		vector<int> s(size);    // 判断是否已存入该点到S集合中
		vector<int> dist(size);
		vector<int> prev(size);
		for (int i = 0; i < size; ++i)
		{
			dist[i] = m_topo_->m_adj_matrix_[node][i];
			s[i] = 0;     // 初始都未用过该点
			if (dist[i] == simu_global::kInfinity)
				prev[i] = 0;
			else
				prev[i] = node;
		}
		dist[node] = 0;
		s[node] = 1;

		// 依次将未放入S集合的结点中，取dist[]最小值的结点，放入结合S中
		// 一旦S包含了所有V中顶点，dist就记录了从源点到所有其他顶点之间的最短路径长度
		for (int i = 1; i < size; ++i)
		{
			int tmp = simu_global::kInfinity;
			int u = node;
			// 找出当前未使用的点j的dist[j]最小值
			for (int j = 0; j < size; ++j)
			{
				if ((!s[j]) && dist[j] < tmp)
				{
					u = j;              // u保存当前邻接点中距离最小的点的号码
					tmp = dist[j];
				}
			}
			s[u] = 1;    // 表示u点已存入S集合中

			// 更新dist
			for (int j = 0; j < size; ++j)
			if ((!s[j]) && m_topo_->m_adj_matrix_[u][j] < simu_global::kInfinity)
			{
				int newdist = dist[u] + m_topo_->m_adj_matrix_[u][j];
				if (newdist < dist[j])
				{
					dist[j] = newdist;
					prev[j] = u;
				}
			}
		}
		for (int ii = 0; ii < size; ++ii)
		{
			if (ii != node)
			{
				SearchPath(prev, node, ii);
			}
		}
	}
}

void DRouting::SearchPath(vector<int>& prev, int v, int u)
{
	int que[100];//这就是个很大的数，大于最大节点数就行
	int tot = 1;
	que[tot] = u;
	tot++;
	int tmp = prev[u];
	while (tmp != v)
	{
		que[tot] = tmp;
		tot++;
		tmp = prev[tmp];
	}
	que[tot] = v;
	vector<int> path;
	for (int i = tot; i >= 1; --i)
	{
		if (i != 1)
		{
			//cout << que[i] << " -> ";
			path.push_back(que[i]);
		}
		else
		{
			//cout << que[i] << endl;
			path.push_back(que[i]);
			m_nodepair_path_[make_pair(v,u)] = path;
		}
	}
}

//KSP算法计算多条路径
KRouting::KRouting(Topology* topo) :Routing(topo)
{
	m_size = m_initSize = m_topo_->get_node_num();
}
void KRouting::init()
{
	CalculateAllPaths();
}

int KRouting::path_calculate(MultiplePath& ServicePath, int num, int source, int dest)
{

	ServicePath.clear();

	CreateLinkTab();
	int pathNum = ksp_algorithm(ServicePath, num, source, dest);
	clear();

	if (pathNum == 0)
		return -1;	//ksp fails.
	else
		return pathNum;
}

void KRouting::CreateLinkTab()
{
	unsigned size = m_initSize;
	m_initSize = m_topo_->get_node_num();
	m_size = m_topo_->get_node_num();
	for (unsigned int i = 0; i < size; i++)
	{
		Vertex ver(i, 0);
		m_adj_map.push_back(ver);
		m_in_map.push_back(ver);
	}
	for (unsigned int i = 0; i < size; i++)
	{
		for (unsigned int j = 0; j < size; j++)
		{
			PathWeight PathWeight;
			PathWeight = m_topo_->m_adj_matrix_[i][j];
			if (PathWeight > 0 && PathWeight < simu_global::kInfinity - 1)
			{
				shared_ptr<Vertex> vertex1(new Vertex(j, PathWeight));
				shared_ptr<Vertex> vertex2(new Vertex(i, PathWeight));
				vertex1->m_next = m_adj_map[i].m_next;
				m_adj_map[i].m_next = vertex1;
				vertex2->m_next = m_in_map[j].m_next;
				m_in_map[j].m_next = vertex2;
			}
		}
	}
	m_init_in_map = m_in_map;
	m_init_adj_map = m_adj_map;
}

void KRouting::clear()
{
	//delete_adj_list(m_adj_map);
	//delete_adj_list(m_in_map);
	//delete_adj_list(m_init_adj_map);
	//delete_adj_list(m_init_in_map);
	m_adj_map.clear();
	m_in_map.clear();
	m_init_adj_map.clear();
	m_init_in_map.clear();
}

int KRouting::CalculatePath(Service& service, PathList& path_list)
{
	int src(0), dest(0);
	service.get_src_dst(src, dest);
	MultiplePath multipaths = m_nodepair_path_[make_pair(src, dest)];
	for (int i = 0; i < multipaths.size(); ++i)
	{
		SinglePath &path = multipaths[i];
		SinglePath::iterator iter = path.begin();
		vector<int> vec_path;
		while (iter != path.end())
		{
			vec_path.push_back(*iter);
			iter++;
		}
		path_list.push_back(vec_path);
	}
	return path_list.size();
}

void KRouting::CalculateAllPaths()
{
	MultiplePath multi_paths;
	for (int src = 0; src < m_topo_->get_node_num(); ++src)
	{
		for (int dest = src + 1; dest < m_topo_->get_node_num();
			++dest)
		{
			multi_paths.clear();
			path_calculate(multi_paths, simu_global::kKpaths, src, dest);
			m_nodepair_path_[make_pair(src, dest)] = multi_paths;
			m_nodepair_path_[make_pair(dest, src)] = multi_paths;
		}
	}
}

int KRouting::ksp_algorithm(MultiplePath& kpaths, unsigned k, int src, int dest)
{
	/* Initialize */
	vector<int> Path, Prime, Base;
	vector<double> Dist;
	Path.resize(m_size);
	Prime.resize(m_size);
	Base.resize(m_size);
	Dist.resize(m_size);
	for (size_t i = 0; i < m_size; i++)
	{
		Prime[i] = -1;
		Base[i] = i;
	}

	/* Find the shortest path firstly */
	dijkstra(src, &Dist[0], &Path[0]);
	if (Dist[dest] > simu_global::kInfinity - 1) return 0;
	SinglePath path;
	int j = Path[dest];
	path.push_back(dest);
	while (j >= 0)
	{
		path.push_front(j); j = Path[j];
	}
	kpaths.push_back(path); // store the shortest path

	/* Find the 2th - kth shortest paths */
	unsigned int ki = 1, kj = 1;
	while (ki < k && kj < 50)
	{
		/* Find the first node with more than a single incoming arc */
		int nh = -1;
		while (path.size())
		{
			unsigned node = path.front();
			path.pop_front();
			int count = 0;
			Vertex* vertex = &m_in_map[node];
			while (vertex->m_next != false)
			{
				count++;
				vertex = vertex->m_next.get();
				if (count > 1) break;
			}
			if (count > 1)
			{
				nh = node;
				break;
			}
		}

		if (nh == -1) break; // there is NOT an alternative path, exit!

		int ni = -1;
		/* Add the first prime node to graph */
		if (Prime[nh] < 0)
		{
			unsigned nh1 = AddNode(nh, Path[nh]);

			/* compute the minimal distance from node 0 to nh1 */
			double min_dist = simu_global::kInfinity;
			int min_node = -1;
			for (shared_ptr<Vertex> ver = m_in_map[nh1].m_next; ver != false; ver = ver->m_next)
			{
				//cout << Dist[i] << " " << m_adj_map[i][nh1] << endl; // for debug
				int id = ver->m_id;
				PathWeight wei = ver->m_weightValue;
				if (Dist[id] + wei < min_dist)
				{
					min_dist = Dist[id] + wei;
					min_node = id;
				}
			}
			Dist.push_back(min_dist);
			Path.push_back(min_node);
			Prime.push_back(-1);
			Prime[nh] = nh1;

			/* record the base node */
			unsigned basei = nh;
			while (basei != Base[basei])
				basei = Base[basei];
			Base.push_back(basei);

			if (path.size())
			{
				ni = path.front();
				path.pop_front();
			}
		}
		/*  Get node ni, it must meet it's the first node following nh in path, but its prime node ni` is NOT in graph */
		else
		{
			while (path.size())
			{
				ni = path.front(); path.pop_front();
				if (Prime[ni] < 0) break;
			}
		}

		/* Add the other prime nodes to graph */
		while (true)
		{
			unsigned ni1 = AddNode(ni, Path[ni]);
			int temp1 = Path[ni];
			int temp2 = Prime[temp1];
			if (temp2 >= 0)
			{
				PathWeight wei = Dist[ni] - Dist[temp1];
				shared_ptr<Vertex>  ver1(new Vertex(ni1, wei));
				shared_ptr<Vertex>  ver2(new Vertex(temp2, wei));
				ver1->m_next = m_adj_map[temp2].m_next;
				m_adj_map[temp2].m_next = ver1;
				ver2->m_next = m_in_map[ni1].m_next;
				m_in_map[ni1].m_next = ver2;
			}
			/* compute the minimal distance from node 0 to ni1 */
			double min_dist = (unsigned)simu_global::kInfinity;
			int min_node = -1;

			for (shared_ptr<Vertex> ver = m_in_map[ni1].m_next; ver != false; ver = ver->m_next)
			{
				int id = ver->m_id;
				PathWeight wei = ver->m_weightValue;
				if (Dist[id] + wei < min_dist)
				{
					min_dist = Dist[id] + wei;
					min_node = id;
				}
			}

			Dist.push_back(min_dist);
			Path.push_back(min_node);
			Prime.push_back(-1);
			Prime[ni] = ni1;

			/* record the base node */
			unsigned basei = ni;
			while (basei != Base[basei])
				basei = Base[basei];
			Base.push_back(basei);

			if (!path.size()) break;
			ni = path.front();
			path.pop_front();
		}

		/* get the kth shortest path */
		if (ni == -1) ni = nh; // if nh is just the SerEnd node.
		SinglePath temp;
		int j = Prime[ni];
		while (j >= 0)
		{
			path.push_front(j); temp.push_front(Base[j]); j = Path[j];
		}
		if (temp.size() < 2) break;
		SinglePath::iterator it1, it2;
		bool reg = true;
		for (it1 = temp.begin(); it1 != temp.end(); it1++)
		{
			for (it2 = it1, it2++; it2 != temp.end(); it2++)
			{
				if (*it1 == *it2)
				{
					reg = false;
					break;
				}
			}
			if (!reg)
			{
				break;
			}
			//cout<<*it1<<" ";
		}
		//cout<<endl;
		if (reg)
		{
			kpaths.push_back(temp);  // store the kth shortest path
			ki++;
		}
		kj++;
		//this->Output(); // for debug
	}
	return ki;
}

void KRouting::delete_adj_list(Adjacency_list& map)
{
	for (size_t i = 0; i < map.size(); i++)
	{
		Vertex* v = map.at(i).m_next.get();
		Vertex* p = NULL;
		while (v != NULL)
		{
			p = v;
			v = v->m_next.get();
			delete p;
		}
	}
}

void KRouting::dijkstra(int srcId, PathWeight* dist, int* path)
{

	for (size_t i = 0; i < m_size; i++)
	{
		dist[i] = simu_global::kInfinity;
		path[i] = -1;
	}
	dist[srcId] = 0;
	bool *done = new bool[m_size]; assert(done != NULL);
	for (size_t i = 0; i < m_size; i++)
	{
		done[i] = false;
	}
	//memset(done, 0, m_size);
	priority_queue < Edge > pq;
	Edge edge(srcId, 0);
	pq.push(edge);
	while (!pq.empty())
	{
		Edge out = pq.top();
		pq.pop();
		if (done[out.m_id] == true)
		{
			continue;
		}
		done[out.m_id] = true;
		dist[out.m_id] = out.m_metric;

		for (shared_ptr<Vertex> vertex = m_adj_map[out.m_id].m_next; vertex != false; vertex = vertex->m_next)
		{
			if (done[vertex->m_id] == false)
			{
				if (vertex->m_weightValue + out.m_metric < dist[vertex->m_id])
				{
					dist[vertex->m_id] = vertex->m_weightValue + out.m_metric;
					path[vertex->m_id] = out.m_id;
					Edge in(vertex->m_id, dist[vertex->m_id]);
					pq.push(in);
				}
			}
		}
	}
	delete[] done;
}

double KRouting::dijkstra(int start, int SerEnd, SinglePath& SinglePath)
{
	int* paths = new int[m_size]; assert(paths != NULL);
	PathWeight* dist = new PathWeight[m_size]; assert(dist != NULL);
	dijkstra(start, dist, paths);
	if (dist[SerEnd] > simu_global::kInfinity - 1)
	{
		delete[] paths;
		delete[] dist;
		return -1;
	}
	// parse the shortest path
	int i = SerEnd;
	while (i >= 0)
	{
		SinglePath.push_front(i);
		i = paths[i];
	}
	PathWeight min = dist[SerEnd];
	delete[] paths;
	delete[] dist;
	return min;
}

unsigned KRouting::AddNode(unsigned ni, int preni)
{
	Vertex vertex(m_size, 0);
	m_adj_map.push_back(vertex);
	m_in_map.push_back(vertex);
	for (Vertex* ver = m_in_map[ni].m_next.get(); ver != NULL; ver = ver->m_next.get())
	{
		int id = ver->m_id;
		PathWeight wei = ver->m_weightValue;
		if (id != preni)
		{
			shared_ptr<Vertex>  vertex2(new Vertex(m_size, wei));
			vertex2->m_next = m_adj_map[id].m_next;
			m_adj_map[id].m_next = vertex2;

			shared_ptr<Vertex>  vertex3(new Vertex(id, wei));
			vertex3->m_next = m_in_map[m_size].m_next;
			m_in_map[m_size].m_next = vertex3;
		}
	}
	return m_size++;
}

Vertex& Vertex::operator = (const Vertex& _graph)
{
	m_id = _graph.m_id;
	m_weightValue = _graph.m_weightValue;
	if (_graph.m_next)
	{
		m_next.reset(new Vertex);
		*m_next = *(_graph.m_next);
	}
	else
	{
		m_next.reset();
	}
	return *this;
}
